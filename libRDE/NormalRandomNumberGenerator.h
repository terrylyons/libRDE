/* *************************************************************

Copyright 2001 - 2010 Terry Lyons and Jessica Gaines

Distributed under the terms of the GNU General Public License, 
Version 3. (See accompanying file License.txt)

************************************************************* */



#pragma once

#ifndef __NORMALRANDOMNUMBERGENERATOR__
#define __NORMALRANDOMNUMBERGENERATOR__


#include <ctime>
#include "stdint.h"
#include "SFMT-seed.h"

static unsigned long timenow = (unsigned long) time(0);
static unsigned long default_unit[1] = {timenow};

/// A class to generate random numbers with a Normal distribution.
class NormalRandomNumberGenerator
{

private:

	int choose_wedge(double);
	double tail(double (*func)()); 
	double wedge(int, double (*func)()); 
	double fastnorm2(double, double (*func)());

public:

	/// Constructor. Arguments are optional and allow the user to choose a seed. 
	NormalRandomNumberGenerator( uint32_t init1_key[]=SFMTseedArray , int key_length=SFMTseedArray_size);	
	double operator()(double variance = 1);

};// END CLASS DEFINITION NormalRandomNumberGenerator

#endif // __NORMALRANDOMNUMBERGENERATOR__
