#include <gmock/gmock.h>
#include "..\XM\lru_cache.h"

TEST(LRUTests, Insertion)
{
	LRUCache<int, int> cache(3);
	cache.Insert(1, 1);
	cache.Insert(2, 2);
	cache.Insert(3, 3);
	EXPECT_TRUE(cache.GetItem(1) == 1);
	EXPECT_TRUE(cache.GetItem(2) == 2);
	EXPECT_TRUE(cache.GetItem(3) == 3);
}

TEST(LRUTests, CapacityLimit)
{
	LRUCache<int, int> cache(3);
	cache.Insert(1, 1);
	cache.Insert(2, 2);
	cache.Insert(3, 3);
	cache.Insert(4, 4);
	EXPECT_FALSE(cache.GetItem(1).has_value());
	EXPECT_TRUE(*cache.GetItem(2) == 2);
	EXPECT_TRUE(*cache.GetItem(3) == 3);
	EXPECT_TRUE(*cache.GetItem(4) == 4);
}

TEST(LRUTests, GetItemRaisesPriority)
{
	LRUCache<int, int> cache(3);
	cache.Insert(1, 1);
	cache.Insert(2, 2);
	cache.Insert(3, 3);
	EXPECT_TRUE(*cache.GetItem(1) == 1);
	cache.Insert(4, 4);
	EXPECT_FALSE(cache.GetItem(2).has_value());
}

TEST(LRUTests, InsertionOfDuplicates)
{
	LRUCache<int, int> cache(3);
	cache.Insert(1, 1);
	cache.Insert(2, 2);
	cache.Insert(2, 2);
	cache.Insert(3, 3);
	EXPECT_TRUE(*cache.GetItem(1) == 1);
	EXPECT_TRUE(*cache.GetItem(2) == 2);
	EXPECT_TRUE(*cache.GetItem(3) == 3);
}

TEST(LRUTests, NoItem)
{
	LRUCache<int, int> cache(3);
	EXPECT_FALSE(cache.GetItem(1).has_value());
	cache.Insert(1, 1);
	EXPECT_FALSE(cache.GetItem(2).has_value());
}

TEST(LRUTests, ZeroSize)
{
	using Cache = LRUCache<int, int>;
	EXPECT_THROW(Cache{ 0 }, std::invalid_argument);
}

TEST(LRUTests, Remove)
{
	LRUCache<int, int> cache(3);
	cache.Insert(1, 1);
	cache.Insert(2, 2);
	cache.Insert(3, 3);
	cache.RemoveItem(3);
	EXPECT_TRUE(cache.GetItem(1) == 1);
	EXPECT_TRUE(cache.GetItem(2) == 2);
	EXPECT_FALSE(cache.GetItem(3).has_value());
	cache.Insert(3, 3);
	EXPECT_TRUE(cache.GetItem(3).has_value());
}
