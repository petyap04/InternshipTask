#include <iostream>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <string>
#include <queue>
#include "FlightAPI.h"

void FlightAPI::readFromFile(std::istream& ifs)
{
	std::string from, to;
	size_t price;
	while (!ifs.eof())
	{
		std::getline(ifs, from, ',');
		std::getline(ifs, to, ',');
		ifs >> price;
		flights[from].push_back(std::pair<std::string, size_t>(to, price));
		ifs.ignore();
	}
}

FlightAPI::FlightAPI(const std::string& filePath)
{
	std::ifstream file(filePath);
	if (!file.is_open())
	{
		throw std::runtime_error("Cannot open flights file: " + filePath);
	}
	std::string line;
	while (std::getline(file, line))
	{
		std::istringstream iss(line);
		std::string from, to;
		int cost;

		if (std::getline(iss, from, ',') && std::getline(iss, to, ',') && (iss >> cost))
		{
			flights[from].push_back({ to, cost });
		}
		else
		{
			throw std::runtime_error("Invalid format in flights file");
		}
	}
}

void FlightAPI::dfs(std::vector<path_and_price_to_destination>& result, const std::string& curr, const std::string& destination,
	std::set<std::string>& visited, std::vector<std::string>& path, size_t currPrice, size_t currDepth)
{
	currDepth++;
	if (max_flights != -1 && currDepth > max_flights) {
		return;
	}
	visited.insert(curr);
	path.push_back(curr);

	if (curr == destination)
	{
		result.push_back({ path, currPrice });
	}
	else {
		for (const auto& flight : flights[curr])
		{
			const std::string& next = flight.first;
			size_t price = flight.second;

			if (!visited.count(next))
			{
				dfs(result, next, destination, visited, path, currPrice + price, currDepth);
			}
		}
	}
	visited.erase(curr);
	path.pop_back();
}

std::vector<path_and_price_to_destination> FlightAPI::getFlights(const std::string& origin, const std::string& destination)
{
	if (flights.find(origin) == flights.end()) {
		throw std::invalid_argument("Origin '" + origin + "' not found in data");
	}
	std::vector<path_and_price_to_destination> result;
	std::set<std::string> visited;
	std::vector<std::string> path;
	size_t currPrice = 0, currDepth = 0;
	if (origin == destination) {
		return result;
	}
	dfs(result, origin, destination, visited, path, currPrice, currDepth);
	std::sort(result.begin(), result.end(), [](const auto& a, const auto& b) {
		return a.second < b.second;
		});
	return result;
}

void FlightAPI::setMax_flights(size_t max_flights)
{
	this->max_flights = max_flights;
}
