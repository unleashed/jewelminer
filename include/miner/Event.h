/* Event.h - Copyright (c) 2014 Alejandro Martinez Ruiz <alex@flawedcode.org>
 *
 * The Event template class provides a way to store a description of each
 * change that the game performs on a Matrix as a direct consequence of
 * applying the game's rules.
 *
 * The two types of events correspond to insertions and deletions, and they
 * apply to rows or columns, specified as the event's target. Further data
 * includes the target's offset and a container with a copy of the original
 * elements being deleted or inserted.
 *
 * This is currently only used internally by the Game class.
 */

#ifndef MINER_EVENT_H__
#define MINER_EVENT_H__

#include <vector>

namespace Miner {

template <typename T>
class Event {
	public:

	typedef typename std::vector<T> const container_type;

	enum class Type {
		Deletion,
		Insertion,
	};

	enum class Target {
		Row,
		Column
	};

	Event(Type type, Target target, int targetnum, int start, container_type& container) :
			m_Type{type}, m_Target{target}, m_TargetNum{targetnum},
			m_Start{start}, m_Container{container} {}

	// accessors
	Type GetType() const { return m_Type; }
	Target GetTarget() const { return m_Target; }
	int GetTargetNum() const { return m_TargetNum; }
	int GetStart() const { return m_Start; }
	int GetSize() const { return m_Container.size(); }
	container_type const& GetContainer() const { return m_Container; }
	container_type& GetContainer() {
		return const_cast<container_type&>( static_cast<Event<T> const&>(*this).GetContainer() );
	}

	private:

	Type m_Type;
	Target m_Target;
	int m_TargetNum;
	int m_Start;
	container_type m_Container;
};

}	// Miner

#endif
