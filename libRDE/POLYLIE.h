/* *************************************************************

Copyright 2010 Terry Lyons, Stephen Buckley, Djalil Chafai, 
Greg Gyurkó and Arend Janssen. 

Distributed under the terms of the GNU General Public License, 
Version 3. (See accompanying file License.txt)

************************************************************* */



#pragma once

#ifndef __POLYLIE__
#define __POLYLIE__


#include "libalgebra/libalgebra.h"
#include "LieMatrix.h"
#include "AbstractSolutionPoint.h"


template <typename my_alg_type>
class POLYLIE :
	public alg::poly_lie<typename my_alg_type::SCA, typename my_alg_type::RAT, my_alg_type::ALPHABET_SIZE, my_alg_type::DEPTH> 
{
	typedef typename my_alg_type::SCA SCA;
	typedef typename my_alg_type::RAT RAT;
	typedef typename my_alg_type::DEG DEG;
	typedef typename my_alg_type::LET LET;

	static const unsigned DEPTH = my_alg_type::DEPTH;
	static const unsigned myDIM = my_alg_type::myDIM;
	static const unsigned ALPHABET_SIZE = my_alg_type::ALPHABET_SIZE;

	typedef alg::poly_lie<SCA,RAT,ALPHABET_SIZE,DEPTH> ALG;

public:
	POLYLIE (ALG in) : ALG(in)
	{
	}

	POLYLIE(LET x, LET y , DEG z) : ALG(x,y,z) //constructs y^z d/dx
	{
	}

	POLYLIE(void) 
	{
	}

	~POLYLIE(void)
	{
	}

	explicit POLYLIE(LieMatrix<my_alg_type> & m) : ALG(LET(1), LET(1), DEG(1))
	{
		(*this) *= m[0][0];

		for(unsigned int j=1; j < myDIM; j++)
		{
			(*this) += (ALG(LET(1), LET(j+1), DEG(1)) * m[0][j]);
		}

		for(unsigned int i=1; i < myDIM; i++)
		{
			for(unsigned int j=0; j < myDIM; j++)
			{
				(*this) += (ALG(LET(i+1), LET(j+1), DEG(1)) * m[i][j]);
			}
		}
	}

	static POLYLIE<my_alg_type>& scaled_add( POLYLIE<my_alg_type> & B, SCA l, POLYLIE<my_alg_type> & result)
	{
		result += B*l;
		return result;
	}

	// the Lie bracket
	static POLYLIE<my_alg_type> Lie(const POLYLIE<my_alg_type> & A, const POLYLIE<my_alg_type> & B)
	{
		return A.ALG::operator *(B);
	}

	AbstractSolutionPoint<my_alg_type> evaluate(AbstractSolutionPoint<my_alg_type> & p)
	{
		AbstractSolutionPoint<my_alg_type> result;

		std::map<alg::poly_lie_basis<SCA,RAT,ALPHABET_SIZE,DEPTH>::KEY,SCA,alg::poly_lie_basis<SCA,RAT,ALPHABET_SIZE,DEPTH>::KEY_LESS>::iterator j=(*this).begin();

		while(j != (*this).end())
		{			
			if (result.find(((*j).first).first) != result.end() )
			{
				result[((*j).first).first] += (*j).second * eval_key(((*j).first).second, p);
			}
			else
			{
				result[((*j).first).first] = (*j).second * eval_key(((*j).first).second, p);
			}

			j++;
			
		}

		return result;
	}

	SCA eval_key(std::map<LET, DEG> k, AbstractSolutionPoint<my_alg_type> & values)
	{
		SCA result(1);

		std::map<LET, DEG>::iterator it;
		AbstractSolutionPoint<my_alg_type>::iterator j;

		for(it = k.begin(); it != k.end(); ++it)
		{
			if (it->second > 0)
			{
				j = values.find(it->first);

				if (j != values.end())
				{
					for (DEG d = 1; d <= it->second; ++d)
					{
						result *= j->second;
					}
				} 
				else
				{   
					result = SCA(0);
				}
			}
		}

		return result;
	}

};

#endif // __POLYLIE__