#ifndef __STREAMEDPIXELPATH__
#define __STREAMEDPIXELPATH__
#include "BasePath.h"

/// StreamedPixelPath
template <typename my_alg_type>
class StreamedPixelPath :
	public BasePath<my_alg_type>
{

public:
	StreamedPixelPath(/*const character c*/)
	{
	};

	~StreamedPixelPath(void)
	{
	};

	/// describe the path over a dyadic interval
	LIE DescribePath(const dyadic_interval & increment, const int accuracy) const
	{
		return LIE();
	};

	/// describe the path over a real interval
	LIE DescribePath(double inf, double sup, int tolerance) const
	{
		return LIE();
	};
};

#endif // 

