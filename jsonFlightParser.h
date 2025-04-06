#pragma once
#include <iostream>
#include <optional>
#include <vector>
#include "httplib.h"
#include "json.hpp"
using json = nlohmann::json;
using namespace httplib;

class jsonFlightParser
{
public:
	struct ParsedRecord {
		std::string origin;
		std::string destination;
		std::optional<int> maxFlights;
	};
	static ParsedRecord parseFromJson(const std::string& request);
	static std::string toJson(const std::vector<std::pair<std::vector<std::string>, size_t>>flights);
};

