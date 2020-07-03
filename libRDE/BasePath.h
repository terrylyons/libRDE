/* *************************************************************

Copyright 2010 Terry Lyons, Stephen Buckley, Djalil Chafai,
Greg Gyurkó and Arend Janssen.

Distributed under the terms of the GNU General Public License,
Version 3. (See accompanying file License.txt)

************************************************************* */

#pragma once

#ifndef __BASEPATH__
#define __BASEPATH__

#include "dyadic_interval.h"
#include <deque>

/// A common ancestor for all given paths.

/// BasePath contains the functions that describe the path as a Lie element
/// for any given interval. These are virtual functions, allowing us to define them
/// later for specific paths.
template <typename my_alg_type>
class BasePath
{
protected:
	typedef typename my_alg_type::TENSOR TENSOR;
	typedef typename my_alg_type::LIE LIE;
	typedef typename my_alg_type::MAPS MAPS;
	typedef typename my_alg_type::CBH CBH;
	typedef typename my_alg_type::SCA SCA;
	typedef typename my_alg_type::S S;
	typedef typename my_alg_type::LET LET;
	typedef typename my_alg_type::DEG DEG;

	static const unsigned myDIM = my_alg_type::myDIM;

public:
	BasePath(void) {};
	virtual ~BasePath(void) {};

	/// The description of the path

	/// Describe the path over a dyadic interval by a Lie element
	LIE virtual DescribePath(const dyadic_interval &increment, const int accuracy) const = 0
	{
		return LIE();
	};

	/// Describe the path over a dyadic interval using the default accuracy deduced from the interval
	LIE DescribePath(const dyadic_interval &increment) const
	{
		return DescribePath(increment, increment.n);
	};

	/// Describe the path over a real interval
	virtual LIE DescribePath(double inf, double sup, int tolerance) const
		/* Describes a Brownian Path across a real interval */
		/* resolving the interval and the path used in the */
		/* computation to a level of time 2^(-_iAccuracy) */
	{
		std::deque<dyadic_interval> stack = to_dyadic_intervals<dyadic_interval::intervaltype, dyadic>(inf, sup, tolerance, dyadic_interval::intervaltype);
		std::vector <LIE > incs;
		std::vector <LIE* > pincs;
		for (std::deque<dyadic_interval>::iterator it = stack.begin(); it != stack.end(); ++it)
		{
			LIE temp = DescribePath(*it, tolerance);
			incs.push_back(temp);
		}
		for(auto & lie_inc : incs ) pincs.push_back(&lie_inc);
		return LibAlgCBH.full(pincs);
	};

	/// Describe the path signature over a dyadic interval using the specified time accuracy
	TENSOR Signature(const dyadic_interval &increment, const int accuracy) const
	{
		MAPS maps;
		return exp(maps.l2t(DescribePath(increment, accuracy)));
	};

	/// Describe the path signature over a dyadic interval using the default accuracy deduced from the interval
	TENSOR Signature(const dyadic_interval &increment) const
	{
		MAPS maps;
		return exp(maps.l2t(DescribePath(increment, increment.n)));
	};

	/// Describe the path signature over a real interval with specified time accuracy 2^-(tolerance)
	TENSOR Signature(double inf, double sup, int tolerance) const
	{
		MAPS maps;
		return exp(maps.l2t(DescribePath(inf, sup, tolerance)));
	};
protected:
	// an object for manipulating LIE elements
	// LIE LibAlgCBH.full(std::vector<LIE>)
	// computes the campbell hausdorff product of the vector
	CBH LibAlgCBH;
};

#endif // __BASEPATH__
