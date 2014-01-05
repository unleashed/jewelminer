/* Jewel.h - Copyright (c) 2014 Alejandro Martinez Ruiz <alex@flawedcode.org>
 *
 * This class implements the methods required by the Game class to act as
 * an element of the matrix (ie, to be a "jewel").
 *
 * Note that this class can be derived to use more specialized jewels as
 * the elements of the game, although it is not quite mandatory.
 */

#ifndef MINER_JEWEL_H__
#define MINER_JEWEL_H__

#include "util/Random.h"

#include <type_traits>

namespace Miner {

class Jewel {
	public:

	enum class Color {
		None = 0,
		Blue,
		Green,
		Yellow,
		Purple,
		Red,
		Max
	};

	static Color RandomColor() {
		return static_cast<Color>(Util::rand_between(
				static_cast<std::underlying_type<Color>::type>(Color::None) + 1,
				static_cast<std::underlying_type<Color>::type>(Color::Max) - 1));
	}

	Jewel() : Jewel(Color::None) {}
	explicit Jewel(Color color, int col = 0, int row = 0) : m_Color{color}, m_Col{col}, m_Row{row} {}
	virtual ~Jewel() {}

	virtual Color GetColor() const { return m_Color; }
	virtual void SetColor(Color color) { m_Color = color; }
	virtual void SetRandomColor() {
		this->SetColor(Jewel::RandomColor());
	}

	virtual Jewel& operator=(Color color) {
		m_Color = color;
		return *this;
	}

	virtual void SetColRow(int col, int row) {
		m_Col = col;
		m_Row = row;
	}

	int GetColumn() const { return m_Col; }
	int GetRow() const { return m_Row; }

	bool None() const { return this->GetColor() == Color::None; }
	bool Colored() const { return this->GetColor() != Color::None; }

	bool operator==(Jewel const& rhs) const {
		return m_Color == rhs.m_Color;
	}

	bool operator!=(Jewel const& rhs) const {
		return m_Color != rhs.m_Color;
	}

	private:

	Color m_Color;
	int m_Col, m_Row;
};

}	// Miner

#endif
