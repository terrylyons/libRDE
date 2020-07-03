/* *************************************************************

Copyright 2010 Terry Lyons, Stephen Buckley, Djalil Chafai, 
Greg Gyurkó and Arend Janssen. 

Distributed under the terms of the GNU General Public License, 
Version 3. (See accompanying file License.txt)

************************************************************* */



#pragma once

#ifndef MakeNonlinearSolutionTrajectory_h__
#define MakeNonlinearSolutionTrajectory_h__


#include "NonlinearSolutionTrajectory.h"
#include "Trajectory.h"
#include <memory>
#include <functional>

template <typename my_alg_type_IN, typename my_alg_type_OUT>
Trajectory<my_alg_type_OUT> MakeNonLinearSolutionTrajectory(Path<my_alg_type_IN> & theControlIn, std::vector<POLYLIE<my_alg_type_OUT>> & theVectorFieldsIn, AbstractSolutionPoint<my_alg_type_OUT> & theInitialValue, typename my_alg_type_OUT::SCA theStartTime = my_alg_type_OUT::SCA(0))
{
	std::tr1::shared_ptr< const NonLinearSolutionTrajectory<my_alg_type_IN,my_alg_type_OUT> > p(new NonLinearSolutionTrajectory<my_alg_type_IN,my_alg_type_OUT>(theControlIn, theVectorFieldsIn, theInitialValue, theStartTime));
	return Trajectory<my_alg_type_OUT>(p);
};

#endif // MakeNonlinearSolutionTrajectory_h__