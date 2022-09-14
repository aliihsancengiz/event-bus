cmake_minimum_required(VERSION 3.16)

set(PROJECT_VERSION 1.0.0)
set(TARGET_NAME "Event Bus")

project(${TARGET_NAME}
        VERSION ${PROJECT_VERSION}
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(EVENTBUS_LIB_INC
	include/EventBus/EventBus.hpp
)

if (BUILD_TEST)
	add_subdirectory(tests)
endif(BUILD_TEST)

if (BUILD_EXAMPLE)
	add_subdirectory(example)
endif(BUILD_EXAMPLE)

include(GNUInstallDirs)

foreach ( file ${EVENTBUS_LIB_INC} )
    get_filename_component( dir ${file} DIRECTORY )
    install( FILES ${file} DESTINATION "${CMAKE_INSTALL_PREFIX}/${dir}")
endforeach()

