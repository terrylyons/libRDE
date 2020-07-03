/* *************************************************************

Copyright 2010 Terry Lyons, Stephen Buckley, Djalil Chafai, 
Greg Gyurkó and Arend Janssen. 

Distributed under the terms of the GNU General Public License, 
Version 3. (See accompanying file License.txt)

************************************************************* */



//  alg_types2.h : provides an interface to and sets consistent sets of basic algebraic types


#pragma once

#ifndef alg_types2_h__
#define alg_types2_h__

#include "mtl/mtl.h"
#include "libalgebra/alg_types.h"

namespace {
template <typename SCALAR>
struct matrix_types 
{
	typedef mtl::dense1D<SCALAR> mtlVector;
	typedef typename mtl::matrix<SCALAR, mtl::rectangle<>, mtl::dense<>, mtl::row_major>::type mtlMatrix;
	typedef typename mtl::matrix<SCALAR, mtl::diagonal<>, mtl::packed<>, mtl::row_major>::type mtlDiagMat;
};
}

template <size_t D, size_t W, coefficient_t F = Rational> 
struct matrix_alg_types : alg_types < D, W, F >,  matrix_types < typename alg_types < D, W, F >::SCA > {};

#endif // alg_types2_h__