/* *************************************************************

Copyright 2010 Terry Lyons, Stephen Buckley, Djalil Chafai, 
Greg Gyurkó and Arend Janssen. 

Distributed under the terms of the GNU General Public License, 
Version 3. (See accompanying file License.txt)

************************************************************* */



#pragma once

#ifndef DataTree_h__
#define DataTree_h__


#include "dyadic_interval.h"
#include <map>

template <typename my_alg_type>
class Increment;

/// A class used in tree structures to identify iterators to relatives

/// A class used in tree structures to identify iterators to relatives
/// which defaults to null pointers but can be constructed otherwise.
template <typename my_alg_type>
class FamilyConnections
{
	typedef std::map < dyadic_interval, Increment<my_alg_type> > DataTree;

public:		
	typename DataTree::iterator 
		mitParent, 
		mitSibling;

	/// Constructor that initializes the iterator for Parent and the 
	/// iterator for sibling.
	FamilyConnections(
		const typename DataTree::iterator itParent , 
		const typename DataTree::iterator itSibling 
		): mitParent( itParent ), mitSibling( itSibling )
	{
	}
};

/// Class containing the _LieValue for a given dyadic interval.

/// Contains the _LieValue at a given dyadic interval and
/// _iAccuracy which is always initialised to match the interval.
/// If provided with an iterator to the parent that is currently 
/// a leaf will generate conditional distribution
/// needs access to the normal random number generator.
template <typename my_alg_type>
class Increment : public FamilyConnections<my_alg_type>
{
	typedef typename my_alg_type::TENSOR TENSOR;
	typedef typename my_alg_type::LIE LIE;
	typedef typename my_alg_type::MAPS MAPS;
	typedef typename my_alg_type::CBH CBH;
	typedef typename my_alg_type::SCA SCA;
	typedef typename my_alg_type::RAT RAT;
	typedef typename my_alg_type::S S;
	typedef typename my_alg_type::Q Q;
	typedef typename my_alg_type::DEG DEG;
	typedef typename my_alg_type::LET LET;

	static const unsigned DEPTH = my_alg_type::DEPTH;
	static const unsigned myDIM = my_alg_type::myDIM;
	static const unsigned ALPHABET_SIZE = my_alg_type::ALPHABET_SIZE;

	typedef std::map < dyadic_interval, Increment<my_alg_type> > DataTree;
protected:
	/// The increment.
	LIE _LieValue;
	
	/// The depth below to which this increment is calculated.
	int _iAccuracy;
	

public:
	/// Constructs node from increment and can set family connections if requested.
	Increment(
		const dyadic_interval inc = dyadic_interval(), 
		typename DataTree::iterator itParent = DataTree::iterator(), 
		typename DataTree::iterator itSibling = DataTree::iterator() 
		) 
		: FamilyConnections<my_alg_type>( itParent , itSibling ),_LieValue(),_iAccuracy(inc.n)		
	{
	}

	/// Retrieves the Lie Increment
	const LIE& LieValue() const { return _LieValue; } // needed to preserve external const-ness
	LIE& LieValue() { return _LieValue; } // needed because CBH is not const 
	
	/// Sets the Lie Increment
	void LieValue(LIE val) { _LieValue = val; }
	
	/// Retrieves _iAccuracy
	int Accuracy() const { return _iAccuracy; }
	
	/// Sets _iAccuracy
	void Accuracy(int val) { _iAccuracy = val; }
	
	/// A Boolean function for an iterator pointing to a pair in DataTree 
	/// (a dyadic_interval and Increment) returning whether the 
	/// length of the interval and accuracy of the increment match.

	static bool IsLeaf(const typename DataTree::const_iterator & itIncDescriptor)
	{
		const dyadic_interval & 
			diIncDescriptor = (itIncDescriptor->first);
		const Increment<my_alg_type> & 
			nvIncDescriptor = (itIncDescriptor->second);
		return nvIncDescriptor.Accuracy()==diIncDescriptor.n;
	}
};

#endif // DataTree_h__