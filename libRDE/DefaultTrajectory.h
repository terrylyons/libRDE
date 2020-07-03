/* *************************************************************

Copyright 2010 Terry Lyons, Stephen Buckley, Djalil Chafai, 
Greg Gyurkó and Arend Janssen. 

Distributed under the terms of the GNU General Public License, 
Version 3. (See accompanying file License.txt)

************************************************************* */



#pragma once

#ifndef __DEFAULTTRAJECTORY__
#define __DEFAULTTRAJECTORY__


#include "BaseTrajectory.h"


/// DefaultTrajectory
template <typename my_alg_type>
class DefaultTrajectory :
	public BaseTrajectory<my_alg_type>
{
public:
	DefaultTrajectory(void)
	{
	};

	~DefaultTrajectory(void)
	{
	};

	/// value of the path at a point of time
	AbstractSolutionPoint Value(const SCA t, const unsigned int Resolution=11) const
	{
		return AbstractSolutionPoint();
	};
};

#endif // __DEFAULTTRAJECTORY__
