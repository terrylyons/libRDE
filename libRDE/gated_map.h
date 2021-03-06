#pragma once
#pragma once
// derived from http://en.cppreference.com/w/cpp/container/map
#include <map>
/// gated map is a map class that also includes as a member "ledger" a set that contains all keys currently in the map
/// it may take a different predicate and allocator to the original map (and so sorts the keys differently)
template<
	class Kty,
	class Ty,
	class Pr,
    class Pr1,
	class Alloc,
	class Alloc1,
	typename... T> class
gated_map : std::map<Kty, Ty, Pr, Alloc, T...>
{
	typedef gated_map<Kty, Ty, Pr, Alloc, T...> my_type;
	typedef std::map<Kty, Ty, Pr, Alloc, T...> my_base;
	typedef std::set<Kty, Pr1, Alloc1, T...> ledger_type;
	ledger_type ledger;
public:
	using my_base::key_type;
	using my_base::mapped_type;
	using my_base::value_type;
private:
	std::set<key_type, key_compare> 
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

	/////////////////////////////////////////
	template<class ...T>
	gated_map(T... args) : my_base(args...) {
		for (auto a& : (*this) )
			ledger.insert(a.first);
	}
	//	  constructs the map
	//	  (public member function)

	/////////////////////////////////////////
	~gated_map() {}
	//destructs the map
	//(public member function)

	/////////////////////////////////////////
	my_type& operator=(const my_type& rhs)
	{
		my_base::operator=(rhs);
		ledger::operator=(rhs.ledger);
		return (*this);
	}

	/////////////////////////////////////////
	my_type& operator=(my_type&& rhs)
	{
		my_base::operator=(std::move(rhs));
		ledger::operator=(std::move(rhs.ledger));
		return (*this);
	}

	/////////////////////////////////////////
	my_type& operator=(std::initializer_list<value_type> init_list)
	{
		clear();
		insert(init_list);
		for (auto a& : (*this))
			ledger.insert(a.first);
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

	/////////////////////////////////////////
	mapped_type& operator[](const key_type& key){
		auto found = find(key);
		if (found == end())
			ledger.insert(key);
		return my_base::operator[](key);
		}

	mapped_type& operator[](key_type&& key) {
		auto found = find(key);
		if (found == end())
			ledger.insert(key);
		return my_base::operator[](key);
	}

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
	
	/////////////////////////////////////////
	inline void clear()
	{
		my_base::clear();
		ledger.clear();
	}
	//clears the contents
	//(public member function)

	/////////////////////////////////////////
	using  my_base::insert;
	//inserts elements or nodes(since C++17)
	//(public member function)

	/////////////////////////////////////////
	using  my_base::insert_or_assign;
	//(C++17)
	//inserts an element or assigns to the current element if the key already exists
	//(public member function)

	/////////////////////////////////////////
	using  my_base::emplace;
	//(C++11)
	//constructs element in - place
	//(public member function)

	/////////////////////////////////////////
	using  my_base::emplace_hint;
	//(C++11)
	//constructs elements in - place using a hint
	//(public member function)

	/////////////////////////////////////////
	using  my_base::try_emplace;
	//(C++17)
	//inserts in - place if the key does not exist, does nothing if the key exists
	//(public member function)

	/////////////////////////////////////////
	using  my_base::erase;
	//erases elements
	//(public member function)

	/////////////////////////////////////////
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
	template< class Key, class T, class Compare, class Alloc > \
	friend 	bool operator xopx (const CLONE_MAP<Key, T, Compare, Alloc>& lhs, const CLONE_MAP<Key, T, Compare, Alloc>& rhs) \
	{return operator xopx (static_cast<const CLONE_MAP<Key, T, Compare, Alloc>::my_base&>(lhs), static_cast<const CLONE_MAP<Key, T, Compare, Alloc>::my_base&>(rhs)); } \

	BOOLOP(== )
		BOOLOP(!= )
		BOOLOP(< )
		BOOLOP(<= )
		BOOLOP(> )
		BOOLOP(>= )
#undef BOOLOP

		/////////////////////////////////////////
		template< class Key, class T, class Compare, class Alloc >
	friend void swap(gated_map<Key, T, Compare, Alloc>& lhs, gated_map<Key, T, Compare, Alloc>& rhs)
	{
		return swap(static_cast<gated_map<Key, T, Compare, Alloc>::my_base&>(lhs), static_cast<gated_map<Key, T, Compare, Alloc>::my_base&>(rhs));
	}
	//std::swap(std::map)
	//specializes the std::swap algorithm
	//(function template)
};
