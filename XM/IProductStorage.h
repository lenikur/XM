#pragma once

#include <string>
#include <memory>
#include <vector>
#include <optional>

using Id = uint64_t;
using Category = uint32_t;

struct Product
{
	Id id{};
	Category category{};
	std::string name;
	std::string description;
	std::vector<std::byte> image;
};

struct IProductStorage
{
	virtual ~IProductStorage() {}
	virtual std::optional<Product> GetProduct(Id id) const = 0;
	virtual size_t GetProductCountByCategory(Category category) const = 0;
};

struct IStorageFactory
{
	virtual ~IStorageFactory() {}
	virtual std::shared_ptr<IProductStorage> CreateProductStorage() const = 0;
};
