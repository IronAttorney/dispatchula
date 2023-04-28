cmake_minimum_required(VERSION 3.22)


set(CMAKE_CXX_STANDARD 23)
SET(CMAKE_CXX_FLAGS)
add_compile_options(-std=c++23)


add_executable(
        DispatcherDemo

        # Uncomment the example you wish to run
        combined_demo.cpp
#        event_demo.cpp
#        request_demo.cpp
)

target_include_directories(
        DispatcherDemo

        PUBLIC
        ../src
)
