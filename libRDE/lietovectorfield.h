/* *************************************************************

Copyright 2010 Terry Lyons, Stephen Buckley, Djalil Chafai, 
Greg Gyurkó and Arend Janssen. 

Distributed under the terms of the GNU General Public License, 
Version 3. (See accompanying file License.txt)

************************************************************* */



#pragma once

#ifndef __lie2vectorfield__
#define __lie2vectorfield__


#include <math.h>
#include "alg_types2.h"


/// Holds the equation, vecargument, and a function to combine the equation with a Lie element.

/// VF is a vector field - a matrix in the linear case or a POLYLIE in the polynomial case. 
/// VF needs a scaled_add function defined in its class; a combination of addition and scalar
/// multiplication.
template <typename my_alg_type, typename VF>
class lietovectorfield
{
	typedef typename my_alg_type::LIE LIE;
	typedef typename my_alg_type::S S;
	typedef typename my_alg_type::Q Q;

	static const unsigned DEPTH = my_alg_type::DEPTH;
	static const unsigned myDIM = my_alg_type::myDIM;

	typedef typename alg::lie_basis<S,Q,myDIM,DEPTH> LBASIS;
	typedef typename LBASIS::KEY LKEY;


private:
	/// The equation.
	std::vector<VF> vecargument;
	/// table stores the vector fields corresponding to combining vecargument with the 
	/// Lie basis elements.
	std::map<unsigned int, VF> table;

public: 
	/// Constructor. Takes 
	lietovectorfield(const std::vector<VF> & vecarg)
		: vecargument(vecarg)
	{
	}

	/// Default Destructor.
	~lietovectorfield(void)
	{
	}

	/// Combines vecargument with a Lie element to output the vector field that we need to exponentiate
	/// to solve the RDE.
	VF lie2vectorfield(const LIE & liearg)
	{
		VF result;
		for (LIE::const_iterator i = liearg.begin(); i != liearg.end(); ++i)
		{
			VF::scaled_add(expand(i->first),(double) i->second, result); 
		}
		return result;
	}

	/// Expanding the Lie basis elements into Vector Fields
	VF expand(const LKEY & k)
	{
		std::map<unsigned int, VF>::iterator it;
		it = table.find(k);
		if (it == table.end())
		{
			return table[k] = rec_expand(k);
		}
		else 
		{
			return it->second;
		}
	}

private:
	/// help function for expand
	VF rec_expand(const LKEY & k)
	{
		if (LIE::basis.letter(k)) 
		{
			return (VF) vecargument[k-1];
		}
		else
		{
			return VF::Lie(expand( LIE::basis.lparent(k)), expand( LIE::basis.rparent(k)));
		}
	}

};



#endif // __lie2vectorfield__