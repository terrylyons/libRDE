/* *************************************************************

Copyright 2010 Terry Lyons, Stephen Buckley, Djalil Chafai, 
Greg Gyurkó and Arend Janssen. 

Distributed under the terms of the GNU General Public License, 
Version 3. (See accompanying file License.txt)

************************************************************* */



#pragma once

#ifndef Path_h__
#define Path_h__


#include "BasePath.h"
#include "DefaultPath.h"

#include <memory>
//#include <functional>


/// The interface class through which all rough paths are used.

/// It only allows const access to the path.
template <typename my_alg_type>
class Path : 
	public BasePath<my_alg_type>
{
public:
	
	typedef typename my_alg_type::LIE LIE;
	
	/// Trivial constructor.
	Path(void) : pImpl(std::shared_ptr< const DefaultPath<my_alg_type> > (new DefaultPath<my_alg_type>))
	{
	}; 

	/// Constructor from pointer to implementation.
	Path(std::shared_ptr < const BasePath<my_alg_type> > pimpl1) : pImpl(pimpl1)
	{
	};

	/// Called by virtual destructor in base class.
	~Path(void)
	{
	};

	/// Describe the path over a dyadic interval.
	LIE DescribePath(const dyadic_interval & di, const int accuracy) const
	{
		return pImpl->DescribePath(di,accuracy);
	};
	
	/// Describe the path over a dyadic interval using the default accuracy deduced from the interval.
	LIE DescribePath(const dyadic_interval & di) const 
	{ 
		return DescribePath(di, di.n);
	};

	/// Describe the path over an arbitrary interval resolved to dyadics at a level coarser, or equal, to 2^(-tolerance)
	LIE DescribePath(double inf, double sup, int tolerance) const
	{
		return pImpl->DescribePath(inf, sup, tolerance);
	};

	Path RestrictPath(const interval& interval) const
	{
		return Path(new RestrictedPath(pImpl, interval));	
	}

	Path Concatenate(const Path& other) const
	{
		return Path(new ConcatenatedPath(pImpl, other.pImpl));
	}

private:
	/// The pointer to the implementation.
	std::shared_ptr < const BasePath<my_alg_type> > pImpl;

}; //END CLASS DEFINITION Path

#endif // Path_h__

