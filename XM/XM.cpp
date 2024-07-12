#include <boost/asio.hpp>
#include <boost/asio/thread_pool.hpp>

#include "StorageFactory.h"

int main()
{
	spdlog::set_pattern("[thread %t] %v");

	FakeStorageFactory factory;
	auto storage = factory.CreateProductStorage();

	boost::asio::thread_pool pool{ 3 };

	auto task = [&storage]() 
	{
		for (size_t i = 0; i < 100; ++i)
		{
			storage->GetProduct(10);
			storage->GetProduct(20);
			storage->GetProduct(30);
			storage->GetProduct(40);
			storage->GetProduct(50);
		}
	};

	boost::asio::post(pool.executor(), task);
	boost::asio::post(pool.executor(), task);
	boost::asio::post(pool.executor(), task);

	pool.join();

	spdlog::info("category 1 products count: {}", storage->GetProductCountByCategory(1));
	spdlog::info("category 2 products count: {}", storage->GetProductCountByCategory(2));
	spdlog::info("category 3 products count: {}", storage->GetProductCountByCategory(3));
}
