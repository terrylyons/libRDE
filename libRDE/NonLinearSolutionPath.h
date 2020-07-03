/* *************************************************************

Copyright 2010 Terry Lyons, Stephen Buckley, Djalil Chafai, 
Greg Gyurkó and Arend Janssen. 

Distributed under the terms of the GNU General Public License, 
Version 3. (See accompanying file License.txt)

************************************************************* */



#pragma once

#ifndef __NONLINEARSOLUTIONPATH__
#define __NONLINEARSOLUTIONPATH__


#include "DynamicallyConstructedPath.h"
#include "libalgebra/alg_types.h"
#include "NonLinearGroupElement.h"


/// Gives the solution of the (non-linear) RDE as Path.

template <typename my_alg_type_IN, typename my_alg_type_OUT>
class NonLinearSolutionPath :
	public DynamicallyConstructedPath<my_alg_type_OUT>
{
private:
	/// The input path (e.g. Brownian path)
	Path<my_alg_type_IN> & theControl;

	/// The vector fields
	const std::vector<POLYLIE<my_alg_type_OUT>> theVectorFields;

	/// The start time
	const SCA Inf;

	/// The initial value
	const AbstractSolutionPoint<my_alg_type_OUT> Ini;

public:
	/// Constructor. Takes the input path, equation, start time and initial value.
	NonLinearSolutionPath(Path<my_alg_type_IN> & theControlIn, std::vector<POLYLIE<my_alg_type_OUT>> & theVectorFieldsIn, AbstractSolutionPoint<my_alg_type_OUT> & theInitialValue, SCA theStartTime = SCA(0))
		: theControl(theControlIn), theVectorFields(theVectorFieldsIn), Inf(theStartTime), Ini(theInitialValue)
	{
	};

	/// Default Destructor.
	~NonLinearSolutionPath(void)
	{
	};

	/// Implementation of ComputeChildLieIncrements.
	void ComputeChildLieIncrements( Increment<my_alg_type_OUT> & nvLeft, Increment<my_alg_type_OUT> & nvRight, ConstIterator itLeafAbove ) const
	{
		const dyadic_interval &  diAbove = itLeafAbove->first;
		const Increment<my_alg_type_OUT> & nvAbove = itLeafAbove->second;

		//split the interval diAbove into halves
		SCA midPoint = SCA(diAbove.inf() + ((diAbove.sup() - diAbove.inf())*0.5));
		LIE incLeft = ComputeIncrement( SCA(diAbove.inf()), midPoint );
		LIE incRight = nvAbove.LieValue() - incLeft;
		nvLeft.LieValue(incLeft);
		nvRight.LieValue(incRight);
	};

	/// Implementation of MakeRootLieIncrement.
	LIE MakeRootLieIncrement( const dyadic_interval &increment ) const
	{
		return ComputeIncrement( SCA(increment.inf()), SCA(increment.sup()) );
	};

	/// Implementation of MakeNeighborRootLieIncrement
	void MakeNeighborRootLieIncrement( LIE & ans, const Iterator & OldRoot ) const
	{
		//// Cannot assume that OldRoot is still the root
		dyadic_interval diIncrement = OldRoot->first;
		diIncrement.flip_interval();
		ans = ComputeIncrement( diIncrement.inf(), diIncrement.sup() );
	};

private:
	LIE ComputeIncrement(const SCA t0, const SCA t1, const int accuracy=11) const 
	{
		if (t0 == Inf)
		{
			//compute the solution
			AbstractSolutionPoint<my_alg_type_OUT> solution;
			solution = ComputeSolution(Inf, Ini, t1, accuracy);

			//increment over the time interval
			AbstractSolutionPoint<my_alg_type_OUT> inc;
			AbstractSolutionPoint<my_alg_type_OUT> temp(Ini);
			inc = solution - temp;

			LIE result;

			for(unsigned int i = 1; i <= my_alg_type_OUT::myDIM; i++)
			{
				result += LIE(LET(i),inc[LET(i)]);
			}	

			return result;
		}
		else
		{
			if (t0 > Inf)
			{
				//compute solution at time increment.inf first and then solve the RDE from there
	
				//solution at time increment.inf
				AbstractSolutionPoint<my_alg_type_OUT> a0;
				a0 = ComputeSolution(Inf, Ini, t0, accuracy);

				//solution at time increment.sup
				AbstractSolutionPoint<my_alg_type_OUT> solution;
				solution = ComputeSolution(t0, a0, t1, accuracy);

				//increment over the time interval
				AbstractSolutionPoint<my_alg_type_OUT> inc;
				inc = solution - a0;

				LIE result;

				for(unsigned int i = 1; i <= my_alg_type_OUT::myDIM; i++)
				{
					result += LIE(LET(i),inc[LET(i)]);
				}	

				return result;
			}
			else
			{
				//throw error and abort 
				std::cout << "Error (NonLinearSolutionPath): Time less than start time!" << std::endl;
				abort();
				return LIE();
			}
		}
	};

	/// Computes the solution of the RDE at time t1, starting from a0 at time t0.
	AbstractSolutionPoint<my_alg_type_OUT> ComputeSolution(SCA t0, const AbstractSolutionPoint<my_alg_type_OUT> & a0, SCA t1, const unsigned int my_accuracy) const
	{
		dyadic_interval Width(t1-t0, my_accuracy);
		dyadic_interval Begin = dyadic_interval::dyadic_bracket(t0,Width.n); 
		dyadic_interval End = dyadic_interval::dyadic_bracket(t1,Width.n);

		unsigned int NoIncrements = End.k - Begin.k;

		SPB::NonLinearGroupElement<my_alg_type_IN,my_alg_type_OUT> CurrentGroupElt;

		AbstractSolutionPoint<my_alg_type_OUT> CurrentValue;
		CurrentValue = a0;

		AbstractSolutionPoint<my_alg_type_OUT> temp;

		dyadic_interval CurrentInterval=Begin;

		lietovectorfield<my_alg_type_IN,POLYLIE_OUT> l2vf(theVectorFields);

		for(unsigned int i=1; i <= NoIncrements; i++)
		{
			CurrentGroupElt = SPB::NonLinearGroupElement<my_alg_type_IN,my_alg_type_OUT>(theControl.DescribePath(CurrentInterval++), l2vf);

			temp = CurrentGroupElt.evaluate(CurrentValue);
			CurrentValue = temp;
		}

		return CurrentValue;
	};

};

#endif // __NONLINEARSOLUTIONPATH__
