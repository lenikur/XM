#pragma once


#include <unordered_map>
#include <list>
#include <optional>
#include <exception>

template<typename Key, typename Value, typename Hasher = std::hash<Key>, typename KeyEq = std::equal_to<Key>>
class LRUCache
{
	using Cache = std::list<std::pair<Key, Value>>;
public:
	explicit LRUCache(size_t capacity)
		: _capacity{ capacity }
	{
		if (_capacity == 0)
		{
			throw std::invalid_argument{ "capacity should not be 0" };
		}
	}

	using ValueRefOpt = std::optional<std::reference_wrapper<const Value>>;

	template<typename _Key, typename _Value>
	ValueRefOpt Insert(_Key&& key, _Value&& value)
	{
		const auto it = _hashTable.find(key);
		if (_hashTable.end() != it)
		{
			_cache.splice(_cache.begin(), _cache, it->second);
			return _cache.front().second;
		}

		return insert(std::forward<_Key>(key), std::forward<_Value>(value));
	}

	// The return result is valid until next Insert call
	ValueRefOpt GetItem(const Key& key) const
	{
		const auto it = _hashTable.find(key);
		if (_hashTable.end() == it)
		{
			return std::nullopt;
		}

		_cache.splice(_cache.begin(), _cache, it->second);
		return _cache.front().second;
	}

	void RemoveItem(const Key& key)
	{
		const auto it = _hashTable.find(key);
		if (_hashTable.end() == it)
		{
			return;
		}

		_cache.erase(it->second);
		_hashTable.erase(it);
	}

private:
	template<typename _Key, typename _Value>
	ValueRefOpt insert(_Key&& key, _Value&& value)
	{
		if (_cache.size() + 1 > _capacity)
		{
			_hashTable.erase(_cache.back().first);
			_cache.pop_back();
		}

		_cache.emplace_front(std::forward<_Key>(key), std::forward<_Value>(value));
		_hashTable.emplace(std::forward<_Key>(key), _cache.begin());
		
		return _cache.front().second;
	}

private:
	const size_t _capacity;
	mutable Cache _cache;
	std::unordered_map<Key, typename Cache::const_iterator, Hasher, KeyEq> _hashTable;
};