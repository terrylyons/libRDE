#pragma once

template <typename MAP, typename MIRROR, tjl::my_map_trap<MAP::value_type>* p_trap = 0>
class
	mirrored_map : 
	connect<\> // initialize first!
	, std::map<typename MAP::key_type, typename MAP::mapped_type, typename MAP::key_compare, tjl::trap_alloc<typename MAP::value_type, p_trap> >
{
	// first initialization step has to be to contact the trap and give it a function that allows it to access the set.
	typedef mirrored_map<MAP, MIRROR, p_trap> my_type;
	using connect< MIRROR, p_trap>::my_mirror;
	typedef std::map<typename MAP::key_type, typename MAP::mapped_type, typename MAP::key_compare, tjl::trap_alloc<typename MAP::value_type, p_trap> > my_base;
public:
	using my_base::key_type;
	using my_base::mapped_type;
	using my_base::value_type;
	using my_base::size_type;
	using my_base::difference_type;
	using my_base::key_compare;
	using my_base::allocator_type;
	using my_base::/*std::allocator_traits<allocator_type>::*/pointer;
	using my_base::/*std::allocator_traits<allocator_type>::*/const_pointer;
	using my_base::iterator;
	using my_base::const_iterator;
	using my_base::reverse_iterator;
	using my_base::const_reverse_iterator;
	//	using my_base::node_type;
	//  using my_base::insert_return_type;
	using  my_base::value_compare;

	/// constructors

	template<class ...T>
	mirrored_map(TRAP_T... args) : my_base(args...) {}
	//	  constructs the map
	//	  (public member function)

	~mirrored_map() {}
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

#define BOOLOP(xopx) \
	template< class Key, class T, class Compare, tjl::trap* p_trap, class Compare1  > \
	friend 	bool operator xopx (const mirrored_map<Key, T, Compare, p_trap, Compare1>& lhs, const mirrored_map<Key, T, Compare, p_trap, Compare1>& rhs) \
	{return operator xopx (static_cast<const mirrored_map<Key, T, Compare, p_trap, Compare1>::my_base&>(lhs), static_cast<const mirrored_map<Key, T, Compare, p_trap, Compare1>::my_base&>(rhs)); } \

	BOOLOP(== )
		BOOLOP(!= )
		BOOLOP(< )
		BOOLOP(<= )
		BOOLOP(> )
		BOOLOP(>= )
#undef BOOLOP

		template< class Key, class TRAP_T, class Compare, tjl::trap* p_trap, class Compare1  >
	friend void swap(mirrored_map<Key, TRAP_T, Compare, p_trap, Compare1>& lhs, mirrored_map<Key, TRAP_T, Compare, p_trap, Compare1>& rhs)
	{
		return swap(static_cast<mirrored_map<Key, TRAP_T, Compare, p_trap, Compare1>::my_base&>(lhs), static_cast<mirrored_map<Key, TRAP_T, Compare, p_trap, Compare1>::my_base&>(rhs));
	}
	//std::swap(std::map)
	//specializes the std::swap algorithm
	//(function template)
};


