/**
 * 
 * 
 * 
 * 
 * 
 */


#pragma once

#ifndef TRANSFORMED_PATH__H__
#define TRANSFORMED_PATH__H__

#include "BasePath.h"


template <typename my_alg_type>
class RestrictedPath : BasePath<my_alg_type>
{
public:
    typedef BasePath<my_alg_type> BPATH;
	typedef typename my_alg_type::LIE LIE;
	typedef typename my_alg_type::S S;

	RestrictedPath(std::shared_ptr<const BPATH> base, const interval& interval)
		: pImpl{base}, _restriction{interval}
	{}

	~RestrictedPath() {}

	LIE DescribePath(const dyadic_interval & di, const int accuracy) const
	{
		if (di.sup() < _restriction.inf() || di.inf() > _restriction.sup())
			return LIE();

		interval intersection {
			std::max(double(di.inf()), _restriction.inf()),
			std::min(double(di.sup()), _restriction.sup())
		};
		
		return pImpl->DescribePath(intersection, accuracy);
	}

	LIE DescribePath(double inf, double sup, int tolerance) const
	{
		if (sup < _restriction.inf() || inf > _restriction.sup())
			return LIE();

		interval intersection {
			std::max(inf, _restriction.inf()),
			std::min(sup, _restriction.sup())
		};
		
		return pImpl->DescribePath(intersection, accuracy);
	}

	LIE DescribePath(const interval& interval, const int accuracy) const
	{
		return DescribePath(interval.inf(), interval.sup(), accuracy);
	}

private:
	const interval _restriction;
    std::shared_ptr<const BPATH> pImpl;
};





template <typename my_alg_type>
class ConcatenatedPath : BasePath<my_alg_type>
{
public:
    typedef BasePath<my_alg_type> BPATH;
    typedef std::shared_ptr<const BPATH> PIMPL_T;
	typedef typename my_alg_type::LIE LIE;
	typedef typename my_alg_type::TENSOR TENSOR;
    typedef typename my_alg_type::MAPS MAPS;

    ConcatenatedPath(PIMPL_T lpath, PIMPL_T rpath)
        : _lpath{lpath}, _rpath{rpath}
    {}

    ~ConcatenatedPath() {}

    TENSOR Signature(const  dyadic_interval& increment, const int accuracy) const
    {
        return _lpath->Signature(increment, accuracy) * _rpath->Signature(increment, accuracy);
    }

    TENSOR Signature(const dyadic_interval &increment) const
	{
		return _lpath->Signature(increment) * _rpath->Signature(increment);
	};

	/// Describe the path signature over a real interval with specified time accuracy 2^-(tolerance)
	TENSOR Signature(double inf, double sup, int tolerance) const
	{
		return _lpath->Signature(inf, sup, tolerance) 
               * _rpath->Signature(inf, sup, tolerance);
	};

    LIE DescribePath(const dyadic_interval &increment, const int accuracy) const
    {
        MAPS maps;
        return maps.t2l(log(Signature(increment, accuracy)));
    }

    LIE DescribePath(const dyadic_interval &increment) const
	{
        MAPS maps;
		return maps.t2l(log(Signature(increment, increment.n)));
	};

    LIE DescribePath(double inf, double sup, int tolerance) const
    {
        MAPS maps;
		return maps.t2l(log(Signature(inf, sup, tolerance)));
    }

private:
    std::shared_ptr<const BPATH> _lpath;
    std::shared_ptr<const BPATH> _rpath;
};











#endif