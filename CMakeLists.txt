cmake_minimum_required(VERSION 3.14)
project(miniproject)

set(CMAKE_CXX_STANDARD 14)

add_executable(
        miniproject main.cpp
        project/graph.hpp
        project/generator/GraphBuilder.cpp
        project/generator/GraphBuilder.hpp
        project/generator/ManageFile.cpp
        project/generator/ManageFile.hpp
        )