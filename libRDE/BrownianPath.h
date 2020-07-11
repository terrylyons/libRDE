/* *************************************************************

Copyright 2010 Terry Lyons, Stephen Buckley, Djalil Chafai, 
Greg Gyurkó and Arend Janssen. 

Distributed under the terms of the GNU General Public License, 
Version 3. (See accompanying file License.txt)

************************************************************* */



#pragma once

#ifndef __BROWNIANPATH__
#define __BROWNIANPATH__


#include "DynamicallyConstructedPath.h"
#include "NormalRandomNumberGenerator.h"

/// A Brownian Path

/// An implementation of Brownian motion built on DynamicallyConstructedPath. Gaussian
/// random variables are obtained using NormalRandomNumberGenerator

template <typename my_alg_type>
class BrownianPath :
	public DynamicallyConstructedPath<my_alg_type>
{

	typedef typename my_alg_type::TENSOR TENSOR;
	typedef typename my_alg_type::LIE LIE;
	typedef typename my_alg_type::MAPS MAPS;
	typedef typename my_alg_type::CBH CBH;
	typedef typename my_alg_type::SCA SCA;
	typedef typename my_alg_type::S S;
	typedef typename my_alg_type::LET LET;
	typedef typename my_alg_type::DEG DEG;

	typedef std::map < dyadic_interval, Increment<my_alg_type> > DataTree;
	typedef typename DataTree::iterator Iterator;
	typedef typename DataTree::const_iterator ConstIterator;

	static const unsigned myDIM = my_alg_type::myDIM;

	/// Default NormalRandomNumberGenerator - used if another is not specified in the constructor.
	static NormalRandomNumberGenerator vgCommonNormal;

	/// Dimension of the subspace in which the path lives.
	const unsigned int dim;

	/// The Lie element given by the increment of Brownian motion on an interval of length t.
	virtual LIE GaussianIncrement( double t ) const
	{
		unsigned int d1;
		if (dim <= myDIM)
		{
			d1 = dim;
		}
		else
		{
			d1 = myDIM;
		}

		LIE l;
		for(unsigned int i = 1; i <= d1; i++)
		{
			l += LIE(LET(i),S(vgNormal(t)));
		}	
		return l;
	}

protected:

	/// The NormalRandomNumberGenerator used.
	NormalRandomNumberGenerator & vgNormal;

public:	
	/// Default Constructor
	BrownianPath(void)  : vgNormal(vgCommonNormal), dim(myDIM) {};

	/// Constructor with a specified normal random number generator.
	BrownianPath(NormalRandomNumberGenerator & rand, unsigned int d=my_alg_type::myDIM) : vgNormal( rand ), dim(d) {};

	///Default destructor.
	~BrownianPath(void){};

	/// Implementation of ComputeChildLieIncrements.
	void ComputeChildLieIncrements( Increment<my_alg_type> & nvLeft, Increment<my_alg_type> & nvRight, ConstIterator itLeafAbove ) const
	{
		const dyadic_interval &  diAbove = itLeafAbove->first;
		const Increment<my_alg_type> & nvAbove = itLeafAbove->second;

		LIE perturbation = GaussianIncrement((diAbove.sup()-diAbove.inf())*.25);
		LIE mean = nvAbove.LieValue()*S(.5);
		nvLeft.LieValue (mean+perturbation);
		nvRight.LieValue(mean-perturbation);
	}

	/// Implementation of MakeRootLieIncrement.
	LIE MakeRootLieIncrement( const dyadic_interval &increment ) const
	{
		return GaussianIncrement( increment.sup()-increment.inf());
	}

	/// Implementation of MakeNeighborRootLieIncrement
	void MakeNeighborRootLieIncrement( LIE & ans, const Iterator & OldRoot ) const
	{
		// Cannot assume that OldRoot is still the root
		dyadic_interval diIncrement = OldRoot->first;
		diIncrement.flip_interval();
		ans = GaussianIncrement( diIncrement.sup() - diIncrement.inf());
	}
};

template <typename my_alg_type> NormalRandomNumberGenerator BrownianPath<my_alg_type>::vgCommonNormal;

#endif // __BROWNIANPATH__
