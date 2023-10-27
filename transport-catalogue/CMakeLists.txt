cmake_minimum_required(VERSION 3.24)
project(Final_13_Sp_project)

set(CMAKE_CXX_STANDARD 17)

add_executable(Final_13_Sp_project main.cpp domain.cpp geo.cpp json.cpp json_builder.cpp json_reader.cpp map_renderer.cpp request_handler.cpp svg.cpp transport_catalogue.cpp graph.h ranges.h router.h
        transport_router.h
        transport_router.cpp)
