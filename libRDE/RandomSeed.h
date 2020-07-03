/* *************************************************************

Copyright 2010 Terry Lyons, Stephen Buckley, Djalil Chafai, 
Greg Gyurkó and Arend Janssen. 

Distributed under the terms of the GNU General Public License, 
Version 3. (See accompanying file License.txt)

************************************************************* */



#pragma once

#ifndef __RANDOMSEED__
#define __RANDOMSEED__


#include <stdlib.h>
#include "NormalRandomNumberGenerator.h"

/// A random seed for the NormalRandomNumberGenerator
class RandomSeed
{
public:
	RandomSeed(void);
	~RandomSeed(void);

	const unsigned int SeedArray_size;
	uint32_t SeedArray[32];
};

#endif // __RANDOMSEED__
