cmake_minimum_required(VERSION 3.24)
project(Ex15_1)

set(CMAKE_CXX_STANDARD 17)

add_executable(Ex15_1 main.cpp domain.cpp geo.cpp json.cpp json_reader.cpp map_renderer.h map_renderer.cpp request_handler.cpp svg.h svg.cpp transport_catalogue.cpp)
