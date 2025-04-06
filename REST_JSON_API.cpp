#include "REST_JSON_API.h"
#include <iostream>
#include <optional>
#include "json.hpp"
using json = nlohmann::json;

std::pair<std::string, std::string> REST_JSON_API::parseFromJson(const std::string& body, std::optional<int>& maxFlights)
{
    json j;
    try
    {
        j = json::parse(body);
    }
    catch (...)
    {
        throw std::invalid_argument("Invalid JSON");
    }
    if (!j.contains("origin") || !j.contains("destination"))
    {
        throw std::invalid_argument("Missing 'origin' or 'destination'");
    }
    if (!j["origin"].is_string() || !j["destination"].is_string())
    {
        throw std::invalid_argument("'origin' and 'destination' must be strings");
    }
    if (j.contains("maxFlights"))
    {
        if (!j["maxFlights"].is_number_integer())
        {
            throw std::invalid_argument("'maxFlights' must be an integer");
        }
        maxFlights = j["maxFlights"];
    }
    return { j["origin"], j["destination"] };
}


std::string REST_JSON_API::toJson(const std::vector<std::pair<std::vector<std::string>, size_t>> flights)
{
    json j = json::array();
    for (const auto& flight : flights)
    {
        j.push_back({
            {"route", flight.first},
            {"price", flight.second}
            });
    }
    return j.dump(4);
}

