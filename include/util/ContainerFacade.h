/* ContainerFacade.h - Copyright (c) 2014 Alejandro Martinez Ruiz <alex@flawedcode.org>
 *
 * These classes are useful to _wrap_ some standard container, effectively compositing
 * it along with extra data and/or behavior in derived classes.
 */

#ifndef UTIL_CONTAINERFACADE_H__
#define UTIL_CONTAINERFACADE_H__

#include <type_traits>
#include <utility>
#include <vector>

namespace Util {

// base container facade
template <typename T, typename C>
class ContainerFacade {
	public:
		typedef typename C::value_type value_type;
		static_assert(std::is_same<T, value_type>::value,
			"Error: the type of the underlying container differs from the one specified");
		typedef typename C::size_type size_type;
		typedef typename C::const_iterator const_iterator;
		typedef typename C::iterator iterator;

		virtual ~ContainerFacade() noexcept {}

		bool empty() const { return m_Container.empty(); }
		size_type size() const { return m_Container.size(); }
		iterator begin() { return m_Container.begin(); }
		iterator end() { return m_Container.end(); }
		const_iterator begin() const { return m_Container.cbegin(); }
		const_iterator end() const { return m_Container.cend(); }
		const_iterator cbegin() const { return m_Container.cbegin(); }
		const_iterator cend() const { return m_Container.cend(); }

		C m_Container;

	protected:

		ContainerFacade() : m_Container{} {}
		ContainerFacade(size_type capacity) : m_Container(capacity) {}
		ContainerFacade(ContainerFacade const& rhs) : m_Container{rhs.m_Container} {}
		ContainerFacade(ContainerFacade&& rhs) : m_Container{std::move(rhs.m_Container)} {}
		ContainerFacade& operator=(ContainerFacade const& rhs) {
			m_Container = rhs.m_Container;
			return *this;
		}
		ContainerFacade& operator=(ContainerFacade&& rhs) {
			m_Container = std::move(rhs.m_Container);
			return *this;
		}
};

// Random Access container facade
template <typename T, typename C = std::vector<T>>
class RAContainerFacade : public ContainerFacade<T, C> {
	public:
		typedef typename ContainerFacade<T, C>::value_type value_type;
		typedef typename ContainerFacade<T, C>::size_type size_type;
		typedef typename ContainerFacade<T, C>::const_iterator const_iterator;
		typedef typename ContainerFacade<T, C>::iterator iterator;

		value_type const& operator[](size_type index) const {
			return this->m_Container[index];
		}
		value_type& operator[](size_type index) {
			return const_cast<T&>( static_cast<RAContainerFacade const&>(*this)[index] );
		}

	protected:
		RAContainerFacade() = default;
		RAContainerFacade(size_type capacity) : ContainerFacade<T, C>{capacity} {}
		RAContainerFacade(RAContainerFacade const& rhs) : ContainerFacade<T, C>{rhs} {}
		RAContainerFacade(RAContainerFacade&& rhs) : ContainerFacade<T, C>{rhs} {}
		RAContainerFacade& operator=(RAContainerFacade const& rhs) {
			ContainerFacade<T, C>::operator=(rhs);
			return *this;
		}
		RAContainerFacade& operator=(RAContainerFacade&& rhs) {
			ContainerFacade<T, C>::operator=(rhs);
			return *this;
		}
};

}	// Util

#endif
