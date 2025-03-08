file(GLOB_RECURSE SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
add_executable(statlog_tests ${SOURCE_FILES})

include_directories(${CMAKE_SOURCE_DIR}/vendors/doctest)
target_link_libraries(statlog_tests PRIVATE statlog)

enable_testing()
add_test(NAME StatlogTests COMMAND statlog_tests)
