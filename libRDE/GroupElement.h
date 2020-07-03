/* *************************************************************

Copyright 2010 Terry Lyons, Stephen Buckley, Djalil Chafai, 
Greg Gyurkó and Arend Janssen. 

Distributed under the terms of the GNU General Public License, 
Version 3. (See accompanying file License.txt)

************************************************************* */



#pragma once

#ifndef __GROUPELEMENT__
#define __GROUPELEMENT__


#include "AbstractSolutionPoint.h"


/// An ActionPoint is a point in solution space, analagously to AbstractSolutionPoint.

/// An ActionPoint is an mtlVector. The point in solution space is given by sum{a_i*e_i}
/// where a_i is the scalar in the ith entry of the ActionPoint and e_i is the ith unit 
/// coordinate vector.
template <typename my_alg_type>
class ActionPoint : public my_alg_type::mtlVector
{
	typedef typename my_alg_type::LET LET;
	typedef typename my_alg_type::mtlVector mtlVector;

	static const unsigned myDIM = my_alg_type::myDIM;

public:
	/// Constructor from mtlVector
	ActionPoint(mtlVector & M) : mtlVector(M){};

	template <typename T>
	ActionPoint(T M) : mtlVector((mtlVector)M){};

	ActionPoint() : mtlVector(myDIM){};

	~ActionPoint() {};

	/// Construct an ActionPoint from an AbstractSolutionPoint.
	explicit ActionPoint (AbstractSolutionPoint<my_alg_type> & p) : mtlVector(myDIM)
	{
		for(unsigned int i=0; i < myDIM; i++)
		{
			if (p.find(LET(i+1)) != p.end())
			{
				(*this)[i] = p[LET(i+1)];
			}
			else
			{
				(*this)[i] = 0;
			}
		}
	};

	/// Transform this ActionPoint into an AbstractSolutionPoint.
	AbstractSolutionPoint<my_alg_type> ActionPoint2AbstractSolutionPoint(void) 
	{
		AbstractSolutionPoint<my_alg_type> result;

		for(unsigned int i=0; i < myDIM; i++)
		{
			if ( (*this)[i] != 0 )
			{
				result[LET(i+1)] = (*this)[i];
			}
		}

		return result;
	};

	/// Transform an AbstractSolutionPoint into an ActionPoint
	static ActionPoint<my_alg_type> AbstractSolutionPoint2ActionPoint(AbstractSolutionPoint<my_alg_type> & p)
	{
		ActionPoint<my_alg_type> a;

		for(unsigned int i=0; i < myDIM; i++)
		{
			if (p.find(LET(i+1)) != p.end())
			{
				a[i] = p[LET(i+1)];
			}
			else
			{
				a[i] = 0;
			}
		}

		return a;
	};

};// END CLASS DEFINITION ActionPoint


/// A GroupElement transforms ActionPoints to ActionPoints.

/// Based on mtlMatrix.
template <typename my_alg_type>
class GroupElement : public  my_alg_type::mtlMatrix
{
	typedef typename my_alg_type::RAT RAT;
	typedef typename my_alg_type::mtlMatrix mtlMatrix;

	static const unsigned myDIM = my_alg_type::myDIM;

public:
	///Constructor from an mtlMatrix.
	GroupElement(mtlMatrix & M) : mtlMatrix(M) {};

	/// Default Constructor. Produces the identity matrix.
	GroupElement() : mtlMatrix(myDIM,myDIM)
	{
		mtl::set_diagonal((mtlMatrix &)*this,RAT(1));
	};

	/// Default Destructor.
	~GroupElement() {};

	/// Multiplication of matrices.
	GroupElement<my_alg_type> operator* (const GroupElement<my_alg_type> & g) const
	{
	GroupElement<my_alg_type> temp;
	mtl::set_diagonal((mtlMatrix) temp,RAT(0));
	mtl::mult((mtlMatrix &) g,(mtlMatrix &) *this  ,(mtlMatrix &) temp);
	return temp;
	};

	/// The group element transforms ActionPoints with this function
	ActionPoint<my_alg_type> operator()(const ActionPoint<my_alg_type> & Arg)
	{
	ActionPoint<my_alg_type> temp;
	mtl::set_value(temp,RAT(0));
	mtlMatrix trans;
	mtl::mult((mtlMatrix &) *this,Arg,temp);
	return ActionPoint<my_alg_type>(temp);
	};

};// END CLASS DEFINITION GroupElement


#endif // __GROUPELEMENT__
