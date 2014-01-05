/* Listener.h - Copyright (c) 2014 Alejandro Martinez Ruiz <alex@flawedcode.org>
 *
 * This class declares an interface to be implemented by inheriting classes
 * in order to get notified about transitions in the game's matrix.
 */

#ifndef MINER_LISTENER_H__
#define MINER_LISTENER_H__

#include "miner/Event.h"
#include "miner/Jewel.h"

namespace Miner {

template <class T>
class Listener {
	public:
		virtual void Swapped(int col1, int row1, int col2, int row2) = 0;
		virtual void SwapOK(int col1, int row1, int col2, int row2) = 0;
		virtual void SwapFailed(int col1, int row1, int col2, int row2) = 0;
		virtual void Ready() = 0;
		virtual void Destroyed(int matches) = 0;
		virtual void Deletion(typename Event<T>::Target target, int num, int start, typename Event<T>::container_type& container) = 0;
		virtual void Insertion(typename Event<T>::Target target, int num, int start, typename Event<T>::container_type& container) = 0;
		virtual void CycleFinished() = 0;
		virtual void Fall(int column, int row, int gaps) = 0;
		virtual void New(int column, int row, Jewel::Color color, int totalgaps) = 0;
		virtual void Delete(typename Event<T>::Target target, int colrow, int pos) = 0;

	protected:
		Listener() {}
		~Listener() noexcept {}
	private:
		Listener(Listener const&);
		Listener& operator=(Listener const&);
};

}	// Miner

#endif
