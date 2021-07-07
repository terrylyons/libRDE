#pragma once
#include <memory>
#include "type_name.h"
#include <type_traits>
//#include <map>
//#include <set>
//#include <memory>
// a complete custom allocator for standard containers that "traps" each construction and destruction


namespace tjl
{
	// http://docs.roguewave.com/sourcepro/11.1/html/toolsug/11-6.html sets out the required features
	// implemented in a custom allocator (note the misprint
	// where allocator is used instead of my_allocator??)
	// http://www.boost.org/doc/libs/1_54_0/doc/html/allocator_traits.html explains how allocator_traits interacts
	// with and uses member functions
	// see http://www.codesynthesis.com/~boris/blog/2012/03/20/delaying-function-signature-instantiation-cxx11/
	// for hints on instantiation order

	// the custom allocator derived privately from std::allocator<A>
	template <typename  A, typename TRAP_T, const TRAP_T* p_trap>
	struct trap_alloc : private std::allocator<A>
	{
		template <typename  U, typename TRAP_T1, const TRAP_T1* V> friend struct trap_alloc;
		
		typedef std::allocator<A> base;

		using base::address;
		using base::allocate;
		using base::deallocate;
		using base::max_size;

		using typename base::size_type;
		using typename base::difference_type;
		using typename base::pointer;
		using typename base::const_pointer;
		using typename base::reference;
		using typename base::const_reference;
		using typename base::value_type;

		template <typename Type> struct rebind {
			typedef trap_alloc<Type, TRAP_T, p_trap> other;
		};

		trap_alloc() throw() : base() {};

		trap_alloc(const trap_alloc& arg) throw () : base(arg) {};

		template <typename U>
		trap_alloc(const trap_alloc<U, TRAP_T, p_trap>& arg) : base(static_cast<const typename trap_alloc<U, TRAP_T, p_trap>::base&>(arg)) {};

		template <typename U>
		trap_alloc& operator = (const trap_alloc<U,TRAP_T, p_trap>& arg) throw()
		{
			base::operator = (static_cast<const typename trap_alloc<U, TRAP_T, p_trap>::base&>(arg));
			return (*this);
		};

		template< typename T, typename... Args >
		void construct(T* p, Args&&... args)
		{
			// remove the const from null pointer from the value_type and construct T in situ (no memory allocation)
			::new (const_cast<void*>(static_cast<const void*>(p))) T(std::forward<Args>(args)...);
			// clang cant survive the version below:
			//base::construct(p, std::forward<Args>(args)...);
			//std::cout << "construct " << type_name<decltype(p)>() << "\n";
			//struct std::pair<class basic_dyadic_interval<10333,class basic_dyadic<int,int> > const ,unsigned int> *
			p_trap->inserted(*p);
			//std::cout << this << "\n";
		}

		template <typename U>
		void destroy(U* p)
		{
			p_trap->to_be_erased(*p);
			// match decision above re clang
			//base::destroy(p);
			//std::cout << "destroy " << type_name<decltype(p)>() << "\n" << std::endl;
			p->~U();
		}

		template <typename U>
		friend bool operator == (const trap_alloc& a, const trap_alloc<U, TRAP_T ,p_trap>& b)
		{
			return true;
		}

		template <typename U>
		friend bool operator != (const trap_alloc& a, const trap_alloc<U, TRAP_T, p_trap>& b)
		{
			return !(a == b);
		}
	};
}

//// a container
//typedef std::map<int, double > my_base_container;
//
//typedef my_base_container::value_type my_value_type;
//typedef my_base_container::key_type my_key_type;
//typedef my_base_container::mapped_type my_mapped_type;
//typedef my_base_container::key_compare my_key_compare;
//
//trap p;
//// a trapped version of the container
//typedef std::map<my_key_type, my_mapped_type
//	//>
//	, my_key_compare
//	, trap_alloc<my_value_type, &p > >
//	my_cache;
//
//int main()
//{
//	{
//		my_cache a;
//		std::cout << "Container constructed" << std::endl;
//		for (int i = 1; i < 1000; i += i)
//			a[i] = 1. + i;
//		for (auto k : a) std::cout << k.first << " " << k.second << "\n";
//		a.clear();
//		std::cout << "Container cleared\n";
//	}
//	std::cout << "Container destructed" << std::endl;
//
//	return 0;
//}
