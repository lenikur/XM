// XM.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "StorageFactory.h"

#include <boost/asio.hpp>
#include <boost/asio/thread_pool.hpp>

class Printer
{
public:
	void Print(std::optional<Product> product)
	{
		if (!product.has_value())
		{
			std::scoped_lock lock{_mutex};
			std::cout << "thread: " << std::this_thread::get_id() << " - no product info -" << std::endl;
			return;
		}

		std::scoped_lock lock{ _mutex };
		std::cout
			<< "thread: " << std::this_thread::get_id()
			<< " id: " << product->id << "; category: " << product->category << "; name: " << product->name << std::endl;
	}

private:
	std::mutex _mutex;
};

int main()
{
	FakeStorageFactory factory;
	auto storage = factory.CreateProductStorage();

	boost::asio::thread_pool pool{ 3 };
	Printer printer;

	auto task = [&storage, &printer]() 
	{
		for (size_t i = 0; i < 100; ++i)
		{
			printer.Print(storage->GetProduct(10));
			printer.Print(storage->GetProduct(20));
			printer.Print(storage->GetProduct(30));
			printer.Print(storage->GetProduct(40));
			printer.Print(storage->GetProduct(50));
		}
	};

	boost::asio::post(pool.executor(), task);
	boost::asio::post(pool.executor(), task);
	boost::asio::post(pool.executor(), task);

	pool.join();

	std::cout << "category 1 products count: " << storage->GetProductCountByCategory(1) << std::endl;
	std::cout << "category 2 products count: " << storage->GetProductCountByCategory(2) << std::endl;
	std::cout << "category 3 products count: " << storage->GetProductCountByCategory(3) << std::endl;
}
