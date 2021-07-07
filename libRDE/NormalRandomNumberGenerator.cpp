/* *************************************************************

Copyright 1985 - 2010 Terry Lyons and Jessica Gaines

Distributed under the terms of the GNU General Public License, 
Version 3. (See accompanying file License.txt)

************************************************************* */



#include <stdlib.h>
#include "NormalRandomNumberGenerator.h"

extern "C" {
#include "SFMT.h"
}

#include <cmath>


static const double S2=0.59604645e-7;  
static const int  NW=13;      /* Number of wedges */
static const int  NWEDGE=12;  /* Number of wedges - 1 */
static const int  DIMW=25;    /* Dimension of wtree: 2*NWEDGE+1 */
static const int  DIMW1=24;   /* Dimension of wtree-1: 2*NWEDGE */
static const double  S1= 0.3814697266e-5;


static const double AA[225]={
	0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,
		0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,
		0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,
		0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,
		0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,
		0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,
		0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,
		0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,
		0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,
		0.25,0.25,0.25,0.25,0.25,
		0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,
		0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,
		0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,
		0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,
		0.75,0.75,0.75,0.75,0.75,0.75,0.75,0.75,0.75,0.75,
		0.75,0.75,0.75,0.75,0.75,0.75,0.75,0.75,0.75,0.75,
		0.75,0.75,0.75,0.75,0.75,0.75,0.75,0.75,0.75,0.75,
		1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,
		1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,
		1.0,1.0,1.0,
		1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,
		1.25,1.25,1.25,1.25,1.25,1.25,
		1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,
		1.5,
		1.75,1.75,1.75,1.75,1.75,1.75,
		2.0,2.0,2.0,2.0,
		2.25,2.25,
		2.50};
/// A wedgenode used by the NormalRandomNumberGenerator
	typedef struct {double p; short int rson,lson;} wedgenode;
	static const wedgenode wtree[DIMW]=
	{
		{1.000000000000000e+00,2,1},
		{9.494713901105239e-01,4,3},
		{1.000000000000000e+00,6,5},
		{9.179213068830829e-01,21,7},
		{9.494713901105239e-01,22,20},
		{9.781397813377510e-01,8,19},
		{1.000000000000000e+00,9,17},
		{8.991646099922610e-01,23,18},
		{9.781397813377510e-01,24,10},
		{1.000000000000000e+00,11,16},
		{9.721333799719041e-01,13,15},
		{1.000000000000000e+00,14,12},
		{9.974464458185570e-01,-1,-1},
		{9.721333799719041e-01,-1,-1},
		{1.000000000000000e+00,-1,-1},
		{9.688736495650551e-01,-1,-1},
		{9.947466497552960e-01,-1,-1},
		{9.893203311690271e-01,-1,-1},
		{8.894335888100819e-01,-1,-1},
		{9.646565349065180e-01,-1,-1},
		{9.340885174122869e-01,-1,-1},
		{9.179213068830829e-01,-1,-1},
		{9.494713901105239e-01,-1,-1},
		{8.991646099922610e-01,-1,-1},
		{9.781397813377510e-01,-1,-1}
	};

static const double R1=0.3814697266e-5;   /* 2^-18 */
static const double R2=0.953674316e-6;    /* 2^-20 */
static const double  R3=0.238418579e-6;    /* 2^-22 */
static const double  R4=0.59604645e-7;     /* 2^-24 */
static const double  MAXRAN=4.294967296e9; /* 2^32  */

	static const int A[10]={0,0,0,1,1,2,2,3,4,5},
		B[12]={1,1,1,2,3,3,3,4,6,6,7,8},
		C[17]={0,1,2,2,3,3,4,4,4,6,6,6,7,7,9,9,10};
	static const double P[13]=
	{       0.884912651365847,
	0.894643672548026,
	0.910026545246263,
	0.928783242137085,
	0.944950452666289,
	0.960135597462283,
	0.970662936272365,
	0.981843486103641,
	0.987269804689910,
	0.991486919348447,
	0.994040473529890,
	0.997300203936739,
	1.0
	};
	static const double Q[12]=
	{
		0.880834058401424,
			0.885164064747997,
			0.896774888625428,
			0.913824407505834,
			0.930264034831596,
			0.947209250499235,
			0.960305506875539,
			0.974220919528959,
			0.982088312021475,
			0.988221454936694,
			0.992127450599242,
			0.996256397735859
	};
	static const double D[12]=
	{
		0.507894519985643,
			0.786281122462607,
			0.901965046237357,
			0.978080431064381,
			0.992530768577612,
			0.979815257323371,
			0.968564825442572,
			0.958250115973873,
			0.948554192970468,
			0.939291236052193,
			0.930346394387841,
			0.921645337293069
	};
	static const double E[12]=
	{
		16.0,
			8.0,
			5.333333333333333,
			4.0,
			3.078962214587208,
			2.437680485542790,
			1.995299871621917,
			1.672253526481772,
			1.426416923928201,
			1.233401389609144,
			1.078108473581538,
			0.950690738793861
	};

NormalRandomNumberGenerator::NormalRandomNumberGenerator(uint32_t init_key[], int key_length)
{
	init_by_array(init_key,key_length);
}

double NormalRandomNumberGenerator::operator()(double variance)
{
	return fastnorm2(variance, genrand_res53);
}

int NormalRandomNumberGenerator::choose_wedge(double x)
{
	int i,rson,lson;
	lson=wtree[0].lson;
	rson=wtree[0].rson;
	/* wtime++; */
	while(lson>=0)
	{if(x<=wtree[lson].p) 
	{i=lson; lson=wtree[i].lson; rson=wtree[i].rson;}
	else                
	{i=rson; lson=wtree[i].lson; rson=wtree[i].rson;}
	/*  wtime++; */
	}
	if(i<NWEDGE) {printf(" Mistake in choose_wedge: i=%d\n",i);
	exit(1);
	}
	i=DIMW1-i; /* in wtree the wedges are in the 2nd half and in reverse
			   order */
	return i;
}

double NormalRandomNumberGenerator::tail(double (*func)())
{double fac,r,v1,v2;

do
{ v1=(*func)(); 
v2=(*func)(); 
r=v1*v1+v2*v2; 
} while (r>=1.0);
fac=sqrt((9.0-2.0*log(r))/r);
v1=v1*fac; 
if(v1>=3.0) return v1;
else { v2=v2*fac; if(v2>=3.0) return v2;
else return tail(func);
}
}

double NormalRandomNumberGenerator::wedge(int j, double (*func)())
{
	double x,y,temp,xx;
	x=(*func)();
	y=(*func)();
	if(x>y) {temp=x; x=y; y=temp;} /* need x<y */
	xx=0.25*(j+x);
	if(y<=D[j]) return xx;
	else
	{temp=x+E[j]*(exp((j+1)*(j+1)*0.03125-xx*xx*0.5)-1.0);
	if(y<=temp) return xx;
	else return wedge(j,func);
	}
}

double NormalRandomNumberGenerator::fastnorm2(double variance, double (*func)())
{
	unsigned long u,ubits;
	int j;
	unsigned long sign;
	double x, xx;


	u=(unsigned long)((*func)()*MAXRAN);/* tjl */
	sign=(u&0X80000000)>>31; 
	if((ubits=(u & 0X7F800000)>>23)<(unsigned long)225) /* TJL added this typing*/
		x=AA[ubits]+((u&0X7FFF80)>>7)*S1; 
	else { /* not an easy case */
		xx=((u&0X7FFFFF80)>>7)*S2;
		j=choose_wedge(xx); 
		if(j<12) /* wedge or skinny rectangle */
		{if(xx<Q[j]) x=0.25*(j+(*func)());
		else x=wedge(j,func);
		}
		else x=tail(func);
	}
	if(sign) x=(-x);
	return sqrt(variance)*x;

}
