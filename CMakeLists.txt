cmake_minimum_required(VERSION 3.22)
project(DispatcherDemo)

set(CMAKE_CXX_STANDARD 23)


add_compile_options(-std=c++23)


SET(CMAKE_CXX_FLAGS)


add_executable(
        DispatcherDemo

        # Uncomment the example you wish to run
#        example/combined_demo.cpp
#        example/event_demo.cpp
        example/request_demo.cpp
)

target_include_directories(
        DispatcherDemo

        PUBLIC
        src
)