/* *************************************************************

Copyright 2010 Terry Lyons, Stephen Buckley, Djalil Chafai, 
Greg Gyurkó and Arend Janssen. 

Distributed under the terms of the GNU General Public License, 
Version 3. (See accompanying file License.txt)

************************************************************* */



#pragma once

#ifndef __LIEMATRIX__
#define __LIEMATRIX__


#include <mtl/matrix.h>
#include <mtl/mtl.h>
#include <mtl/utils.h>
#include <mtl/lu.h>
#include "GroupElement.h"
#include "libalgebra/libalgebra.h"


/// The LieMatrix. Used extensively in the implementation of Pade Approximation to solve linear ODEs.

/// Built on mtlMatrix.
template <typename my_matrix_alg_type>
class LieMatrix : public my_matrix_alg_type::mtlMatrix
{
	typedef typename my_matrix_alg_type::SCA SCA;
	typedef typename my_matrix_alg_type::RAT RAT;
	typedef typename my_matrix_alg_type::mtlVector mtlVector;
	typedef typename my_matrix_alg_type::mtlMatrix mtlMatrix;
	typedef typename my_matrix_alg_type::mtlDiagMat mtlDiagMat;

	static const unsigned myDIM = my_matrix_alg_type::myDIM;

public:
	/// Default constructor.
	LieMatrix() : mtlMatrix(myDIM,myDIM)
	{
		mtl::set_value((mtlMatrix &) *this, RAT(0));
	};

	/// Copy constructor.
	LieMatrix(const LieMatrix<my_matrix_alg_type> & m) : mtlMatrix(myDIM,myDIM)
	{
		mtl::copy(m,(mtlMatrix &)*this);
	};

	/// Default destructor.
	~LieMatrix(void) {};

	/// Adds to this
	LieMatrix<my_matrix_alg_type> & operator+=(const mtlMatrix & m)
	{
		mtl::add(m,(mtlMatrix &)*this);
		return *this;
	};

	/// Rescales this
	LieMatrix<my_matrix_alg_type> & operator *=(const SCA a)
	{
		mtl::scale((mtlMatrix &)*this,a);
		return *this;
	};

	/// Returns the sum of this and n
	LieMatrix<my_matrix_alg_type> operator+(const LieMatrix<my_matrix_alg_type> & n) const
	{
		return (LieMatrix<my_matrix_alg_type>(*this)+=n);
	};

	/// Returns the exponential GroupElement
	GroupElement<my_matrix_alg_type> exp(const SCA t = SCA(1)) const
	{
		SCA s = mtl::infinity_norm(*this) + mtl::one_norm(*this);
// TODO get rid of the numerical specific code here
		// use a template?
		// or fix on double 
		int mantissa;
		frexp( s*t, &mantissa );

		if (mantissa < 0) mantissa=0;
		mantissa=mantissa+1;

		SCA r1,r2;
		r1=ldexp(1.,mantissa);//power of two
		r2=ldexp(1.,-mantissa);//power of two

		LieMatrix<my_matrix_alg_type> z(*this);
		z*=(t*r2);
		LieMatrix<my_matrix_alg_type> z_squared;
		mtl::mult(z,z,z_squared);

		//use pade approx to compute exp(z) as a rational
		// exp(z)~numerator/denominator
		GroupElement<my_matrix_alg_type> numerator, denominator;
		numerator=pade_recurse(z,z_squared,4,top);
		denominator=pade_recurse(z,z_squared,4,bottom);

		// invert the denominator
		mtlMatrix temp(myDIM,myDIM),padeexp(myDIM,myDIM),denom(myDIM,myDIM);
		mtl::copy((mtlMatrix &) denominator,denom);
		mtl::set_value(temp,RAT(0));
		mtl::set_value(padeexp,RAT(0));
		mtl::dense1D<int> pvector(myDIM);
		mtl::lu_factor(denom, pvector);
		mtl::lu_inverse(denom, pvector, temp);
		mtl::mult((mtlMatrix &) numerator,temp,padeexp);
		GroupElement<my_matrix_alg_type> exp;
		mtl::set_diagonal((mtlMatrix &) exp,RAT(0));
		mtl::copy(padeexp,(mtlMatrix &) exp);
	
		// compute the 2^mantissa power of exp(z)
		while  (mantissa--) 
		{
			GroupElement<my_matrix_alg_type> tem;
			tem=exp*exp;
			mtl::swap((mtlMatrix &) exp,(mtlMatrix &) tem);
		};
		return exp;
	};

	static mtlMatrix& scaled_add ( mtlMatrix& B, SCA l, mtlMatrix & result)
	{
		mtl::add(mtl::scaled( B, l), result);
		return result;
	};

	/// Lie bracket of two LieMatrices (ie AB-BA).
	static LieMatrix<my_matrix_alg_type> Lie(const LieMatrix<my_matrix_alg_type> & A, const LieMatrix<my_matrix_alg_type> & B)
	{
		LieMatrix<my_matrix_alg_type> temp;
		mtl::mult((mtlMatrix &) A, (mtlMatrix &) B,(mtlMatrix &) temp);
		mtl::mult(mtl::scaled((mtlMatrix &) B,SCA(-1)), (mtlMatrix &) A, (mtlMatrix &) temp);
		return temp;
	};

private:
	enum flip{top=1,bottom=-1};

	/// computes a factor of the continued fraction
	GroupElement<my_matrix_alg_type> pade_recurse(const LieMatrix<my_matrix_alg_type> & zz,const LieMatrix<my_matrix_alg_type> & zz_squared, unsigned int c,flip position) const 
	{
		GroupElement<my_matrix_alg_type> flow0,flow1;
		mtl::add(mtl::scaled((mtlMatrix &)zz, SCA(position) * SCA(SCA(1)/SCA(2))) ,(mtlMatrix &) flow1);
		for (unsigned int count(1);count<c;count++)
		{
			GroupElement<my_matrix_alg_type> flow2;
			for(unsigned int i1(0); i1<myDIM; i1++)
				for(unsigned int i2(0); i2<myDIM; i2++)
				{
					flow2[i1][i2]=flow1[i1][i2];
				}
			mtl::mult(
				mtl::scaled(
				zz_squared
				,SCA(1)/(SCA(16)*SCA(count)*SCA(count)- SCA(4))
				)
				,(mtlMatrix &) flow0
				,(mtlMatrix &) flow2
				);
				
			for(unsigned int i1(0); i1<myDIM; i1++)
				for(unsigned int i2(0); i2<myDIM; i2++)
				{
					flow0[i1][i2]=flow1[i1][i2];
					flow1[i1][i2]=flow2[i1][i2];
				}		
		}
		return flow1;
	};

};// END CLASS DEFINITION LieMatrix

#endif // __LIEMATRIX__
