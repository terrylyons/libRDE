/* *************************************************************

Copyright 2010 Terry Lyons, Stephen Buckley, Djalil Chafai, 
Greg Gyurkó and Arend Janssen. 

Distributed under the terms of the GNU General Public License, 
Version 3. (See accompanying file License.txt)

************************************************************* */



#pragma once

#ifndef __Shell__
#define __Shell__


#include "alg_types2.h"
#include "AbstractSolutionPoint.h"
#include "DataTree.h"
#include "BrownianPath.h"
#include "GroupElement.h"
#include "LieMatrix.h"
#include "POLYLIE.h"
#include "NonLinearGroupElement.h"
#include "OdeSolver.h"
#include "Path.h"
#include "SolutionPath.h"
#include "FractBrownianPath.h"


template <typename my_alg_type>
struct Shell
{
	typedef AbstractSolutionPoint<my_alg_type> AbstractSolutionPoint;
	typedef BrownianPath<my_alg_type> BrownianPath;
	typedef ActionPoint<my_alg_type> ActionPoint;
	typedef GroupElement<my_alg_type> GroupElement;
	typedef LieMatrix<my_alg_type> LieMatrix;
	typedef POLYLIE<my_alg_type> POLYLIE;
	typedef OdeSolver<my_alg_type> OdeSolver;
	typedef Path<my_alg_type> Path;
	typedef FractBrownianPath<my_alg_type> FractBrownianPath;
};

#endif // __Shell__