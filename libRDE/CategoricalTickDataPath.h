/* *************************************************************

Copyright 2010-2016 Terry Lyons, Hao Ni, Stephen Buckley, Djalil Chafai,
Greg Gyurkó and Arend Janssen.

Distributed under the terms of the GNU General Public License,
Version 3. (See accompanying file License.txt)

************************************************************* */

#pragma once

#ifndef __CategoricalTickDataPath__
#define __CategoricalTickDataPath__

#ifndef DEFAULT_DYADIC_INTERVAL_TYPE
#define DEFAULT_DYADIC_INTERVAL_TYPE opencl
#endif

#include "dyadic.h"
#include "dyadic_interval.h"

static_assert(DEFAULT_DYADIC_INTERVAL_TYPE == opencl,"Categorical TickData requires an opencl environment");
#undef DEFAULT_DYADIC_INTERVAL_TYPE

#include <limits>
#include "libalgebra/libalgebra.h"
#include "libalgebra/implimentation_types.h"

#include "BasePath.h"
#include <iterator>
#include "multi_erase.h"

#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/thread.hpp>

/// Categorical tick data is defined to be a multi-map of time stamped categorical values
/// Categories should be non-zero positive integers of type LET
/// Timestamps should convert to a numerical type

template <typename my_alg_type>
class CategoricalTickDataPath :
	public BasePath<my_alg_type>
{
private:
	//static boost::recursive_mutex path_access;
public:
	using typename BasePath::LIE;
	using typename BasePath::S;
	using typename BasePath::MAPS;
	
	typedef dyadic::k_t k_t;
	typedef dyadic::n_t n_t;

private:
	/// The DataTree Type
	typedef std::map < dyadic_interval, LIE > DataTree;

	/// The original data
	std::multimap<double, alg::LET> TickData;

	/// The transformed data
	mutable DataTree mPathData;

	/// LIE manipulation object
	using BasePath::LibAlgCBH;

public:
	/// 
	using BasePath::DescribePath;

	/// Describe the path over a dyadic interval with caching
	LIE DescribePath(const dyadic_interval &increment, const int accuracy) const
	{
		// get exclusive recursive access for the thread 
		//boost::lock_guard<boost::recursive_mutex> lock(path_access);

		const LIE zero;
		// is increment empty then return a reference to the zero lie element
		if (empty(increment)) return zero;
		// is increment already described?
		auto interval = mPathData.find(increment);
		if (interval != mPathData.end()) return (*interval).second;
		// proceed to add info without further locks >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		// is it requested on the resolution specified in accuracy
		  // if yes, compute and store description from raw data
		if (increment.n == accuracy)
			return mPathData[increment] = DescribePath0(increment, accuracy);
		  // else compute and store recursively from descriptions of children
		dyadic_interval lhs_inc{ increment }, rhs_inc{ increment };
		lhs_inc.shrink_interval_left();
		rhs_inc.shrink_interval_right();
		auto lhs = DescribePath(lhs_inc, accuracy);
		auto rhs = DescribePath(rhs_inc, accuracy);
		std::vector<LIE*> pincs;
		pincs.push_back(&lhs);
		pincs.push_back(&rhs);
		return mPathData[increment] = LibAlgCBH.full(pincs);
	}


private:
	/// tests whether a dyadic interval has any data in it
	bool empty(const dyadic_interval &increment) const
	{
		auto current = TickData.lower_bound(increment.inf());
		auto end = TickData.lower_bound(increment.sup());
		return (end == current);
	}

	/// No Caching; duplicate time stamps allowed
	LIE DescribePath0(const dyadic_interval &increment, const int accuracy) const
	{
		assert(increment.n <= accuracy);

		MAPS m_maps;
		const LIE zero;
		TENSOR tmp(1);// compiler should set this to one!

		// does the interval contain any entry from original data?
		auto current = TickData.lower_bound(static_cast<double>(increment.inf()));
		auto end = TickData.lower_bound(static_cast<double>(increment.sup()));

		while (current != end) {
			// get all with this time stamp
			auto range = TickData.equal_range((*current).first);
			// add these increments
			LIE lie_tmp;
			for (auto it = range.first; it != range.second; ++it)
				lie_tmp += LIE( (*it).second, S(1) );
			tmp *= exp(m_maps.l2t(lie_tmp));
			// update current
			current = range.second;
		}
		return m_maps.t2l(log(tmp));
	}

	/// remove all cache entries affected by data in TickData and all data before a certain cutoff epoch
	void trim_history(double cutoff, dyadic::n_t resolution)
	{
		// trim computed cache
		{
			dyadic_interval cu{ cutoff, resolution };
			auto begin = mPathData.begin();
			auto end = mPathData.upper_bound(cu);
			//delete any interval that started before cutoff epoch
			mPathData.erase(begin, end);
		}	
		// then trim data
		{
			auto begin = TickData.begin();
			auto end = TickData.upper_bound(cutoff);
			TickData.erase(begin, end);
		}
	}
public:
	/// remove all cached signatures of intervals below cu and all intervals that contain cu
	std::set<dyadic_interval> delete_shadow(std::vector<double> time_stamps) const
	{		
		dyadic_interval zero(0,std::numeric_limits::max()); assert(zero.reversed().k < 0);//opencl
		auto& cache = mPathData;

		std::set<dyadic_interval> shadow;
		// build list of elements affected by the time data
		// dyadic intervals are totally ordered but not well ordered

		//given t find the range of nested intervals around t that need consideration in cache	
		auto& bound[&](double t)->std::pair<dyadic_interval, dyadic_interval>
		{
			dyadic_interval now(t, std::numeric_limits::max());
			auto ans = std::make_pair(((t >= 0) ? cache.upper_bound(zero)->first : cache.begin()->first), cache.upper_bound(now)->first);
			return (ans.second.contains(now)) ? ans : ans.second = ans.first, ans;
		}

		for (stamp : timestamps) {
			dyadic_interval now(stamp, std::numeric_limits::max());
			auto bounds = bound(t);
			for (dyadic_interval be(bounds.second); (bounds.first < be) && shadow.insert(be).first; be.expand_interval());
		}
		// shadow built 
		
		multi_erase(mPathData, shadow);
	}

	/// add a timestamped letter removing all polluted cache data
	void add_event(double time_stamp, alg::LET letter)
	{
		std::vector<double> time_stamps;
		time_stamps.push_back(time_stamp);
		delete_shadow(time_stamps);
		TickData.insert(make_pair(time_stamp, letter));
	}

	/// add an ordered container of timestamped letters
	void add_events(std::multimap<double, alg::LET> new_data)
	{
		std::vector<double> time_stamps;
		for (auto t : newdata)
		time_stamps.push_back(t.first);
		delete_shadow(timestamps);
		TickData.insert(new_data.begin(), new_data.end());
	}
	public:

	template <class T>
	CategoricalTickDataPath(const T& data)
		: TickData(data.begin(),data.end())
	{}

protected:
};

#endif // __CategoricalTickDataPath__
