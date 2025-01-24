cmake_minimum_required(VERSION 3.28)

project(A_star)

add_subdirectory(pathfinder)

add_executable(A_star main.cc)

target_link_libraries(A_star pathfinder)