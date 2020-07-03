/* *************************************************************

Copyright 2010 Terry Lyons, Stephen Buckley, Djalil Chafai, 
Greg Gyurkó and Arend Janssen. 

Distributed under the terms of the GNU General Public License, 
Version 3. (See accompanying file License.txt)

************************************************************* */



#pragma once

#ifndef __ABSTRACTSOLUTIONPOINT__
#define __ABSTRACTSOLUTIONPOINT__


#include "lietovectorfield.h"


/// A point in solution space 

/// An AbstractSolutionPoint is of the form map<LET, SCA>, where each 
/// scalar correspond to the coefficient of the unit vectors in the 
/// direction given by the corresponding LET.
template <typename my_alg_type>
class AbstractSolutionPoint : public std::map<typename my_alg_type::LET, typename my_alg_type::SCA>
{
	typedef typename my_alg_type::SCA SCA;
	typedef typename my_alg_type::LET LET;
	typedef typename my_alg_type::mtlVector mtlVector;

	static const unsigned myDIM = my_alg_type::myDIM;

public:
	/// Default Constructor, empty map.
	AbstractSolutionPoint(void){};
	/// Default Destructor.
	~AbstractSolutionPoint(void){};

	/// Constructor that converts an mtlVector to an AbstractSolution point.
	explicit AbstractSolutionPoint(mtlVector& v)
	{
		for(unsigned int i=0; i < myDIM; i++)
		{
			if ( v[i] != 0 )
			{
				(*this)[LET(i+1)] = v[i];
			}
		}
	};

	AbstractSolutionPoint<my_alg_type> operator + (AbstractSolutionPoint<my_alg_type> & p2)
	{
		AbstractSolutionPoint<my_alg_type> result;

		std::map<LET, SCA>::iterator i=(*this).begin();

		while(i != (*this).end())
		{
			if (p2.find((*i).first) != p2.end() )
			{
				result[(*i).first] = (*this)[(*i).first] + p2[(*i).first];
			}
			else
			{
				result[(*i).first] = (*this)[(*i).first]; 
			}

			i++;
		}

		i=p2.begin();

		while(i != p2.end())
		{
			if (result.find((*i).first) == result.end())
			{
				result[(*i).first] = p2[(*i).first];
			}

			i++;
		}

		return result;
	};

	AbstractSolutionPoint<my_alg_type> operator - (AbstractSolutionPoint<my_alg_type> & p2)
	{
		return (-1)*p2 + (*this);
	};

	friend AbstractSolutionPoint<my_alg_type> operator * (SCA s, AbstractSolutionPoint<my_alg_type> & p)
	{
		AbstractSolutionPoint<my_alg_type> result;

		std::map<LET, SCA>::iterator i=p.begin();

		while(i != p.end())
		{
			result[(*i).first] = (*i).second * s;
			i++;
		}

		return result;
	};


}; //END CLASS DEFINITION AbstractSolutionPoint


#endif // __ABSTRACTSOLUTIONPOINT__
