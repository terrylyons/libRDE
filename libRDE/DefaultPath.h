/* *************************************************************

Copyright 2010 Terry Lyons, Stephen Buckley, Djalil Chafai, 
Greg Gyurkó and Arend Janssen. 

Distributed under the terms of the GNU General Public License, 
Version 3. (See accompanying file License.txt)

************************************************************* */



#pragma once

#ifndef __DEFAULTPATH__
#define __DEFAULTPATH__


#include "BasePath.h"


/// DefaultPath
template <typename my_alg_type>
class DefaultPath :
	public BasePath<my_alg_type>
{
public:

	typedef typename my_alg_type::LIE LIE;

	DefaultPath(void)
	{
	};

	~DefaultPath(void)
	{
	};

	/// describe the path over a dyadic interval
	LIE DescribePath(const dyadic_interval & increment, const int accuracy) const
	{
		return LIE();
	};

	/// describe the path over a real interval
	LIE DescribePath(double inf, double sup, int tolerance) const
	{
		return LIE();
	};
};

#endif // __DEFAULTPATH__
