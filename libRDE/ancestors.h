#pragma once
#include <utility>
#include <iterator>
#include <limits>
#include <exception>

class myexception : public std::exception
{
	virtual const char* what() const throw()
	{
		return "Too coarse dyadic interval overflow";
	}
} ex;

	///// a helper class that maintains a basic dyadic interval (pair) 
	///// that contains every basic dyadic interval it has seen
	//static struct supremum
	//{
	//	basic_dyadic_interval interval_including_zero;
	//	basic_dyadic_interval interval_not_including_zero;

	//	supremum()
	//		: interval_including_zero(0, std::numeric_limits<k_t>::min())
	//		, interval_not_including_zero; (-unit, std::numeric_limits<k_t>::min())
	//	{}

	//	void flush() {
	//		operator=(supremum());
	//	}

	//	const basic_dyadic_interval& operator()(basic_dyadic_interval arg)
	//	{
	//		/// the smallest dyadic interval containing both dyadic intervals
	//		auto imax = [](basic_dyadic_interval arg1, basic_dyadic_interval arg2)->basic_dyadic_interval
	//		{
	//			// both intervals must be on same side of zero
	//			assert(((unit * arg.k >= 0) && (unit * arg.k >= 0)) || ((unit * arg.k < 0) && (unit * arg.k < 0)));
	//			if (arg1 == arg2) return arg1;
	//			if (arg1.n < arg2.n)
	//				arg2.expand_interval(arg2.n - arg1.n);
	//			else
	//				arg1.expand_interval(arg1.n - arg2.n);
	//			if (arg1 == arg2) return arg1;

	//			// different k same n
	//			n_t power1 = floor(log2(abs(double(arg1.k - arg2.k))));
	//			//  no interval of this length can contain both intervals
	//			n_t power2 = ceil(log2(std::max(abs(double(arg1.k)), abs(double(arg2.k)))))));
	//			// this interval from zero contains both intervals 
	//			assert((arg2.expand_interval(power1) != arg2.expand_interval(power1)) && (arg2.expand_interval(power2) == arg2.expand_interval(power2)));
	//			// assert logic is correct

	//			// now do a binary search for the smallest interval containing both 
	//			// start search at the top as most often our intervals will start from zero and the algo becomes O(1)
	//			n_t hi(power2), lo(power1), needle(hi);
	//			while (hi - lo > 1)
	//			{
	//				needle = lo + (hi - lo) / 2;
	//				if (arg2.expand_interval(needle) == arg2.expand_interval(needle))
	//					hi = needle;
	//				else
	//					lo = needle;
	//			}
	//			return arg2.expand(hi);
	//		};

	//		// update the relevant big interval
	//		if (unit * arg.k >= 0)
	//			return interval_including_zero = imax(interval_including_zero, arg);
	//		else
	//			return interval_not_including_zero = imax(interval_not_including_zero, arg);
	//	}

	//} sup;

// a container of successively smaller dyadic intervals containing me
template<class DI>
class ancestors :
	private DI
{
	// name base class type
	typedef DI dyadic_interval;
public:

	typedef decltype(dyadic_interval().n) n_t;
	typedef decltype(dyadic_interval().k) k_t;

	class const_iterator
		: public std::iterator<std::random_access_iterator_tag, dyadic_interval, n_t>
	{
	private:
		std::pair<dyadic_interval, dyadic_interval> range;

		static bool valid(const std::pair<dyadic_interval, dyadic_interval> & range)
		{
			return (range.first) == (range.second) || (range.first).contains(range.second);
		}

		bool consistent(const_iterator& const_iterator)
		{
			assert(valid(const_iterator.range) && valid(range));
			return (range.second == const_iterator.range.second);
		}

	public:

		const_iterator(dyadic_interval now, dyadic_interval end)
			: range(std::pair<dyadic_interval,dyadic_interval>(now, end)) {}

		dyadic_interval operator*() const {
			return range.first;
		}

		bool operator < (const_iterator rhs) const {
			assert(consistent(rhs));
			return operator*().n < (*rhs).n;
		}

		bool operator > (const_iterator rhs) const {
			assert(consistent(rhs));
			return operator*().n > (*rhs).n;
		}

		bool operator >= (const_iterator rhs) const {
			assert(consistent(rhs));
			return operator*().n >= (*rhs).n;
		}

		bool operator <= (const_iterator rhs) const {
			assert(consistent(rhs));
			return operator*().n <= (*rhs).n;
		}

		bool operator == (const_iterator rhs) const {
			// same range
			assert(consistent(rhs));
			return operator*() == (*rhs);
		}

		bool operator != (const_iterator rhs) const {
			assert(consistent(rhs));
			return !(operator*() == (*rhs));
		}

		n_t operator- (const_iterator & rhs) const {
			return (rhs.range.first.n - range.first.n);
		}

		// move towards smallest interval
		// achieved by moving away from smallest interval

		const_iterator operator+(n_t step)
		{
			auto safeaddition = [](k_t a, k_t b) -> bool {return !(a > 0 && b > std::numeric_limits<k_t>::max() - a) && !(a < 0 && b < std::numeric_limits<k_t>::min() - a); };

			// set up const_iterator to hold answer
			const_iterator ans(*this);
			if (step != 0) {

				// make the active interval in this answer accessible
				dyadic_interval& now = ans.range.first;
				dyadic_interval& end = ans.range.second;

				const n_t now_n = now.n;
				const n_t end_n = end.n;

				// increasing the size of the interval
				if (step <= 0) 
				{
					if (safeaddition(now_n, -step))
						now.expand_interval(-step);
					else 
						throw ex;
				}
				else
					// decreasing the size of the interval
				{
					now = end; // reset now to the end
					if (safeaddition(now_n, step) && now_n + step < end_n)// move back from end since the advance suggested by step does not overshoot the end
						now.expand_interval(end_n - (now_n + step));
				}
			}
			return ans;
		}

		const_iterator operator-(n_t step)
		{
			if (step != std::numeric_limits<k_t>::min())
				return (*this) + (-step)
			else
				return ((*this) - 1) - (step + 1);
		}

		const_iterator& operator-=(n_t step)
		{
			return (*this) = operator-(step);
		}

		const_iterator& operator+=(n_t step)
		{
			return (*this) = operator+(step);
		}

		const_iterator& operator--()
		{
			return operator-=(1);
		}

		const_iterator operator--(int)
		{
			auto ans = *this;
			operator--();
			return ans;
		}

		const_iterator& operator++()
		{
			return operator+=(1);
		}

		const_iterator operator++(int)
		{
			auto ans = *this;
			operator++();
			return ans;
		}

		dyadic_interval operator[](n_t offset)
		{
			return *(operator+(offset));
		}


	};

	// introduce basic iterators
	class const_iterator;
	const_iterator begin() {
		dyadic_interval temp(*this);
		return const_iterator(temp);
	}

	const_iterator end() {
		dyadic_interval temp(*this);
		return const_iterator(temp, temp);
	}

	// constructors
	ancestors(dyadic_interval me)
		: dyadic_interval(me)
	{
	}

	explicit ancestors(double me)
		: dyadic_interval(me)
	{
	}

	~ancestors()
	{
	}
};
