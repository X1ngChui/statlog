# Create interface library
add_library(statlog INTERFACE)

# Set C++ standard
target_compile_features(statlog INTERFACE cxx_std_23)

# Add include directory
target_include_directories(statlog INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")