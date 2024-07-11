#include <gmock/gmock.h>
#include "..\XM\ProductProvider.h"

using ::testing::Return;

struct ProductStorageMock : IProductStorage
{
	MOCK_METHOD(std::optional<Product>, GetProduct, (Id id), (const, override));
	MOCK_METHOD(size_t, GetProductCountByCategory, (Category category), (const, override));
};

TEST(ProductProviderTests, CacheTest)
{
	auto storage = std::make_shared<ProductStorageMock>();
	EXPECT_CALL(*storage, GetProduct(10)).Times(2).WillRepeatedly(Return(Product{ .id = 10 }));
	EXPECT_CALL(*storage, GetProduct(20)).Times(1).WillOnce(Return(Product{ .id = 20 }));
	EXPECT_CALL(*storage, GetProduct(30)).Times(1).WillOnce(Return(Product{ .id = 30 }));
	EXPECT_CALL(*storage, GetProduct(40)).Times(1).WillOnce(Return(Product{ .id = 40 }));

	ProductProvider provider(storage, 3);

	EXPECT_TRUE(provider.GetProduct(10)->id == 10);
	EXPECT_TRUE(provider.GetProduct(20)->id == 20);
	EXPECT_TRUE(provider.GetProduct(30)->id == 30);
	EXPECT_TRUE(provider.GetProduct(40)->id == 40);
	EXPECT_TRUE(provider.GetProduct(10)->id == 10);
}

TEST(ProductProviderTests, NoCacheUsingTest)
{
	auto storage = std::make_shared<ProductStorageMock>();
	EXPECT_CALL(*storage, GetProductCountByCategory(10)).Times(2).WillOnce(Return(5)).WillOnce(Return(7));

	ProductProvider provider(storage, 3);

	EXPECT_TRUE(provider.GetProductCountByCategory(10) == 5);
	EXPECT_TRUE(provider.GetProductCountByCategory(10) == 7);
}
