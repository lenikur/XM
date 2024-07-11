#pragma once


#include <unordered_map>
#include <list>
#include <optional>

template<typename Key, typename Value>
class LRUCache
{
	using Cache = std::list<std::pair<Key, Value>>;
public:
	explicit LRUCache(size_t capacity)
		: _capacity{ capacity }
	{
	}

	template<typename _Key, typename _Value>
	void Insert(_Key&& key, _Value&& value)
	{
		const auto it = _hashTable.find(key);
		if (_hashTable.end() != it)
		{
			_cache.splice(_cache.begin(), _cache, it->second);
			return;
		}

		insert(std::forward<_Key>(key), std::forward<_Value>(value));
	}

	// The return result is valid until next Insert call
	std::optional<std::reference_wrapper<Value>> GetItem(const Key& key) const
	{
		const auto it = _hashTable.find(key);
		if (_hashTable.end() == it)
		{
			return std::nullopt;
		}

		_cache.splice(_cache.begin(), _cache, it->second);
		return _cache.front().second;
	}

private:
	template<typename _Key, typename _Value>
	void insert(_Key&& key, _Value&& value)
	{
		// think about exceptions
		if (_cache.size() + 1 > _capacity)
		{
			_hashTable.erase(_cache.back().first);
			_cache.pop_back();
		}

		_cache.emplace_front(std::forward<_Key>(key), std::forward<_Value>(value));
		_hashTable.emplace(std::forward<_Key>(key), _cache.begin());
	}



private:
	const size_t _capacity;
	mutable Cache _cache;
	std::unordered_map<Key, typename Cache::const_iterator> _hashTable;
};