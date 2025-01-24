add_library(pathfinder
    a_star.cc
    ioutils.cc)

target_include_directories(pathfinder PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(pathfinder)