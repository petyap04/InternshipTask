#include <iostream>
#include "FlightAPI.h"
#include "REST_JSON_API.h"
#include "httplib.h"
#include "json.hpp"

using json = nlohmann::json;
using namespace httplib;

int main() {
    try {
        REST_JSON_API api;
        FlightAPI flightAPI("flights.txt");
        Server svr;

        svr.Post("/search_flights", [&](const Request& req, Response& res)
            {
                try {
                    json parsed_json = json::parse(req.body);

                    if (!parsed_json.contains("origin") || !parsed_json.contains("destination")) {
                        res.status = 400;
                        res.set_content(R"({"error": "Missing 'origin' or 'destination'"})", "application/json");
                        return;
                    }

                    if (!parsed_json["origin"].is_string() || !parsed_json["destination"].is_string()) {
                        res.status = 400;
                        res.set_content(R"({"error": "'origin' and 'destination' must be strings"})", "application/json");
                        return;
                    }
                    std::optional<int> max_flights;
                    std::pair<std::string, std::string> origins = api.parseFromJson(req.body, max_flights);
                    std::string origin = origins.first;
                    std::string destination = origins.second;
                    std::vector<path_and_price_to_destination> flights = flightAPI.getFlights(origin, destination);

                    if (flights.empty()) {
                        res.status = 404;
                        res.set_content(R"({"error": "No flights found for the given route"})", "application/json");
                        return;
                    }

                    if (max_flights.has_value()) {
                        if (flights.size() > max_flights.value()) {
                            flights.resize(max_flights.value());
                        }
                    }
                    std::string json_response = api.toJson(flights);
                    res.set_content(json_response, "application/json");
                }
                catch (const json::parse_error& e) {
                    res.status = 400;
                    res.set_content(R"({"error": "Invalid JSON format"})", "application/json");
                }
                catch (const std::exception& e) {
                    res.status = 500;
                    res.set_content(std::string("{\"error\": \"Internal Server Error: ") + e.what() + "\"}", "application/json");
                }
            });

        svr.listen("localhost", 8080);
    }
    catch (const std::invalid_argument& e) {
        std::cout << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "Failed to start server: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
