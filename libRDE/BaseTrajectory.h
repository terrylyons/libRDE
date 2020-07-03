/* *************************************************************

Copyright 2010 Terry Lyons, Stephen Buckley, Djalil Chafai, 
Greg Gyurkó and Arend Janssen. 

Distributed under the terms of the GNU General Public License, 
Version 3. (See accompanying file License.txt)

************************************************************* */



#pragma once

#ifndef __BASETRAJECTORY__
#define __BASETRAJECTORY__


#include "AbstractSolutionPoint.h"


/// A common ancestor for all trajectories.

/// BaseTrajectory contains the function that returns the value of a path as an AbstractSolutionPoint
/// for any given point of time. This is a virtual function, allowing us to define it 
/// later for specific trajectories.
template <typename my_alg_type>
class BaseTrajectory
{
protected:
	typedef typename my_alg_type::SCA SCA;
	typedef AbstractSolutionPoint<my_alg_type> AbstractSolutionPoint;

public:
	BaseTrajectory(void){};

	virtual ~BaseTrajectory(void){};

	/// The value of the path
	AbstractSolutionPoint virtual Value(const SCA t, const unsigned int Resolution=11) const
	{
		return AbstractSolutionPoint();
	};
};

#endif // __BASETRAJECTORY__

