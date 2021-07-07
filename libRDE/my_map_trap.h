#pragma once
#include "trap_alloc.h"
#include "type_name.h"
#include <type_traits>
#include <stddef.h>


namespace tjl
{
	//NOTE THAT THIS NEEDS MODIFICATION FOR CLANG COMPILER which does not construct or destroy items of type value_type
	template <typename MAP_T, typename MIRROR_T>
	class my_map_trap
	{
	public:
		typedef MIRROR_T MIRROR;
		typedef MAP_T MAP;
		typedef my_map_trap<MAP, MIRROR> TRAP;
		typedef typename MAP::key_type key_type;
		typedef typename MAP::mapped_type mapped_type;
		typedef typename MAP::value_type value_type;
		typedef typename MAP::key_compare key_compare;

		static_assert(std::is_same< key_type, typename MIRROR::value_type >::value, " key types must match") ;

		template<TRAP* p_trap>
		struct connect{	
			MIRROR mirror; // the mirror data
			connect() : mirror(){p_trap->Mirror(&mirror);} // informing MY_MAP_TRAP of the location of the mirror - could also pass the address of the parent class to reuse the trap
		};

		bool Is_connected() const { return p_mirror != 0; }
		const ptrdiff_t offset;
		my_map_trap(ptrdiff_t offst = 0) : p_mirror(0), offset(offst) {}

		~my_map_trap() {}

	private:	
		template <typename  U, typename TRAP_T, const TRAP_T* V> friend struct trap_alloc;

		// the value types of MAP and MIRROR must be convertible
		typename MIRROR::value_type convert(const value_type& arg) const {
			return arg.first;
		}
		typename MIRROR::value_type convert(value_type&& arg) const {
			return arg.first;
		}

		// accessors for p_mirror
		MIRROR* Mirror() const { return p_mirror; }
		void Mirror(MIRROR* val) { p_mirror = val; }
        
		MIRROR* Mirror(char* p_allocator_instance) const { 
			return (MIRROR*)(p_allocator_instance + offset);
		}
		// must reflect any other calls to construct or destroy with other types 
		// containers like map generate them (although it is against the standard)

		void inserted(const value_type & arg) const {
			//std::cout << type_name<decltype(arg)>() << " inserted\n";
			Mirror()->insert(convert(arg));
		}

		void to_be_erased(const value_type & arg) const {
			//std::cout << type_name<decltype(arg)>() << " destroyed\n";
			Mirror()->erase(convert(arg));
		}

		template<typename T> void inserted(const T& arg) const {
			//std::cout << type_name<decltype(arg)>() << "+ non-standard\n";
		}

		template<typename T> void to_be_erased(const T& arg) const {
			//std::cout << type_name<decltype(arg)>() << "- non-standard\n";
		}
		/// with call back
		void inserted(const value_type & arg, char* allocator_instance) const {
			//std::cout << type_name<decltype(arg)>() << " inserted\n";
			Mirror(allocator_instance)->insert(convert(arg));
		}

		void to_be_erased(const value_type & arg, char* allocator_instance) const {
			//std::cout << type_name<decltype(arg)>() << " destroyed\n";
			Mirror(allocator_instance)->erase(convert(arg));
		}

		template<typename T> void inserted(const T& arg, char* allocator_instance) const {
			//std::cout << type_name<decltype(arg)>() << "+ non-standard\n";
		}

		template<typename T> void to_be_erased(const T& arg, char* allocator_instance) const {
			//std::cout << type_name<decltype(arg)>() << "- non-standard\n";
		}
		// maintain mirror

	private:
		
		MIRROR* p_mirror;
	};

	// //usage
	// typedef my_map_trap<MAP,MIRROR> MY_TRAP;
	// // MAP and MIRROR must have same key_type
	// MY_TRAP trap; // needs to be const in accordance with non-type template argument requirements 
	// my_mirrored_map<MY_TRAP, &trap> mp;
	// mp[key] = value; 
	// mp.my_mirror.find(key) == true;

	template < typename MY_TRP, MY_TRP* p_trap>
	class
		my_mirrored_map	: MY_TRP::template connect < p_trap > // build mirror and initialize trap to connect to it first!
		, std::map <
		  typename MY_TRP::key_type // MAP but with trap allocator
		, typename MY_TRP::mapped_type
		, typename MY_TRP::key_compare
		, trap_alloc<typename MY_TRP::value_type, MY_TRP, p_trap>
		>
	{
		typedef my_mirrored_map<MY_TRP, p_trap> my_type;
		typedef std::map <typename MY_TRP::key_type // MAP but with trap allocator
			, typename MY_TRP::mapped_type
			, typename MY_TRP::key_compare
			, trap_alloc<typename MY_TRP::value_type, MY_TRP, p_trap>
		> my_base;
	public:
		using MY_TRP::template connect<p_trap>::mirror;

		using typename my_base::key_type;
		using typename my_base::mapped_type;
		using typename my_base::value_type;
		using typename my_base::size_type;
		using typename my_base::difference_type;
		using typename my_base::key_compare;
		using typename my_base::allocator_type;
		using typename my_base::pointer;
		using typename my_base::const_pointer;
		using typename my_base::iterator;
		using typename my_base::const_iterator;
		using typename my_base::reverse_iterator;
		using typename my_base::const_reverse_iterator;
		//	using typename my_base::node_type;
		//  using typename my_base::insert_return_type;
		using typename my_base::value_compare;

		/// constructors

		template<class ...T>
		my_mirrored_map(T... args) : my_base(args...) {}
		//	  constructs the map
		//	  (public member function)

		~my_mirrored_map() {}
		//destructs the map
		//(public member function)

		my_type& operator=(const my_type& rhs)
		{
			my_base::operator=(rhs);
			return (*this);
		}

		my_type& operator=(my_type&& rhs)
		{
			my_base::operator=(std::move(rhs));
			return (*this);
		}

		my_type& operator=(std::initializer_list<value_type> init_list)
		{
			clear();
			insert(init_list);
			return (*this);
		}

		//assigns values to the container
		//(public member function)

		using  my_base::get_allocator;
		//returns the associated allocator
		//(public member function)

		///		  Element access

		using  my_base::at;
		//(C++11)
		//access specified element with bounds checking
		//(public member function)

		using  my_base::operator[];
		//access specified element
		//(public member function)

		///		  Iterators

		using  my_base::begin;
		using  my_base::cbegin;
		//returns an iterator to the beginning
		//(public member function)

		using  my_base::end;
		using  my_base::cend;
		//returns an iterator to the end
		//(public member function)

		using  my_base::rbegin;
		using  my_base::crbegin;
		//returns a reverse iterator to the beginning
		//(public member function)

		using  my_base::rend;
		using  my_base::crend;
		//returns a reverse iterator to the end
		//(public member function)

		///		  Capacity

		using  my_base::empty;
		//checks whether the container is empty
		//(public member function)

		using  my_base::size;
		//returns the number of elements
		//(public member function)

		using  my_base::max_size;
		//returns the maximum possible number of elements
		//(public member function)

		///		  Modifiers

		using  my_base::clear;
		//clears the contents
		//(public member function)

		using  my_base::insert;
		//inserts elements or nodes(since C++17)
		//(public member function)

		using  my_base::insert_or_assign;
		//(C++17)
		//inserts an element or assigns to the current element if the key already exists
		//(public member function)

		using  my_base::emplace;
		//(C++11)
		//constructs element in - place
		//(public member function)

		using  my_base::emplace_hint;
		//(C++11)
		//constructs elements in - place using a hint
		//(public member function)

		using  my_base::try_emplace;
		//(C++17)
		//inserts in - place if the key does not exist, does nothing if the key exists
		//(public member function)

		using  my_base::erase;
		//erases elements
		//(public member function)

		using  my_base::swap;
		//swaps the contents
		//(public member function)

		//	using  my_base::extract;
		//(C++17)
		//extracts nodes from the container
		//(public member function)

		//	using  my_base::merge;
		//(C++17)
		//splices nodes from another container
		//(public member function)

		///		  Lookup

		using  my_base::count;
		//returns the number of elements matching specific key
		//(public member function)

		using  my_base::find;
		//finds element with specific key
		//(public member function)

		using  my_base::equal_range;
		//returns range of elements matching a specific key
		//(public member function)

		using  my_base::lower_bound;
		//returns an iterator to the first element not less than the given key
		//(public member function)

		using  my_base::upper_bound;
		//returns an iterator to the first element greater than the given key
		//(public member function)

		///		  Observers

		using  my_base::key_comp;
		//returns the function that compares keys
		//(public member function)

		using  my_base::value_comp;
		//returns the function that compares keys in objects of type value_type
		//(public member function)

		///		  Non - member functions

		//template< class my_type > 

#define BOOLOP(xopx) \
	friend 	bool operator xopx (const my_type& lhs, const my_type& rhs) \
	{return operator xopx (static_cast<const my_base&>(lhs), static_cast<const my_base&>(rhs)); } \

		BOOLOP(== );
		BOOLOP(!= );
		BOOLOP(< );
		BOOLOP(<= );
		BOOLOP(> );
		BOOLOP(>= );
#undef BOOLOP

		//template< class my_type >
		friend void swap(my_type& lhs, my_type& rhs)
		{
			swap(static_cast<my_base&>(lhs), static_cast<my_base&>(rhs));
			swap(lhs.mirror, rhs.mirror);
		}
	};
}

