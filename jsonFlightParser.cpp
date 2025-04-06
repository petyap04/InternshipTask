#include "jsonFlightParser.h"
#include <iostream>
#include <optional>
#include "json.hpp"
using json = nlohmann::json;

jsonFlightParser::ParsedRecord jsonFlightParser::parseFromJson(const std::string& body)
{
    ParsedRecord record;
    json j;
    try
    {
        j = json::parse(body);
    }
    catch (...)
    {
        throw std::invalid_argument("Invalid JSON");
    }

    for (auto it = j.begin(); it != j.end(); ++it)
    {
        const std::string& key = it.key();
        if (key != "origin" && key != "destination" && key != "maxFlights")
        {
            throw std::invalid_argument("Unexpected field: " + key);
        }
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
        record.maxFlights = j["maxFlights"];
    }
    record.origin = j["origin"];
    record.destination = j["destination"];
    return record;
}


std::string jsonFlightParser::toJson(const std::vector<std::pair<std::vector<std::string>, size_t>> flights)
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

