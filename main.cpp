#include <iostream>
#include "flightsContainer.h"
#include "jsonFlightParser.h"
#include "httplib.h"
#include "json.hpp"

using json = nlohmann::json;
using namespace httplib;

int main(int argc, char* argv[]) {
    try {
        if (argc > 1 && (std::string(argv[1]) == "--help" || std::string(argv[1]) == "-h")) {
            std::cout << "Usage: " << argv[0] << " [host] [port] [flights_file]" << std::endl;
            std::cout << "Default host: localhost" << std::endl;
            std::cout << "Default port: 8080" << std::endl;
            std::cout << "Default flights file: flights.txt" << std::endl;
            return 0;
        }
        std::string filePath = "flights.txt";
        if (argc > 3) {
            filePath = argv[3];
        }
        flightsContainer container(filePath);
        Server svr;
        std::string host = "localhost";
        int port = 8080;
        if (argc > 1) {
            host = argv[1];
        }
        if (argc > 2) {
            port = std::stoi(argv[2]);
        }
        std::cout << "Server starting on " << host << ":" << port << std::endl;
        svr.Post("/search_flights", [&](const Request& req, Response& res)
            {
                try {
                    std::cout << "request received .." << std::endl;
                    std::cout << "request body: " << req.body << std::endl;
                    std::cout << "processing" << std::endl;
                    std::cout << "parsing.." << std::endl;
                    jsonFlightParser::ParsedRecord record = jsonFlightParser::parseFromJson(req.body);
                    std::vector<path_and_price_to_destination> flights = container.getSolutions(record.origin, record.destination, record.maxFlights);
                    std::cout << "returned status code: " << (flights.empty() ? StatusCode::NotFound_404 : StatusCode::OK_200) << std::endl;
                    std::string json_response = jsonFlightParser::toJson(flights);
                    res.set_content(json_response, "application/json");
                }
                catch (const std::invalid_argument& e) {
                    std::cout << "JSON request error: " << e.what() << std::endl;
                    res.status = (int)StatusCode::BadRequest_400;
                    res.set_content(R"({"error": "Invalid fields in JSON"})", "application/json");
                }
                catch (const json::parse_error& e) {
                    std::cout << "JSON parsing error: " << e.what() << std::endl;
                    res.status = (int)StatusCode::BadRequest_400;
                    res.set_content(R"({"error": "Invalid JSON format"})", "application/json");
                }
                catch (const std::exception& e) {
                    std::cout << "Exception: " << e.what() << std::endl;
                    res.status = (int)StatusCode::InternalServerError_500;
                    res.set_content(std::string("{\"error\": \"Internal Server Error: ") + e.what() + "\"}", "application/json");
                }
            });
        std::cout << "Listening on " << host << ":" << port << std::endl;
        svr.listen(host.c_str(), port);
    }
    catch (const std::invalid_argument& e) {
        std::cout << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "Failed to start server: " << e.what() << std::endl;
    }
    return 0;
}