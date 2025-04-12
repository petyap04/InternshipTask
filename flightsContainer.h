#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <set>
#include <unordered_map>

typedef std::pair<std::string, size_t> destinaton_and_price;
typedef std::unordered_map<std::string, std::vector<destinaton_and_price>> map_with_flights;
typedef std::pair <std::vector<std::string>, size_t> path_and_price_to_destination;

class flightsContainer
{
public:
	flightsContainer() = default;
	flightsContainer(const std::string& filename);

	std::vector<path_and_price_to_destination> getSolutions(const std::string& origin, const std::string& destination, const std::optional<int>& maxFlights);
private:
	map_with_flights flights;

	void readFromFile(std::istream& ifs);
	void dfs(std::vector<path_and_price_to_destination>& result, const std::string& curr, const std::string& destination,
		std::set<std::string>& visited, std::vector<std::string>& path, size_t currPrice, size_t currDepth, const std::optional<int>& maxFlights);
};
