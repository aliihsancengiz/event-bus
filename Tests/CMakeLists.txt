file(GLOB TEST_SRC *.cpp)

find_package(GTest REQUIRED)

add_executable(run_tests ${TEST_SRC})
include_directories(${PROJECT_SOURCE_DIR})

target_link_libraries(run_tests
	-pthread
	${GTEST_LIBRARIES}
	${GTEST_MAIN_LIBRARIES}
)