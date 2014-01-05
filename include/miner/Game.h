/* Game.h - Copyright (c) 2014 Alejandro Martinez Ruiz <alex@flawedcode.org>
 *
 * This class implements the game's business logic.
 *
 * It is a Finite State Machine that performs state transitions and notifies
 * a listener about important game events. It is designed to be as fine
 * grained as needed in order to stop and inspect the state of the game.
 *
 * This class only communicates matches applying to ranges of jewels.
 * How this is to be interpreted regarding the score is left for the
 * listener to define.
 *
 * Note: this is a template class in order to open the possibility to
 * implement Miner::Jewel as part of another object hierarchy.
 */

#ifndef MINER_GAME_H__
#define MINER_GAME_H__

#include <cmath>

#include <deque>
#include <algorithm>
#include <functional>
#include <memory>
#include <type_traits>

#include "miner/Jewel.h"

#include "miner/Event.h"
#include "miner/Listener.h"
#include "miner/Matrix.h"

namespace Miner {

template <typename T>
class Game {
	// T is expected to provide a Miner::Jewel interface
	static_assert(std::is_base_of<Miner::Jewel, T>::value, "Game's template type must be derived from Miner::Jewel!");

	public:

	enum class State {
		AwaitingInput,		// there are no more matches pending
		Swapped,		// a swap has been performed
		Dirty,			// new jewels have been introduced
		Destroyed,		// some jewels have been eliminated
	};

	typedef typename std::deque<Event<T>> EventQueue;

	Game(std::shared_ptr<Matrix<T>> m, Listener<T> *listener,
			unsigned int col_streak_min = 3, unsigned int row_streak_min = 3) :
				m_Matrix{m}, m_Deletions{}, mp_Listener{listener},
				m_ColsStreak{col_streak_min}, m_RowsStreak{row_streak_min} {
		Populate();
	}

	// this should be called to re-populate the matrix (ie. new game)
	void Populate() {
		int col = 0, row = 0;
		int const maxcols = m_Matrix->NumColumns();
		std::for_each(m_Matrix->begin(), m_Matrix->end(),
			[&col, &row, &maxcols](T& jewel) {
				jewel.SetColor(Jewel::RandomColor());
				jewel.SetColRow(col, row);
				if (++col >= maxcols) {
					col = 0;
					++row;
				}
			});
		m_FSMState = State::Dirty;
	}

	// returns whether it is legal to swap to determinate jewels
	bool CanSwap(int col1, int row1, int col2, int row2) {
		bool ret;

		if (col1 == col2)
			ret = abs(row1 - row2) == 1;
		else if (row1 == row2)
			ret = abs(col1 - col2) == 1;
		else
			ret = false;

		return ret;
	}

	// performs a swap operation, return value is the same as CanSwap
	bool Swap(int col1, int row1, int col2, int row2) {
		if (CanSwap(col1, row1, col2, row2)) {
			DoSwap(col1, row1, col2, row2);
			m_FSMState = State::Swapped;
			mp_Listener->Swapped(col1, row1, col2, row2);
			swapcol1 = col1;
			swaprow1 = row1;
			swapcol2 = col2;
			swaprow2 = row2;
			return true;
		}

		return false;
	}

	// returns whether a game needs a swap operation to advance
	bool Ready() const {
		return m_FSMState == State::AwaitingInput;
	}

	// this is the main interface to advance the state machine
	int Go() {
		switch (m_FSMState) {
		case State::Swapped:
		case State::Dirty:
			return GoDirty();
		case State::Destroyed:
			return GoDestroyed();
		case State::AwaitingInput:
		default:
			return 0;
		}
	}

	private:

	std::shared_ptr<Matrix<T>> m_Matrix;
	EventQueue m_Deletions;
	Listener<T> *mp_Listener;
	unsigned int m_ColsStreak, m_RowsStreak;
	State m_FSMState;
	// these are used to remember what to swap in case of
	// a swap operation that failed to produce matches
	unsigned int swapcol1, swaprow1, swapcol2, swaprow2;

	// these are private to avoid copies
	Game(Game const&);
	Game& operator=(Game const&);

	// called when a swap can be performed but matches nothing
	void SwapFailed() {
		DoSwap(swapcol1, swaprow1, swapcol2, swaprow2);
		mp_Listener->SwapFailed(swapcol1, swaprow1, swapcol2, swaprow2);
	}

	// an actual swap operation, independent of result
	void DoSwap(int col1, int row1, int col2, int row2) {
		m_Matrix->Swap(col1, row1, col2, row2);
		m_Matrix->At(col1, row1).SetColRow(col1, row1);
		m_Matrix->At(col2, row2).SetColRow(col2, row2);
	}

	// called in order to scan for changes - aptly named :)
	int GoDirty() {
		int matches = ScanMatrix();

		if (matches > 0) {
			DestroyMatches();
			if (m_FSMState == State::Swapped) {
				mp_Listener->SwapOK(swapcol1, swaprow1, swapcol2, swaprow2);
			}
			m_FSMState = State::Destroyed;
		} else {
			if (m_FSMState == State::Swapped) {
				SwapFailed();
			}
			m_FSMState = State::AwaitingInput;
			mp_Listener->Ready();
		}

		return matches;
	}

	// called in order to "apply gravity" and add new jewels
	int GoDestroyed() {
			Compact();
			m_FSMState = State::Dirty;
			return 0;
	}

	// looks at the deletion events to nullify jewels' colors
	void DestroyMatches() {
		for (auto& deletion : m_Deletions) {
			auto targetnum = deletion.GetTargetNum();
			auto start = deletion.GetStart();
			auto end = start + deletion.GetSize();
			auto lambda = [](int col, int row, T& jewel) { jewel.SetColor(Jewel::Color::None); };

			if (deletion.GetTarget() == Event<T>::Target::Row) {
				m_Matrix->EachInRow(targetnum, lambda, start, end);
			} else {
				m_Matrix->EachInColumn(targetnum, lambda, start, end);
			}
		}

		mp_Listener->Destroyed(m_Deletions.size());
		m_Deletions.clear();
	}

	// returns the number of matches found
	int ScanMatrix() {
		int rows = ScanColRow(Event<T>::Target::Row, m_Matrix->GetRows(), false);
		int cols = ScanColRow(Event<T>::Target::Column, m_Matrix->GetColumns(), true);
		return rows + cols;
	}

	// registers an event and calls the appropriate listeners
	void AddEvent(typename Event<T>::Type type, typename Matrix<T>::ColRow const& colrow,
					typename Event<T>::Target target, int start, int streak) {
		auto const& cstart = colrow.cbegin() + start;
		typename Event<T>::container_type data(cstart, cstart + streak);

		switch (type) {
		case Event<T>::Type::Deletion:
			mp_Listener->Deletion(target, colrow.Num(), start, data);
			for (int i = start; i < start + streak; ++i) {
				mp_Listener->Delete(target, colrow.Num(), i);
			}
			m_Deletions.emplace_back(type, target, colrow.Num(), start, data);
			break;
		case Event<T>::Type::Insertion:
			mp_Listener->Insertion(target, colrow.Num(), start, data);
			break;
		}
	}

	// scan code - looks for matches and registers them, does not touch the matrix
	int ScanColRow(typename Event<T>::Target const target,
				std::vector<typename Matrix<T>::ColRow> const& colrows, bool cols) {
		int matches = 0;
		unsigned int const min_streak = cols ? m_ColsStreak : m_RowsStreak;

		for (auto const& colrow : colrows) {
			unsigned int i = 0, streak = 1;
			Jewel::Color last = Jewel::Color::None;

			for (auto const& jewel : colrow) {
				if (jewel.GetColor() == last) {
					if (jewel.Colored())
						++streak;
				} else {
					if (streak >= min_streak) {
						++matches;
						AddEvent(Event<T>::Type::Deletion, colrow, target, i - streak, streak);
					}

					streak = 1;
					last = jewel.GetColor();
				}

				++i;
			}

			// check for a streak finishing at the edge of the column/row
			if (streak >= min_streak) {
				++matches;
				AddEvent(Event<T>::Type::Deletion, colrow, target, i - streak, streak);
				streak = 1;
			}
		}

		return matches;
	}

	// looks for holes/gaps in the matrix, fills them in with upper and new jewels
	void Compact() {
		for (auto& column : m_Matrix->GetColumns()) {
			// bubble up the gaps...
			int gaps = 0;
			for (int pos = column.size() - 1; pos >= 0; --pos) {
				auto& jewel = column[pos];
				if (jewel.None()) {
					++gaps;
				} else if (gaps > 0) {
					column[pos + gaps].SetColor(jewel.GetColor());
					column[pos].SetColor(Jewel::Color::None);
					mp_Listener->Fall(column.Num(), pos, gaps);
				}
			}

			// ...and fill them in with random jewels
			for (int i = gaps - 1; i >= 0; --i) {
				Jewel& jewel = column[i];
				jewel.SetColRow(column.Num(), i);
				jewel.SetRandomColor();
				mp_Listener->New(column.Num(), i, jewel.GetColor(), gaps);
			}

			if (gaps > 0) {
				m_Matrix->ReplaceColumn(column);
				AddEvent(Event<T>::Type::Insertion, column, Event<T>::Target::Column, 0, gaps);
			}
		}

		mp_Listener->CycleFinished();
	}
};

}	// Miner

#endif
