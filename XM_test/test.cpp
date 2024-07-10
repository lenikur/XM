#include "pch.h"
#include "..\XM\lru_cache.h"

TEST(LRUTests, Insertion)
{
	LRUCache<int, int> cache(3);
	cache.Insert(1, 1);
	cache.Insert(2, 2);
	cache.Insert(3, 3);
	EXPECT_TRUE(*cache.GetItem(1) == 1);
	EXPECT_TRUE(*cache.GetItem(2) == 2);
	EXPECT_TRUE(*cache.GetItem(3) == 3);
}

TEST(LRUTests, CapacityLimit)
{
	LRUCache<int, int> cache(3);
	cache.Insert(1, 1);
	cache.Insert(2, 2);
	cache.Insert(3, 3);
	cache.Insert(4, 4);
	EXPECT_TRUE(cache.GetItem(1) == nullptr);
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
	EXPECT_TRUE(cache.GetItem(2) == nullptr);
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
