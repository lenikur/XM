#pragma once

#include "IProductStorage.h"
#include "ProductProvider.h"

class FakeProductStorage : public IProductStorage
{
public:
	std::optional<Product> GetProduct(Id id) const override 
	{
		switch (id)
		{
		case 10: return Product{ .id = 10, .category = 1, .name = "10" };
		case 20: return Product{ .id = 20, .category = 1, .name = "20" };
		case 30: return Product{ .id = 30, .category = 1, .name = "30" };
		case 40: return Product{ .id = 40, .category = 2, .name = "40" };
		}

		return std::nullopt; 
	}

	size_t GetProductCountByCategory(Category category) const override 
	{ 
		switch (category)
		{
		case 1: return 3;
		case 2: return 1;
		}

		return 0;
	}
};

class FakeStorageFactory : public IStorageFactory
{
public:
	std::shared_ptr<IProductStorage> CreateProductStorage() const override
	{
		return std::make_shared<ProductProvider>(std::make_shared<FakeProductStorage>(), 3);
	}
};