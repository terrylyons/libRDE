#pragma once
#include <map>
#include <set>

/// A function that removes entries from container data of (multi)map type, that have keys in an ordered set
/// with a maximal complexity min(n*logm, m*logn) where m and n are the sizes of the two containers. 
template <class value_t, class key_t>
void multi_erase(std::map<key_t, value_t>& data, const std::set<key_t>& keys)
{
	auto iter_k{ keys.begin() };
	auto iter_d{ data.begin() };
	int i = 0;
	while (iter_k != keys.end() && iter_d != data.end()) {
		switch (i) {
		case 0:
		{
			auto range_d = data.equal_range(*iter_k);
			iter_d = data.erase(range_d.first, range_d.second);//C++11
			++iter_k; // set so no duplicates
		}
			i = 1;
			break;
		case 1:
		{
			auto range_k = keys.equal_range((*iter_d).first);
			if (range_k.first != range_k.second) // points to delete
			{
				auto range_d = data.equal_range(*iter_k);
				iter_d = data.erase(range_d.first, range_d.second);
			}
			iter_k = range_k.second;
		}
			i = 0;
			break;
		}
	}
}

/// A function that removes entries from container data of map type, that have keys in an ordered set shadow
/// with a maximal complexity min(nlogm, mlogn) where m and n are the sizes of the two containers. 
template <class value_t, class key_t>
void multi_erase(std::multimap<key_t, value_t>& data, const std::set<key_t>& keys)
{
	auto iter_k{ keys.begin() };
	auto iter_d{ data.begin() };
	int i = 0;
	while (iter_k != keys.end() && iter_d != data.end()) {
		switch (i) {
		case 0:
		{
			auto range_d = data.equal_range(*iter_k);
			iter_d = data.erase(range_d.first, range_d.second);//C++11
			//++iter_k; // set so no duplicates
		}
		i = 1;
		break;
		case 1:
		{
			auto range_k = keys.equal_range((*iter_d).first);
			iter_k = range_k.first;
			if (range_k.first != range_k.second) // points to delete
			{
				auto range_d = data.equal_range(*iter_k);
				iter_d = data.erase(range_d.first, range_d.second);
			}
			iter_k = range_k.second;
		}
		i = 0;
		break;
		}
	}
}

