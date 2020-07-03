/* *************************************************************

Copyright 2010 Terry Lyons, Stephen Buckley, Djalil Chafai, 
Greg Gyurkó and Arend Janssen. 

Distributed under the terms of the GNU General Public License, 
Version 3. (See accompanying file License.txt)

************************************************************* */



#pragma once

#ifndef Trajectory_h__
#define Trajectory_h__


#include "BaseTrajectory.h"
#include "DefaultTrajectory.h"

#include <memory>
//#include <functional>


/// The interface class for trajectories.

template <typename my_alg_type>
class Trajectory : 
	public BaseTrajectory<my_alg_type>
{
public:
	/// Trivial constructor.
	Trajectory(void) : pImpl(std::tr1::shared_ptr< const DefaultTrajectory<my_alg_type> > (new DefaultTrajectory<my_alg_type>))
	{
	}; 

	/// Constructor from pointer to implementation.
	Trajectory(std::tr1::shared_ptr < const BaseTrajectory<my_alg_type> > pimpl1) : pImpl(pimpl1)
	{
	};

	/// Called by virtual destructor in base class.
	~Trajectory(void)
	{
	};

	/// Value of the path at a point of time.
	AbstractSolutionPoint Value(const SCA t, const unsigned int Resolution=11) const
	{
		return pImpl->Value(t, Resolution);
	};

private:
	/// The pointer to the implementation.
	std::tr1::shared_ptr < const BaseTrajectory<my_alg_type> > pImpl;

};

#endif // Trajectory_h__

