/* *************************************************************

Copyright 2010 Terry Lyons, Stephen Buckley, Djalil Chafai, 
Greg Gyurkó and Arend Janssen. 

Distributed under the terms of the GNU General Public License, 
Version 3. (See accompanying file License.txt)

************************************************************* */



#pragma once

#ifndef MakeNonlinearSolutionPath_h__
#define MakeNonlinearSolutionPath_h__


#include "NonlinearSolutionPath.h"
#include "Path.h"
#include <memory>
#include <functional>

template <typename my_alg_type_IN, typename my_alg_type_OUT>
Path<my_alg_type_OUT> MakeNonLinearSolutionPath(Path<my_alg_type_IN> & theControlIn, std::vector<POLYLIE<my_alg_type_OUT>> & theVectorFieldsIn, AbstractSolutionPoint<my_alg_type_OUT> & theInitialValue, typename my_alg_type_OUT::SCA theStartTime = my_alg_type_OUT::SCA(0))
{
	std::tr1::shared_ptr< const NonLinearSolutionPath<my_alg_type_IN,my_alg_type_OUT> > p(new NonLinearSolutionPath<my_alg_type_IN,my_alg_type_OUT>(theControlIn, theVectorFieldsIn, theInitialValue, theStartTime));
	return Path<my_alg_type_OUT>(p);
};

#endif // MakeNonlinearSolutionPath_h__