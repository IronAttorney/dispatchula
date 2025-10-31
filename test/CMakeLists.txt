cmake_minimum_required(VERSION 3.22)

project(DispatchulaTest)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(../cmake/get_cpm.cmake)

CPMAddPackage("gh:catchorg/Catch2#v3.11.0")

add_executable(DispatchulaEventTest event_test.cpp)
target_include_directories(DispatchulaEventTest PUBLIC ../src)
target_link_libraries(DispatchulaEventTest PRIVATE Catch2::Catch2WithMain)

add_executable(DispatchulaRequestTest request_test.cpp)
target_include_directories(DispatchulaRequestTest PUBLIC ../src)
target_link_libraries(DispatchulaRequestTest PRIVATE Catch2::Catch2WithMain)