cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(
        dispatchula

        INTERFACE

        src/dispatchula_event/event_dispatcher.h
        src/dispatchula_event/event_subscriber.h

        src/dispatchula_request/request.h
        src/dispatchula_request/request_concepts.h
        src/dispatchula_request/request_dispatcher.h
        src/dispatchula_request/request_subscriber.h

        src/dispatchula_shared/dispatchula_concepts.h

        src/dispatchula.h
        src/dispatchula_event.h
        src/dispatchula_request.h
)

target_include_directories(
        dispatchula

        INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
