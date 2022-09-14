set(EXAMPLE_SRC example.cpp)
set(EXAMPLE_TARGET example)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(${EXAMPLE_TARGET} ${EXAMPLE_SRC})