/* *************************************************************

Copyright 2010 Terry Lyons, Stephen Buckley, Djalil Chafai, 
Greg Gyurkó and Arend Janssen. 

Distributed under the terms of the GNU General Public License, 
Version 3. (See accompanying file License.txt)

************************************************************* */



#pragma once

#ifndef __NONLINEARGROUPELEMENT__
#define __NONLINEARGROUPELEMENT__


#include <map>
#include <vector>
#include "POLYLIE.h"
#include "lietovectorfield.h"
#include "AbstractSolutionPoint.h"


namespace SPB
{
	/// Solves the RDE over an interval for a given initial point

	/// For a given LIE element (the description of the input path over a dyadic_interval), we combine
	/// the LIE element with the equation we wish to solve, giving us a vector field to exponentiate. 
	/// The function evaluate, then allows us to do this for a given initial point.
	template <typename my_alg_type_IN, typename my_alg_type_OUT>
	class NonLinearGroupElement
	{
		typedef typename my_alg_type_IN::LIE LIE_IN;

	public:
		/// Default Constructor
		NonLinearGroupElement(void)
		{
		};

		/// Constructor that takes a Lie element and a lietovectorfield object (this contains
		/// the equation) and combines the two to obtain the ODE we want to solve: equation.
		NonLinearGroupElement(const LIE_IN & liearg, lietovectorfield< my_alg_type_IN, POLYLIE<my_alg_type_OUT> > & lie2vf)
		{
			equation = lie2vf.lie2vectorfield(liearg);
		}; 

		/// Default Destructor.
		~NonLinearGroupElement(void)
		{
		};

		/// Takes an initial point for the path and outputs the end point.

		/// This corresponds to exponentiating equation with initial point InitialPoint and
		/// solving at time one.
		AbstractSolutionPoint<my_alg_type_OUT> evaluate(AbstractSolutionPoint<my_alg_type_OUT> & InitialPoint)
		{
			OdeSolver<my_alg_type_OUT> temp;
			return temp.solve(InitialPoint, equation);
		};

	private:
		/// The ODE we want to solve.
		POLYLIE<my_alg_type_OUT> equation;
	};
}
#endif // __NONLINEARGROUPELEMENT__