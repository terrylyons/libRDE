#pragma once


//
//	template <class value_t, class key_t>
//	friend void multi_erase(std::map<key_t, value_t>& data, const std::set<key_t>& keys);
//
//	cache() :
//		base(), zero( 0, std::numeric_limits<n_t>::max() ), to_zero(-unit, std::numeric_limits<n_t>::max() )
//	{
//	}
//
//	~cache()
//	{
//	}
//
//	/// dyadic intervals are totally ordered but the order of disjoint intervals
//	/// depends on the choice of opencl or clopen
//
//	/// the ancestor of me index generations earlier
//	static auto ancestor(unsigned index, dyadic_interval me) -> dyadic_interval {
//		return me.expand_interval(index);// a modified copy
//	};
//
//	/// the first representable ancestor of me, and the generation gap or index
//	static auto first_ancestor(unsigned& index, dyadic_interval me) -> dyadic_interval {
//		if (me.k == 0) {
//			index = me.n - std::numeric_limits<n_t>::min();
//			return dyadic_interval(0, std::numeric_limits<n_t>::min());
//		}
//		else {
//			index = (unsigned)(me.k > 0) ? floor(log2(double(std::numeric_limits<k_t>::max()) / double(me.k)))
//				: floor(log2(double(std::numeric_limits<k_t>::min()) / double(me.k)));
//			return ancestor(index, me);
//		}
//	};
//
//	/// an interval [di1,di2) spans di if !(di < di1) and (di < d2)
//	static auto spans(dyadic_interval begin, dyadic_interval end, dyadic_interval target)->bool
//	{
//		return !(target < begin) && (target < end);
//	};
//
//	/// given a dyadic interval target and end, we can consider all intervals [ancestor(j,end),end) and ask if for any representable
//	/// dyadic interval ancestor(j,end) the range [ancestor(j,end),end) spans the target, returning some [ancestor(j,end),end)
//	/// spanning target, or [end,end) if no such range exists
//	static auto spans(dyadic_interval end, dyadic_interval target)-> dyadic_interval
//	{
//		unsigned index;
//		dyadic_interval begin{ first_ancestor(index, end) };
//		return (spans(begin, end, target)) ? begin : end;
//	};
//
//	static auto ancestral_equal_range(dyadic_interval end, dyadic_interval target)->std::pair<dyadic_interval, dyadic_interval>
//	{
//		auto comp = [&](unsigned i, dyadic_interval target)->bool {
//			return ancestor(index - i, end) < di;
//		};
//		unsigned index;
//		first_ancestor(index, end);
//		auto offset = std::equal_range(0, index, target, comp);
//		return std::make_pair(ancestor(index - offset.first, end), ancestor(index - offset.second, end));
//	};
//
//	static auto jet(double t)->std::pair<dyadic_interval, dyadic_interval>
//	{
//		dyadic_interval end((double)t);
//		return std::make_pair(
//			ancestral_equal_range(end, (*upper_bound(end)).first).first,
//			ancestral_equal_range(end, (*lower_bound(end)).first).second
//		);
//	}
//
//	static auto jet(const std::set<double>& T)->std::set<dyadic_interval> {
//		std::set<dyadic_interval> ans;
//		for (auto t : T)
//		{
//			auto range = jet(t);
//			dyadic_interval end = range.second;
//			dyadic_interval begin = range.first;
//			while (begin < end && ans.insert(end.expand_interval())) {};
//		}
//		return ans;
//	}
//
//	/// remove all cached entries before cu and all intervals that contain cu
//	std::set<dyadic_interval> delete_shadow(std::vector<double> time_stamps) const
//	{
//		std::set<dyadic_interval> shadow;
//		// build list of elements affected by the time data
//		// dyadic intervals are totally ordered but not well ordered
//
//		for (stamp : timestamps) {
//			dyadic_interval now(stamp, std::numeric_limits::max());
//			auto bounds = bound(t);
//			for (dyadic_interval be(bounds.second); (bounds.first < be) && shadow.insert(be).first; be.expand_interval());
//		}
//	}
//
//	/// add a timestamped letter removing all polluted cache data
//	void add_event(double time_stamp, payload letter)
//	{
//		std::vector<double> time_stamps;
//		time_stamps.push_back(timestamp);
//		delete_shadow(timestamps);
//		TickData.insert(make_pair(time_stamp, letter));
//	}
//
//	/// add an ordered container of timestamped letters
//	void add_events(std::multimap<double, payload> new_data)
//	{
//		std::vector<double> time_stamps;
//		for (auto t : newdata)
//			time_stamps.push_back(t.first);
//		delete_shadow(timestamps);
//		TickData.insert(new_data.begin(), new_data.end());
//	}
//public:
//
//protected:
//};
