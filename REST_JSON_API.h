#pragma once
#include <iostream>
#include <optional>
#include <vector>
#include "httplib.h"
#include "json.hpp"
using json = nlohmann::json;
using namespace httplib;

class REST_JSON_API
{
public:
	std::pair<std::string, std::string> parseFromJson(const std::string& request, std::optional<int>& max_flights);
	std::string toJson(const std::vector<std::pair<std::vector<std::string>, size_t>>flights);
};

