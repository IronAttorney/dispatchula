cmake_minimum_required(VERSION 3.22)
project(EventDispatcherDemo)

set(CMAKE_CXX_STANDARD 23)

add_executable(
        EventDispatcherDemo

        main.cpp

        src/event_dispatcher.cpp
        src/event_subscriber.cpp
)

target_include_directories(
        EventDispatcherDemo

        PUBLIC
        src
)