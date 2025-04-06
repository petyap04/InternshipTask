#include <iostream>
#include "FlightAPI.h"
#include "REST_JSON_API.h"
#include "httplib.h"
#include "json.hpp"
using json = nlohmann::json;
using namespace httplib;

int main() {
    REST_JSON_API api;
    FlightAPI flightAPI("flights.txt");
    Server svr;
    std::cout << "klsdkdsfl";
    svr.Post("/search_flights", [&](const Request& req, Response& res) 
    {
    std::optional<int> max_flights;
    std::pair<std::string, std::string> origins = api.parseFromJson(req.body, max_flights);
    std::string origin = origins.first;
    std::string destination = origins.second;

    std::vector<path_and_price_to_destination> flights = flightAPI.getFlights(origin, destination);

    if (max_flights.has_value()) 
    {
        if (flights.size() > max_flights.value()) 
        {
            flights.resize(max_flights.value());
        }
    }
    
    std::string json_response = api.toJson(flights);
    res.set_content(json_response, "application/json");
    });
    svr.listen("localhost", 8080);

    return 0;
}