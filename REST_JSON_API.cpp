#include "REST_JSON_API.h"
#include <iostream>
#include <optional>
#include "json.hpp"
using json = nlohmann::json;

std::pair<std::string, std::string> REST_JSON_API::parseFromJson(const std::string& request, std::optional<int>& max_flights)
{
    auto j = json::parse(request);
    std::pair<std::string, std::string> result({ j["origin"], j["destination"] });

    if (j.contains("maxFlights") && j["maxFlights"].is_number_integer()) {
        int curr = j["maxFlights"];
        if (curr < 0) {
            std::cerr << "The value has to be positive!" << std::endl;
        }
        else {
            max_flights = curr;
        }
    }
    else {
        max_flights = std::nullopt;
    }

    return result;
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

