cmake_minimum_required(VERSION 3.22)


set(CMAKE_CXX_STANDARD 26)


add_library(
        dispatchula

        INTERFACE
        src/concepts/subscriber_concepts.h
        src/event/event_dispatcher.h
        src/event/event_subscriber.h
        src/request/request.h
        src/request/request_dispatcher.h
        src/request/request_subscriber.h
)

target_include_directories(
        dispatchula

        INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
