# Add source files
file(GLOB_RECURSE SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")

# Create library target
set(STATLOG_TARGETS "")
if (STATLOG_BUILD_SHARED)
	add_library(statlog SHARED ${SOURCE_FILES})
	list(APPEND STATLOG_TARGETS statlog)
	target_compile_definitions(statlog PUBLIC STATLOG_SHARED)
	target_compile_definitions(statlog PRIVATE STATLOG_SHARED_EXPORT)
endif()

if (STATLOG_BUILD_STATIC)
	add_library(statlog-static STATIC ${SOURCE_FILES})
	list(APPEND STATLOG_TARGETS statlog-static)
	target_compile_definitions(statlog-static PUBLIC STATLOG_STATIC)
endif()

if (STATLOG_MAIN_PROJECT)
	add_executable(statlog-example ${SOURCE_FILES})
	list(APPEND STATLOG_TARGETS statlog-example)
endif()

# ----------------------------- Compiler Options -----------------------------
foreach (target ${STATLOG_TARGETS})
	# Set C++ standard
	target_compile_features(${target} PRIVATE cxx_std_23)

	# Set warnings
	if (MSVC) # MSVC compiler settings
		target_compile_options(${target} PRIVATE /W4 /WX)
	else() # GNU or Clang compiler settings
		target_compile_options(${target} PRIVATE -Wall -Wextra -pedantic -Werror)
	endif()

	# Disable exceptions and RTTI
	if(MSVC) # MSVC compiler settings
		target_compile_options(${target} PRIVATE /EHs-c- /GR-)
		target_compile_definitions(${target} PRIVATE _HAS_EXCEPTIONS=0)
	else() # GNU or Clang compiler settings
		target_compile_options(${target} PRIVATE -fno-exceptions -fno-rtti)
		target_compile_definitions(${target} PRIVATE _LIBCPP_NO_EXCEPTIONS)
	endif()

	# Add include directory
	target_include_directories(${target} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
endforeach()
# --------------------------------------------------------------------------

# ----------------------------- Link Libraries -----------------------------
foreach (target ${STATLOG_TARGETS})
endforeach()
# --------------------------------------------------------------------------
