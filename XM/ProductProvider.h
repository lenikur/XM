#pragma once

#include <shared_mutex>
#include "logging.h"
#include "IProductStorage.h"
#include "lru_cache.h"

// Wrapper for DB-related IProductStorage implementation
class ProductProvider : public IProductStorage
{
public:
	explicit ProductProvider(std::shared_ptr<IProductStorage> storage, size_t cacheSize)
		: _cache{ cacheSize }
		, _storage{ std::move(storage) }
	{
		if (!_storage)
		{
			throw std::invalid_argument("a storage can't be null");
		}
	}

	std::optional<Product> GetProduct(Id id) const override
	{
		{
			std::shared_lock lock{ _mutex };
			const auto product = _cache.GetItem(id);
			if (product.has_value())
			{
				spdlog::info("item {} from cache: {}", id, static_cast<const Product&>(*product));
				return product;
			}
		}

		auto product = _storage->GetProduct(id);
		if (!product.has_value())
		{
			spdlog::info("no item {} in DB", id);
			return std::nullopt;
		}

		spdlog::info("item {} from DB: {}", id, *product);

		std::scoped_lock lock{_mutex};
		
		// let's check the cache again, as another thread could insert it to the cache while we worked out of lock
		const auto cachedProduct = _cache.GetItem(id);
		if (cachedProduct.has_value())
		{
			spdlog::info("item {} placed to cache by another thread: {}", id, static_cast<const Product&>(*cachedProduct));
			return cachedProduct;
		}

		return _cache.Insert(product->id, std::move(*product));
	}

	size_t GetProductCountByCategory(Category category) const
	{
		return _storage->GetProductCountByCategory(category);
	}

private:
	mutable std::shared_mutex _mutex;
	mutable LRUCache<Id, Product> _cache;
	std::shared_ptr<IProductStorage> _storage;
};