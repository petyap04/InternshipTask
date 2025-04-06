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

class FlightAPI
{
public:
	FlightAPI() = default;
	FlightAPI(const std::string& filename);

	std::vector<path_and_price_to_destination> getFlights(const std::string& origin, const std::string& destination);
	void setMax_flights(size_t max_flights);

private:
	map_with_flights flights;
	int max_flights = -1;// the default value will be -1 (this will mean that there is no limit for the depth)

	void readFromFile(std::istream& ifs);
	void dfs(std::vector<path_and_price_to_destination>& result, const std::string& curr, const std::string& destination,
		std::set<std::string>& visited, std::vector<std::string>& path, size_t currPrice, size_t currDepth);
};