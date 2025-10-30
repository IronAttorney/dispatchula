cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 26)
SET(CMAKE_CXX_FLAGS)
add_compile_options(-std=c++23)

add_executable(DispatchulaEventDemo event_demo.cpp)
target_include_directories(DispatchulaEventDemo PUBLIC ../src)

add_executable(DispatchulaRequestDemo request_demo.cpp)
target_include_directories(DispatchulaRequestDemo PUBLIC ../src)

add_executable(DispatchulaCombinedDemo combined_demo.cpp)
target_include_directories(DispatchulaCombinedDemo PUBLIC ../src)