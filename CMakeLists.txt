cmake_minimum_required(VERSION 3.10)
project(InternshipTask)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    main.cpp
    FlightAPI.cpp
    REST_JSON_API.cpp
)

include_directories(${CMAKE_SOURCE_DIR})

add_executable(InternshipTask ${SOURCES})
