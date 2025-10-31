cmake_minimum_required(VERSION 3.22)

project(DispatchulaExample)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(DispatchulaEventDemo event_demo.cpp)
target_include_directories(DispatchulaEventDemo PUBLIC ../src)

add_executable(DispatchulaRequestDemo request_demo.cpp)
target_include_directories(DispatchulaRequestDemo PUBLIC ../src)

add_executable(DispatchulaCombinedDemo combined_demo.cpp)
target_include_directories(DispatchulaCombinedDemo PUBLIC ../src)