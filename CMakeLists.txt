cmake_minimum_required(VERSION 3.0)
project(hybrid_algorithm CXX)

set(CMAKE_CXX_STANDARD 11)

# set(CMAKE_CXX_FLAGS "-O3")

find_package(nlohmann_json 3.2.0 REQUIRED)

add_executable(hybrid_algorithm main.cpp Schedule.cpp Schedule.h Chromosome.cpp Chromosome.h Population.cpp Population.h)

target_link_libraries(hybrid_algorithm nlohmann_json::nlohmann_json)