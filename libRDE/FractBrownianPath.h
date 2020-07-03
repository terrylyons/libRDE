/* *************************************************************

Copyright 2010 Terry Lyons, Stephen Buckley, Djalil Chafai, 
Greg Gyurkó, Arend Janssen and Rahul Raghuram. 

Distributed under the terms of the GNU General Public License, 
Version 3. (See accompanying file License.txt)

************************************************************* */



#pragma once

#ifndef __FRACTBROWNIANPATH__
#define __FRACTBROWNIANPATH__

// Check windows
#include "lapack_defns.h"

#include "DynamicallyConstructedPath.h"
#include "NormalRandomNumberGenerator.h"
#include <algorithm>
#include <vector>
#include <cmath>
#include <math.h>

#define MKL
#ifdef MKL

#define daxpy_ DAXPY
#define ddot_ DDOT
#define dgelsd_ DGELSD 
#define dgelss_ DGELSS 
#define dnrm2_ DNRM2
#define dgesv_ DGESV 
#define dcopy_ DCOPY
#define dgemm_ DGEMM
#define dtptrs_ DTPTRS
#endif


/*
This demonstrates how data in 'packed form', which is what we use:
if we define:
double array1[6] = {1,2,3,4,5,6};
and we specify the size of the matrix (in this case 3) then this is interpreted as:
a matrix A such that: a(1,1)=1, a(2,1)=2, a(2,2)=3, a(3,1)=4, a(3,2)=5, a(3,3)=6
i.e. A =	{	1	0	0	}
			{	2	3	0	}
			{	4	5	6	}
*/

extern "C" double dtptrs_(char *UPLO, 
						char *tran,
						char *diag,
						integer *order,
						integer *nrhs,
						double *matrovectro,	//input - put an "&" before this
						double *vectro,			//output - don't put an "&" before this, alternatively use pointers 
                        integer *order3,
	                    integer  *info);

//2-dim. fractional Brownian path
template <typename my_alg_type>
class FractBrownianPath :
	public DynamicallyConstructedPath<my_alg_type>
{
	typedef std::vector<SCA> MYVEC;
	typedef std::vector<LIE> MYVECLIE;
	typedef double doublereal;
	typedef integer int_t;
	typedef int_t integer;


	const double Hurst; //the Hurst coefficient

	static NormalRandomNumberGenerator vgCommonNormal;

	virtual LIE GaussianIncrement( double t, int_t n ) const
	{
		LIE coord(n,S(vgNormal(t)));
		return coord;
	}

protected:
	NormalRandomNumberGenerator & vgNormal;

public:	
	FractBrownianPath(const double H=0.8) : vgNormal( FractBrownianPath::vgCommonNormal ), Hurst(H) {};

	FractBrownianPath(NormalRandomNumberGenerator & rand, const double H=0.8) : vgNormal( rand ), Hurst(H) {};

	~FractBrownianPath(void){};

	mutable MYVECLIE vectro; //a vector of LIE getting the gaussian values

	mutable MYVECLIE lievalues; //vector of LIE getting the LIE values at specified points

	mutable MYVECLIE vectro2; //a vector of LIE getting the gaussian values

	mutable MYVECLIE lievalues2; //vector of LIE getting the LIE values at specified points

	mutable MYVEC matrovectro; // matrix to input

	mutable MYVEC themidpoints; //vector holding midpoints

	mutable double newleftmidpoint;

	mutable double newrightmidpoint;

	void IncreaseVectors( const double &midpoint) const
		{
		char UPLO('U');
		char tran('T');
		char diag('N');
		int_t nrhs = 1; 
		int_t info = 0; 
		int_t sizeofvectro = vectro.size();
		int_t order = sizeofvectro;
		int_t order2 = sizeofvectro;
		themidpoints.push_back(midpoint); //puts "midpoint" into the vector of midpoints
		MYVEC ltemp; //the 'xi' vector
		for (int_t i = 0; i < order; i++) //give the covariance vector for the midpoint
		{
			double tempcoeff = .5 *
								( pow(themidpoints[i],2 * Hurst) + 
									pow(midpoint,2 * Hurst) -
									pow( fabs( (themidpoints[i]-midpoint) ) , 2 * Hurst)) ;		
			ltemp.push_back(tempcoeff);
		}
		double *pntrvec;
		pntrvec = &ltemp[0]; //"pntrvec" a pointer to first element of "ltemp"
		dtptrs_(		&UPLO,
						&tran, 
						&diag,
						&order,
						&nrhs,
						&matrovectro[0], //with an '&' we pass a reference, without, we pass the actual matrix/vector
						pntrvec, //passing a pointer to first element of "vectro"
						&order2,
						&info );
		double squaredsum(0);
		for (int_t i=0; i< order; i++)
		{
			squaredsum += pow( ltemp[i], 2 );
			matrovectro.push_back(ltemp[i]);
		}
		double thebeta = pow ( fabs( pow(midpoint, 2*Hurst) - squaredsum ) , .5 );
		ltemp.push_back(thebeta);
		matrovectro.push_back(thebeta);
		LIE temp = GaussianIncrement(1,1);
		LIE temp2 = GaussianIncrement(1,2);
		vectro.push_back(temp);
		LIE tobeaddedtolievalues(vectro[0] * ltemp[0]);
		for (int_t i=1; i< order + 1; i++) //starts from 1 because we have initialised it with the first iteration
		{
			tobeaddedtolievalues += ( vectro[i] * ltemp[i] );
		}
		lievalues.push_back(tobeaddedtolievalues);
		vectro2.push_back(temp2);
		LIE tobeaddedtolievalues2(vectro2[0] * ltemp[0]);
		for (int_t i=1; i< order + 1; i++) //starts from 1 because we have initialised it with the first iteration
		{
			tobeaddedtolievalues2 += ( vectro2[i] * ltemp[i] );
		}
		lievalues2.push_back(tobeaddedtolievalues2);
	};

	LIE FindExistingLieValue(const double &point, const int_t n) const
	{
		//takes the double "point", then finds the corresponding value in "lievalues" and returns it
		int_t findplace(-2);
		int_t size(themidpoints.size());
		for (int_t i=0; i< size; i++) if (themidpoints[i] == point) findplace = i;
		LIE tobereturned;
		if (n==1) tobereturned=lievalues[findplace];
		else
		{
			tobereturned=lievalues2[findplace];
		}
		if (findplace==-2) std::cout<< "Oh dear, FindExistingLieValue failed";
		return tobereturned;
	};

	void ComputeChildLieIncrements( Increment<my_alg_type> & nvLeft, Increment<my_alg_type> & nvRight, ConstIterator itLeafAbove ) const
	{
		const dyadic_interval &  diAbove = itLeafAbove->first;
		const Increment<my_alg_type> & nvAbove = itLeafAbove->second;
		newleftmidpoint = ((diAbove.sup() + diAbove.inf()) *.5); //calculates the midpoint of the interval
		FractBrownianPath::IncreaseVectors(newleftmidpoint); //increases "midpoints" and "matrovectro"
		newleftmidpoint = diAbove.inf(); //note the second usage of the same variable, efficient (if bad) practice
		LIE latest = lievalues[lievalues.size() -1]; //latest lie value to be created
		LIE latest2 = lievalues2[lievalues2.size() -1]; //latest lie value to be created
		if (newleftmidpoint == 0) nvLeft.LieValue(latest +latest2); //"FindExistingLieValue" will fail if 0 is passed into it
		else
		{
		LIE leftlievalue = FractBrownianPath::FindExistingLieValue(newleftmidpoint,1);
		LIE leftlievalue2 = FractBrownianPath::FindExistingLieValue(newleftmidpoint,2);
		nvLeft.LieValue( latest - leftlievalue + latest2 - leftlievalue2); //sets the increment for the left-side child lie increment
		}
		newrightmidpoint = diAbove.sup();
		LIE rightlievalue = FractBrownianPath::FindExistingLieValue(newrightmidpoint,1);
		LIE rightlievalue2 = FractBrownianPath::FindExistingLieValue(newrightmidpoint,2);
		nvRight.LieValue( rightlievalue - latest + rightlievalue2 - latest2); //sets the increment for the right-side child lie increment
	};

	LIE MakeRootLieIncrement( const dyadic_interval &increment ) const
	{
		//At this point the path is nowhere defined. This should only be called once in each simulation
		assert(
			vectro.empty() //returns true if size is 0
				);
		assert(
			increment.inf() == 0  //returns true if the increment starts at 0
				);
		newrightmidpoint = increment.sup(); //the top of the increment
		newleftmidpoint = (increment.sup()* 0.5) + (increment.inf()*.5); //value of the midpoint of the interval, I'm using "newleftmidpoint" to save having an extra variable, this is to be added to midpoints vector
		double coefficient = pow(newrightmidpoint, Hurst); //really it is "2*H" rather than "H" and then this is square-rooted - am saving the calculations
		LIE tobeaddedtovectro = GaussianIncrement(1,1);  //this is the value at the rightpoint, so is to be added to "vectro" and is also the increment over the time period
		LIE tobeaddedtovectro2 = GaussianIncrement(1,2);  //this is the value at the rightpoint, so is to be added to "vectro" and is also the increment over the time period
		LIE tobeaddedtolievalues = tobeaddedtovectro * coefficient; // This should be the increment over the interval and also the value at the rightpoint of the interval.
		LIE tobeaddedtolievalues2 = tobeaddedtovectro2 * coefficient; // This should be the increment over the interval and also the value at the rightpoint of the interval.
		vectro.push_back(tobeaddedtovectro);
		vectro2.push_back(tobeaddedtovectro2);
		themidpoints.push_back(newrightmidpoint);
		matrovectro.push_back(coefficient); //adding a(1,1) = coefficient to the matrix
		lievalues.push_back(tobeaddedtolievalues);
		lievalues2.push_back(tobeaddedtolievalues2);
		return (tobeaddedtolievalues + tobeaddedtolievalues2); //increment has variance t^(2H)
	};

	void MakeNeighborRootLieIncrement( LIE & ans, const Iterator & OldRoot ) const
	{
		//At this point, the path is undefined on the flipped section. Also, have already specified value at left point but not at right point. 
		// Cannot assume that OldRoot is still the root
		dyadic_interval diIncrement = OldRoot->first;
		diIncrement.flip_interval();
		newrightmidpoint = diIncrement.sup();
		IncreaseVectors(newrightmidpoint); //the LIE added to "lievalues" is the lie value at the right end of the interval
		newleftmidpoint = diIncrement.inf();
		LIE latest = lievalues[lievalues.size() -1];
		LIE latest2 = lievalues2[lievalues2.size() -1];
		LIE leftlievalue = FractBrownianPath::FindExistingLieValue(newleftmidpoint,1);
		LIE leftlievalue2 = FractBrownianPath::FindExistingLieValue(newleftmidpoint,2);
		ans = ( latest - leftlievalue + latest2 - leftlievalue2); //this gives the increment over the interval
	};
};

#endif // __FRACTBROWNIANPATH__
