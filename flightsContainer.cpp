#include <iostream>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <string>
#include <queue>
#include "flightsContainer.h"

void flightsContainer::readFromFile(std::istream& ifs)
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

flightsContainer::flightsContainer(const std::string& filePath)
{
	std::cout << "Opening file: " << filePath << std::endl;
	std::ifstream file(filePath);
	if (!file.is_open())
	{
		throw std::runtime_error("Cannot open flights file: " + filePath);
	}
	std::cout << "File openned succesfully!" << std::endl;
	std::string line;
	int lines = 0;
	while (std::getline(file, line))
	{
		lines++;
		std::istringstream iss(line);
		std::string from, to;
		int cost;

		if (std::getline(iss, from, ',') && std::getline(iss, to, ',') && (iss >> cost))
		{
			std::cout << "Parsed record: Origin: " << from << ", Destination: " << to << ", Price: " << cost << std::endl;

			flights[from].push_back({ to, cost });
		}
		else
		{
			throw std::runtime_error("Invalid format in flights file");
		}
	}
	std::cout << "Lines parsed: " << lines << std::endl;
}

void flightsContainer::dfs(std::vector<path_and_price_to_destination>& result, const std::string& curr, const std::string& destination,
	std::set<std::string>& visited, std::vector<std::string>& path, size_t currPrice, size_t currDepth, const std::optional<int>& maxFlights)
{
	currDepth++;
	if (maxFlights && currDepth > (*maxFlights) + 1) {
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
				dfs(result, next, destination, visited, path, currPrice + price, currDepth, maxFlights);
			}
		}
	}
	visited.erase(curr);
	path.pop_back();
}

std::vector<path_and_price_to_destination> flightsContainer::getSolutions(const std::string& origin, const std::string& destination, const std::optional<int>& maxFlights)
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
	dfs(result, origin, destination, visited, path, currPrice, currDepth, maxFlights);
	std::sort(result.begin(), result.end(), [](const auto& a, const auto& b) {
		return a.second < b.second;
		});
	return result;
}
