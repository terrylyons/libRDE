/* *************************************************************

Copyright 2010 Terry Lyons, Stephen Buckley, Djalil Chafai, 
Greg Gyurkó and Arend Janssen. 

Distributed under the terms of the GNU General Public License, 
Version 3. (See accompanying file License.txt)

************************************************************* */



#pragma once

#ifndef MakeFractBrownianPath_h__
#define MakeFractBrownianPath_h__


#include "FractBrownianPath.h"
#include "Path.h"
#include <memory>
#include <functional>


template <typename my_alg_type>
Path<my_alg_type> MakeFractBrownianPath(const double H=0.8)
{
	std::tr1::shared_ptr<const FractBrownianPath<my_alg_type> > p(new FractBrownianPath<my_alg_type>(H));
	return Path<my_alg_type>(p);
};

template <typename my_alg_type>
Path<my_alg_type> MakeFractBrownianPath( NormalRandomNumberGenerator & rand, const double H=0.8 )
{
	std::tr1::shared_ptr<const FractBrownianPath<my_alg_type> > p(new FractBrownianPath<my_alg_type>(rand,H));
	return Path<my_alg_type>(p);
};

#endif // MakeFractBrownianPath_h__