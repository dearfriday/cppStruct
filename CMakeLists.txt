

project(cppStruct)
cmake_minimum_required(VERSION 3.0)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 ")


SET(BOOST_COMPONENTS)
LIST(APPEND BOOST_COMPONENTS thread
        date_time
        system
        filesystem
        chrono
        program_options
        unit_test_framework
        locale)
FIND_PACKAGE(Boost 1.58 REQUIRED COMPONENTS ${BOOST_COMPONENTS})








add_subdirectory(example)
add_subdirectory(libraries)