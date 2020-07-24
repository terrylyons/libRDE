/* *************************************************************

Copyright 2010 Terry Lyons, Stephen Buckley, Djalil Chafai, 
Greg Gyurkó and Arend Janssen. 

Distributed under the terms of the GNU General Public License, 
Version 3. (See accompanying file License.txt)

************************************************************* */



#pragma once

#ifndef __DYNAMICCALYCONSTRUCTEDPATH__
#define __DYNAMICCALYCONSTRUCTEDPATH__


#include "BasePath.h"
#include "DataTree.h"

/// A common ancestor for the paths we construct that contains the DescribePath function.

/// Gives a dynamic construction of DescibePath. We describe the path over an increment based on what we
/// have already computed about the path on other increments and the properties of the path 
/// that will be given in their specific files.
template <typename my_alg_type>
class DynamicallyConstructedPath :
	public BasePath<my_alg_type>
{
public:
	typedef std::map < dyadic_interval, Increment<my_alg_type> > DataTree;
	typedef typename my_alg_type::MAPS MAPS;
	typedef typename my_alg_type::CBH CBH;
	typedef typename my_alg_type::LIE LIE;
	typedef typename my_alg_type::S S;
	typedef typename my_alg_type::TENSOR TENSOR;
	typedef typename my_alg_type::SCA SCA;
	typedef typename my_alg_type::LET LET;
	typedef typename my_alg_type::DEG DEG;

private:

	MAPS mLibAlgMaps;
	CBH mLibAlgCBH;
	mutable DataTree mPathData;

public:

	/// Default Constructor.
	DynamicallyConstructedPath(void){};
	/// Default Destructor.
	~DynamicallyConstructedPath(void){};

	/// Describe the path over a dyadic interval
	LIE DescribePath(const dyadic_interval &increment, const int accuracy) const
	{
		assert(increment.k >= 0);

		// If the answer is known already return it immediately 
		typename DataTree::const_iterator itFound = mPathData.find(increment);
		if (itFound != mPathData.end())
		{	
			// itFound not null at this point
			const Increment<my_alg_type> & nvFound = itFound->second;
			if (nvFound.Accuracy() >= accuracy)
				return nvFound.LieValue();
		}

		// Answer not yet known
		ConstIterator itRoot = mPathData.begin();
		if ( itRoot != mPathData.end() )
		{
			// the database for the path has a root
			if ((itRoot->first).contains(increment))
			{
				// interval to be described is below the root interval in mPathData
				return DescribePath2(increment, accuracy);
			}
			else
			{
				// interval to be described is not below the root interval in mPathData
				return DescribePath1(increment, accuracy);
			}
		}
		else
		{
			// the database for the path is empty and does not have a root
			return DescribePath0(increment, accuracy);
		}
	};	

protected:

	/// The DataTree Iterator.
	typedef typename DataTree::iterator Iterator;
	typedef typename DataTree::const_iterator ConstIterator;
	
	
	/// Given the Lie increment of a dyadic interval, itLeafAbove, and two increments nvLeft and nvRight,
	/// calculates the Lie elments of the increments corresponding to the children of itLeafAbove.
	virtual void ComputeChildLieIncrements( Increment<my_alg_type> & nvLeft, Increment<my_alg_type> & nvRight, ConstIterator itLeafAbove) const
	{
		const dyadic_interval &  diAbove = itLeafAbove->first;
		const Increment<my_alg_type> & nvAbove = itLeafAbove->second;
		assert(Increment<my_alg_type>::IsLeaf(itLeafAbove));
	
		nvLeft.LieValue(nvAbove.LieValue()*S(.5));
		nvRight.LieValue(nvAbove.LieValue()*S(.5));
	};

	virtual void MakeNeighborRootLieIncrement( LIE & ans, const Iterator & OldRoot) const
	{
		// Cannot assume that OldRoot is still the root
		dyadic_interval diIncrement = OldRoot->first;
		diIncrement.flip_interval();
		ans =  LIE(1,double(diIncrement.sup())-double(diIncrement.inf()));
	};

	/// Returns the Lie element for the dyadic_interval increment.
	virtual LIE MakeRootLieIncrement( const dyadic_interval &increment ) const
	{
		return LIE(1,double(increment.sup())-double(increment.inf()));
	};


private:
	/// The Default Rough Path Child Extender

	/// Adds two children intervals to a leaf 
	/// and returns an iterator to the child leaf containing increment.
	Iterator InsertChildrenAndRefinePathData ( Iterator itLeafAbove, const dyadic_interval & increment ) const
	{
		const dyadic_interval &  diAbove = itLeafAbove->first;
		Increment<my_alg_type> & nvAbove = itLeafAbove->second;

		assert(diAbove.contains(increment));
		assert(Increment<my_alg_type>::IsLeaf(itLeafAbove));

		dyadic_interval diLeft(diAbove), diRight(diAbove);
		diLeft.shrink_interval_left();
		diRight.shrink_interval_right();

		Increment<my_alg_type> nvLeft(diLeft,itLeafAbove,mPathData.end()), nvRight(diRight,itLeafAbove,mPathData.end());

		ComputeChildLieIncrements(nvLeft, nvRight, itLeafAbove);

		Iterator itLeft = mPathData.insert(
			itLeafAbove, //hint to position
			typename DataTree::value_type(diLeft,nvLeft) // the interval and its data
			);
		Iterator itRight = mPathData.insert(
			itLeft, //hint to position
			typename DataTree::value_type(diRight,nvRight) // the interval and its data
			);
	
		itLeft->second.mitSibling=itRight;
		itRight->second.mitSibling=itLeft;

	//#ifdef __RelocateUpdateParentAccuracySTEP
		UpdateAboveLeaf(itLeft);
	//#endif
		if (diLeft.contains(increment)) itLeafAbove=itLeft;
		else itLeafAbove=itRight;
		return itLeafAbove;
	};
	

	 // Modifies cbh - should be mutable 

	 /// Updates parent in the DataTree if and only if the sibling has at  
	 /// least the _iAccuracy of the Current Record returns an Iterator to  
	 /// the parent structure in the DataTree if it changes it else returns 
	 /// a copy of the Iterator that it was passed.
	Iterator UpdateParentAccuracy( Iterator itCurrent ) const
	{
		const dyadic_interval & 
			diBelow = itCurrent->first;
		Increment<my_alg_type> & 
			nvBelow = itCurrent->second;
		{
			Iterator & 
				itParent = nvBelow.mitParent ;
			if ( itParent !=  mPathData.end() )
			{	
				// not the root of the datatree
				const dyadic_interval &  
					diParent = itParent->first;
				Increment<my_alg_type> & 
					nvParent = itParent->second;

				// so must have sibling
				Iterator & 
					itSibling = nvBelow.mitSibling ;
				const dyadic_interval &  
					diSibling = itSibling->first;
				Increment<my_alg_type> & 
					nvSibling = itSibling->second;
	
				// update _iAccuracy of parent if possible
				int iAccuracyAvailable = std::min(nvBelow.Accuracy(), nvSibling.Accuracy());
				if ( nvParent.Accuracy() <  iAccuracyAvailable )
				{
					std::vector <const LIE*> 
						pincs(typename std::vector<LIE*>::size_type(2));
					if (diBelow.aligned())
					{
						pincs[0]= & nvBelow.LieValue();
						pincs[1]= & nvSibling.LieValue();
					}
					else
					{
						pincs[1]= & nvBelow.LieValue();
						pincs[0]= & nvSibling.LieValue();
					}
					// the slowest step is next so minimize no of calls to it
					nvParent.LieValue(mLibAlgCBH.full(pincs));
					nvParent.Accuracy(iAccuracyAvailable);
					itCurrent = itParent;
				}
			}
		}	
		return itCurrent;
	};

	/// Fuctions used in implementation of DescribePath
	LIE DescribePath2( const dyadic_interval & increment, const int accuracy ) const
	{
		// interval to be added is already covered by an interval in mPathData
		assert((mPathData.begin()->first).contains(increment) && (accuracy>=increment.n));

		std::pair <Iterator, Iterator> itPair = mPathData.equal_range( increment );

		if (itPair.first != itPair.second) 
		{
			// increment already in mPathData to low accuracy		

			const Iterator & 
				itCurrent = itPair.first;
			const dyadic_interval & 
				diCurrent = itCurrent->first;
			Increment<my_alg_type> & 
				nvCurrent = itCurrent->second;

			// accuracy == nvCurrent.Accuracy() trapped earlier
			assert(accuracy > nvCurrent.Accuracy());


			// increment in mPathData not accurate enough
			dyadic_interval diLeft(diCurrent), diRight(diCurrent);
			diLeft.shrink_interval_left();
			diRight.shrink_interval_right();

			// recurse down
			DescribePath(diLeft, accuracy);
			DescribePath(diRight, accuracy);

			// accuracy gets updated dynamically and is the min depth of the tree below this location
			assert(accuracy <= nvCurrent.Accuracy());

			return nvCurrent.LieValue();
		}
		else
		{
			// increment not in mPathData at any accuracy

			dyadic_interval diRefinedIncrement(increment), diRefinedEnd(increment);
			++diRefinedEnd;

			diRefinedIncrement.shrink_interval_left(accuracy-increment.n);
			diRefinedEnd.shrink_interval_left(accuracy-increment.n);

			for( ; diRefinedIncrement < diRefinedEnd ; ++(++diRefinedIncrement) )
			{
				//get iterator to leaf in mPathData containing diRefinedIncrement
				Iterator itLeafAbove (mPathData.equal_range( diRefinedIncrement ).first);
				itLeafAbove--;

				while ((itLeafAbove->first).contains(diRefinedIncrement) && ((itLeafAbove->first) != diRefinedIncrement))
				{
					itLeafAbove = InsertChildrenAndRefinePathData(itLeafAbove, diRefinedIncrement);
				}	
	//#ifndef __RelocateUpdateParentAccuracySTEP
				UpdateAboveLeaf(itLeafAbove);
	//#endif
			}
			return DescribePath( increment, accuracy );
		}
	};

	LIE DescribePath1( const dyadic_interval & increment, const int accuracy ) const
	{
		// interval to be added is not below the root interval in mPathData
		// move root up in database until the interval is below root and try again
		assert((!((mPathData.begin()->first).contains(increment))) && (accuracy >= increment.n));

		Iterator itRoot = mPathData.begin();
		while (!((itRoot->first).contains(increment)))
		{
			// move root up

			const dyadic_interval & diOldRoot = itRoot->first;
			Increment<my_alg_type> & nvOldRoot = itRoot->second;

			dyadic_interval diNewRoot(diOldRoot);
			diNewRoot.expand_interval();
			dyadic_interval diNeighbour(diNewRoot);
			(diOldRoot.aligned())
				?diNeighbour.shrink_interval_right()
				:diNeighbour.shrink_interval_left();

			Iterator itNeighbour = mPathData.insert( itRoot, typename DataTree::value_type(diNeighbour, Increment<my_alg_type>(diNeighbour,mPathData.end(),mPathData.end()) ) );
			Increment<my_alg_type> & nvNeighbour = itNeighbour->second;
			Iterator itNewRoot = mPathData.insert( itRoot, typename DataTree::value_type(diNewRoot, Increment<my_alg_type>(diNewRoot,mPathData.end(),mPathData.end()) ) );
			Increment<my_alg_type> & nvNewRoot = itNewRoot->second;
			nvOldRoot.mitSibling = itNeighbour;
			nvOldRoot.mitParent = itNewRoot;
			nvNeighbour.mitSibling = itRoot;
			nvNeighbour.mitParent = itNewRoot;
			MakeNeighborRootLieIncrement(nvNeighbour.LieValue(),itRoot);
			UpdateParentAccuracy(itRoot);
			itRoot = mPathData.begin();
		}
		return DescribePath( increment, accuracy );
	};

	LIE DescribePath0( const dyadic_interval & increment, const int accuracy ) const
	{
		// the path database is empty
		assert(mPathData.size()==0	&& (accuracy>=increment.n));
		// insert entry with trivial lie values, the given increment
		// and null pointers to the correct container into the database

		Increment<my_alg_type> nvData(increment,mPathData.end(),mPathData.end());

		Iterator itRoot = mPathData.insert(
			typename DataTree::value_type (increment,nvData) 
			).first;
		// update the Lie Value using the model specific helper function
		itRoot->second.LieValue(MakeRootLieIncrement(increment));

		// go back to get the correct accuracy etc after root inserted
		return DescribePath( increment, accuracy );
	};

	void UpdateAboveLeaf( Iterator itLeft ) const
	{
		Iterator top,below (itLeft);
		top = UpdateParentAccuracy(below);
		while (top != below) 
		{
			below = top;
			top = UpdateParentAccuracy(below);
		}
	};

};

#endif // __DYNAMICCALYCONSTRUCTEDPATH__