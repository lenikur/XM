#pragma once

#include <shared_mutex>

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
				return product;
			}
		}

		auto product = _storage->GetProduct(id);
		if (!product.has_value())
		{
			return std::nullopt;
		}

		std::scoped_lock lock{_mutex};
		
		// let's check the cache again, as another thread could insert it to the cache while we worked out of lock
		const auto cachedProduct = _cache.GetItem(id);
		if (cachedProduct.has_value())
		{
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