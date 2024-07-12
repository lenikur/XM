#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <format>

#include "IProductStorage.h"

std::ostream& operator<<(std::ostream& os, const Product& value)
{
	return os << "[id: " << value.id << "; category: " << value.category << "; name: " << value.name << "]";
}
