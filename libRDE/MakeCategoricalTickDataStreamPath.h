/* *************************************************************

Copyright 2010 -2016 Terry Lyons, Hao Ni, Stephen Buckley, Djalil Chafai, 
Greg Gyurkó and Arend Janssen. 

Distributed under the terms of the GNU General Public License, 
Version 3. (See accompanying file License.txt)

************************************************************* */



#pragma once

#ifndef MakeCategoricalTickDataStreamPath_h__
#define MakeCategoricalTickDataStreamPath_h__


#include "CategoricalTickDataStreamPath.h"
#include "Path.h"
#include <memory>
#include <functional>
#include <map>

/// Categorical tick data is defined to be a multi-map of time stamped categorical values
/// Categories should be non-zero positive integers of type LET
/// Timestamps should convert to a numerical type
//MakeCategoricalTickDataStreamPath<my_alg_type, T>(arg)
template <typename my_alg_type, class T>
Path<my_alg_type> MakeCategoricalTickDataStreamPath(const T & arg)
{
	std::tr1::shared_ptr< const CategoricalTickDataStreamPath<my_alg_type> > p(new CategoricalTickDataStreamPath<my_alg_type>(arg));
	return Path<my_alg_type>(p);
};

//template <typename my_alg_type>
//Path<my_alg_type> MakeCategoricalTickDataStreamPath(std::multimap<double, alg::LET> initial_data )
//{
//	std::tr1::shared_ptr< const CategoricalTickDataStreamPath<my_alg_type> > p(new CategoricalTickDataStreamPath<my_alg_type>(initial_data));
//	return Path<my_alg_type>(p);
//};
//
#endif // MakeCategoricalTickDataStreamPath_h__