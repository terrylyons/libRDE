/* *************************************************************

Copyright 2010 Terry Lyons, Stephen Buckley, Djalil Chafai, 
Greg Gyurkó and Arend Janssen. 

Distributed under the terms of the GNU General Public License, 
Version 3. (See accompanying file License.txt)

************************************************************* */



#include "RandomSeed.h"

RandomSeed::RandomSeed(void) : SeedArray_size(16)
{
	srand(timenow);
	for (unsigned int i=0; i<32; i++)
	{
		SeedArray[i] = rand()%65535;
	}
};

RandomSeed::~RandomSeed(void)
{
}
