#pragma once
//#include<ostream>

/// A templated function that iterates a function over over all dyadic intervals
/// at a given level, starting at the one that contains the contained end of begin
/// and finishes at the last one before the contained end of end. The function 
/// arguments - a dyadic interval and a level; returned values are ignored.
template<class DI, class f>
void omp_iterate_function_over_dyadic_intervals(const DI& be, const DI&en, const typename DI::dyadic_t::n_t level, f fn)
{
	auto const unit = DI::unit;
	const dyadic_interval begin(be.included_end(), be.n), end(en.included_end(), be.n);
	int no_intervals = (end.k - begin.k)*unit;

#pragma omp parallel for firstprivate(unit, begin)
	for (int i = 0; i < no_intervals; ++i)
	{
		dyadic_interval di(begin);
		di.k += unit*i;
		fn(di, level);
	}
}



//typedef void (*f) (dyadic_interval, dyadic_interval::dyadic_t::n_t);

/// wraps f around every dyadic interval at all levels in a stopping time - starting with the first elements - uses omp at each level
template <class DI, typename f>
void OMPIterateAllLayersOfDyadicIntervalsBottomUp(std::deque<DI> &de_intervals, typename DI::dyadic_t::n_t base_resolution, f fn)
{
//	auto fn1 = [](typename DI::dyadic_interval di, typename DI::dyadic_t::n_t arg) {std::cout << di << " ";};
	DI::dyadic_t::n_t arg(base_resolution);
	std::vector<DI> coarsest_dyadic_partition;
	{
		auto it = std::back_inserter(coarsest_dyadic_partition);
		if (DI::intervaltype == opencl)
			std::copy(de_intervals.rbegin(), de_intervals.rend(), it);
		if (DI::intervaltype == clopen)
			std::copy(de_intervals.begin(), de_intervals.end(), it);
	}

	if (coarsest_dyadic_partition.size() > 0)
	{
		DI end(coarsest_dyadic_partition.back().excluded_end(), base_resolution);
		for (auto p = coarsest_dyadic_partition.begin(); p != coarsest_dyadic_partition.end() && p->n <= base_resolution; ++p)
		{
			auto included_edge = p->included_end();
			for (typename DI::dyadic_t::n_t index = base_resolution; base_resolution >= p->n; --base_resolution)
			{
				DI current(included_edge, base_resolution);
				std::cout << "level:" << base_resolution << " begin:" << current << " end:" << end << "\n";
				//omp_iterate_function_over_dyadic_intervals(current, end, arg, fn1);
				omp_iterate_function_over_dyadic_intervals(current, end, arg, fn);
				std::cout << "\n\n";
			}
		}
	}
}
