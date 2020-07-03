/* *************************************************************

Copyright 2010 Terry Lyons, Stephen Buckley, Djalil Chafai, 
Greg Gyurkó and Arend Janssen. 

Distributed under the terms of the GNU General Public License, 
Version 3. (See accompanying file License.txt)

************************************************************* */



#pragma once

#ifndef __NONLINEARSOLUTIONTRAJECTORY__
#define __NONLINEARSOLUTIONTRAJECTORY__


#include "BaseTrajectory.h"
#include "libalgebra/alg_types.h"
#include "NonLinearGroupElement.h"


/// Computes the value of the solution of the (non-linear) RDE.

template <typename my_alg_type_IN, typename my_alg_type_OUT>
class NonLinearSolutionTrajectory :
	public BaseTrajectory<my_alg_type_OUT>
{
private:
	/// The input path (e.g. Brownian path)
	Path<my_alg_type_IN> & theControl;

	/// The vector fields
	const std::vector<POLYLIE<my_alg_type_OUT>> theVectorFields;

	/// The start time
	const SCA Inf;

	/// The initial value
	const AbstractSolutionPoint Ini;

public:
	/// Constructor. Takes the input path, equation, start time and initial value.
	NonLinearSolutionTrajectory(Path<my_alg_type_IN> & theControlIn, std::vector<POLYLIE<my_alg_type_OUT>> & theVectorFieldsIn, AbstractSolutionPoint & theInitialValue, SCA theStartTime = SCA(0))
		: theControl(theControlIn), theVectorFields(theVectorFieldsIn), Inf(theStartTime), Ini(theInitialValue)
	{
	};

	/// Default Destructor.
	~NonLinearSolutionTrajectory(void)
	{
	};

	/// Value of the solution at time t. The number of points on the dataset is 
	/// always between 2^(Resolution -1) and 2^Resolution. 
	AbstractSolutionPoint Value(const SCA t, const unsigned int Resolution=11) const
	{
		dyadic_interval Width(t-Inf, Resolution);
		dyadic_interval Begin = dyadic_interval::dyadic_bracket(Inf,Width.n); 
		dyadic_interval End = dyadic_interval::dyadic_bracket(t,Width.n);

		unsigned int NoIncrements = End.k - Begin.k;

		SPB::NonLinearGroupElement<my_alg_type_IN,my_alg_type_OUT> CurrentGroupElt;

		AbstractSolutionPoint CurrentValue;
		CurrentValue = Ini;

		AbstractSolutionPoint temp;

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

#endif // __NONLINEARSOLUTIONTRAJECTORY__
