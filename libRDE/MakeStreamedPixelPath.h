#ifndef MakeStreamedPixelPath_h__
#define MakeStreamedPixelPath_h__


#include "StreamedPixelPath.h"
#include "Path.h"
#include <memory>
#include <functional>


template <typename my_alg_type>
Path<my_alg_type> MakeStreamedPixelPath(/*const character  c*/)
{
	std::tr1::shared_ptr<const StreamedPixelPath<my_alg_type> > p(new StreamedPixelPath<my_alg_type>(/*c*/));
	return Path<my_alg_type>(p);
};

#endif // MakeStreamedPixelPath_h__
