cmake_minimum_required(VERSION 3.10)
project(InternshipTask)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    main.cpp
    flightsContainer.cpp
    jsonFlightParser.cpp
)

include_directories(${CMAKE_SOURCE_DIR})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(InternshipTask ${SOURCES})

target_link_libraries(InternshipTask PRIVATE Threads::Threads)

add_custom_command(
    TARGET InternshipTask POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/flights.txt"
        "$<TARGET_FILE_DIR:InternshipTask>"
)

