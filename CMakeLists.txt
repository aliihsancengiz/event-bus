cmake_minimum_required(VERSION 3.16)
project("Event Bus" VERSION 0.0.8)

set(TARGET "eventbuspoc")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(${TARGET} main.cpp)

target_include_directories(${TARGET}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/EventBus
)

target_link_libraries(${TARGET}
    PUBLIC
    PRIVATE
)
