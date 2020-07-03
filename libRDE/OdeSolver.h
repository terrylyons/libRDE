/* *************************************************************

Copyright 2010 Terry Lyons, Stephen Buckley, Djalil Chafai, 
Greg Gyurkó and Arend Janssen. 

Distributed under the terms of the GNU General Public License, 
Version 3. (See accompanying file License.txt)

************************************************************* */



#pragma once

#ifndef __ODESOLVER__
#define __ODESOLVER__


#include "NonLinearGroupElement.h"
#include "POLYLIE.h"
#include "libalgebra/libalgebra.h"


/// 5th-order ODE Solver 
	
/// Dormand-Prince method (a version of the Runge-Kutta-Fehlberg method RKF54)
/// with adaptive step size control.
/// Method as described in:
/// J. C. Butcher: Numerical methods for ordinary differential equations, 2nd ed., Wiley, 2008.
/// W. H. Press, S. A. Teukolsky, W. T. Vetterling and B. P. Flannery: Numerical recipes, 3rd ed., 
/// Cambridge University Press, 2007.
template <typename my_alg_type>
class OdeSolver
{
	typedef typename my_alg_type::SCA SCA;

	static const unsigned myDIM = my_alg_type::myDIM;

public:
	/// Constuctor. Argument sets absolute and relative error tolerance.
	OdeSolver(const SCA epsilon=SCA(0.001)) : atol(epsilon), rtol(epsilon), S(SCA(0.9)), maxscale(SCA(10.0)), minscale(SCA(0.2))
	{
	};

	/// Default Destructor.
	~OdeSolver(void)
	{
	};

	/// Exponentiates theEquationIn and solves for initial point InitialPoint at time one.
	AbstractSolutionPoint<my_alg_type> solve(AbstractSolutionPoint<my_alg_type> & InitialPoint, POLYLIE<my_alg_type> & theEquationIn)
	{
		SCA x;
		AbstractSolutionPoint<my_alg_type> yOLD;
		AbstractSolutionPoint<my_alg_type> yNEW;
		AbstractSolutionPoint<my_alg_type> yNEWstar;
		AbstractSolutionPoint<my_alg_type> k1;
		AbstractSolutionPoint<my_alg_type> k2;
		AbstractSolutionPoint<my_alg_type> k3;
		AbstractSolutionPoint<my_alg_type> k4;
		AbstractSolutionPoint<my_alg_type> k5;
		AbstractSolutionPoint<my_alg_type> k6;
		SCA err;
		SCA sk;
		SCA scale;
		SCA h;
		bool reject;
		int previousrejections;

		x = SCA(0);
		yOLD = InitialPoint;

		h = SCA(0.1);
		previousrejections = 0;
		reject = true;

		while(x < SCA(1))
		{ 
			while(reject)
			{
				k1 = h*theEquationIn.evaluate(yOLD);
				k2 = h*theEquationIn.evaluate(yOLD+SCA(2)/SCA(10)*k1);
				k3 = h*theEquationIn.evaluate(yOLD+SCA(3)/SCA(40)*k1+SCA(9)/SCA(40)*k2);
				k4 = h*theEquationIn.evaluate(yOLD+SCA(44)/SCA(45)*k1-SCA(56)/SCA(15)*k2+SCA(32)/SCA(9)*k3);
				k5 = h*theEquationIn.evaluate(yOLD+SCA(19372)/SCA(6561)*k1-SCA(25360)/SCA(2187)*k2+SCA(64448)/SCA(6561)*k3-SCA(212)/SCA(729)*k4);
				k6 = h*theEquationIn.evaluate(yOLD+SCA(9017)/SCA(3168)*k1-SCA(355)/SCA(33)*k2+SCA(46732)/SCA(5247)*k3+SCA(49)/SCA(176)*k4-SCA(5103)/SCA(18656)*k5);

				yNEW = yOLD + SCA(35)/SCA(384)*k1 + SCA(500)/SCA(1113)*k3 + SCA(125)/SCA(192)*k4 - SCA(2187)/SCA(6784)*k5 + SCA(11)/SCA(84)*k6;
				yNEWstar = yOLD + SCA(5179)/SCA(57600)*k1 + SCA(7571)/SCA(16695)*k3 + SCA(393)/SCA(640)*k4 - SCA(92097)/SCA(339200)*k5 + SCA(187)/SCA(2100)*k6;

				err = SCA(0);
	
				for(unsigned int i=1; i<=myDIM; i++)
				{
					sk = atol + rtol*maximum(absolutevalue(yNEW[LET(i)]), absolutevalue(yNEWstar[LET(i)]));
					err += ((yNEW[LET(i)]-yNEWstar[LET(i)])*(yNEW[LET(i)]-yNEWstar[LET(i)]))/(sk*sk);
				}
				err = sqrt(err/myDIM);

				if(err <= SCA(1)) //accept yNEW
				{
					reject = false;
				}
				else //make the step size smaller
				{
					scale = maximum(S*pow(err, -0.2), minscale);
					h *= scale;	
					previousrejections += 1;
				}
			}

			x += h;
			yOLD = yNEW;

			//compute new stepsize
			if(err == SCA(0))
			{
				scale = maxscale;
			}
			else
			{
				scale = S*pow(err, -0.2);
			
				if(scale < minscale)
				{
					scale = minscale;
				}

				if(scale > maxscale)
				{
					scale = maxscale;
				}
			}

			if(previousrejections > 0) //do not increase the step size
			{
				if(scale > SCA(1))
				{
					scale = SCA(1);
				}

				h *= scale;
			}
			else
			{
				h *= scale;
			}

			previousrejections = 0;

			if(x+h <= SCA(1))
			{
				reject = true;
			}
			else
			{
				h = SCA(1) - x;
			
				if(h > SCA(0))
				{
					reject = true;
				}
			}		
		}

		return yNEW;
	};

private:
	/// Absolute error tolerance.
	const SCA atol; 

	/// Relative error tolerance.
	const SCA rtol; 

	/// Safety factor
	const SCA S; 

	const SCA maxscale;

	const SCA minscale;

	SCA absolutevalue(SCA a)
	{
		if(a>=0)
		{
			return a;
		}
		else
		{
			return -a;
		}
	};

	SCA maximum(SCA a, SCA b)
	{
		if(a>=b)
		{
			return a;
		}
		else
		{
			return b;
		}
	};
};

#endif // __ODESOLVER__