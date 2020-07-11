/* *************************************************************

Copyright 2010 Terry Lyons, Stephen Buckley, Djalil Chafai, 
Greg Gyurkó and Arend Janssen. 

Distributed under the terms of the GNU General Public License, 
Version 3. (See accompanying file License.txt)

************************************************************* */



#pragma once

#ifndef MakeBrownianPath_h__
#define MakeBrownianPath_h__


#include "BrownianPath.h"
#include "Path.h"
#include <memory>
#include <functional>

template <typename my_alg_type>
Path<my_alg_type> MakeBrownianPath()
{
	std::shared_ptr< const BrownianPath<my_alg_type> > p(new BrownianPath<my_alg_type>());
	return Path<my_alg_type>(p);
};

template <typename my_alg_type>
Path<my_alg_type> MakeBrownianPath( NormalRandomNumberGenerator & rand, unsigned int d = my_alg_type::myDIM )
{
	std::shared_ptr< const BrownianPath<my_alg_type> > p(new BrownianPath<my_alg_type>(rand,d));
	return Path<my_alg_type>(p);
};

#endif // MakeBrownianPath_h__