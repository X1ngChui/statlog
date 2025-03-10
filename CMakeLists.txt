cmake_minimum_required(VERSION 3.10...3.21)

project(statlog LANGUAGES CXX)

option(STATLOG_BUILD_TESTS "Build tests for statlog" ON)

# Set STATLOG_MAIN_PROJECT to ON if we are building statlog
# Check if statlog is being used directly or via add_subdirectory, but allow overriding
if (NOT DEFINED STATLOG_MAIN_PROJECT)
    if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
        set(STATLOG_MAIN_PROJECT ON)
    else()
        set(STATLOG_MAIN_PROJECT OFF)
    endif()
endif()

add_subdirectory("statlog")

if (STATLOG_BUILD_TESTS)
    add_subdirectory("tests")
endif()
