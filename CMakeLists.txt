
project(igloo)
cmake_minimum_required(VERSION 3.0)

# Options
option(BUILD_TESTS "Build tests" OFF)

# --------------
# Default settings
# --------------

# Default build type
if( NOT CMAKE_BUILD_TYPE )
	set( CMAKE_BUILD_TYPE "Debug" CACHE STRING "" FORCE )
endif()

# Default install destination
if( CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT )
	set( CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/_install CACHE PATH "Installation Destination" FORCE )
endif()

# Module path
set (CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cotire/CMake")

# --------------
# COTIRE module
# --------------
include(cotire)
if( NOT COTIRE_MAXIMUM_NUMBER_OF_UNITY_INCLUDES )
	include( ProcessorCount )
	ProcessorCount( N )

	if( NOT N EQUAL 0 )
		# set number of parallel unity builds to processor count
		set( COTIRE_MAXIMUM_NUMBER_OF_UNITY_INCLUDES "-j${N}" )
	endif()
endif()

# --------------
# Build settings - compilation flags
# --------------
if (MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std=c++11 /DWIN32 /D_WINDOWS /GR /EHsc /W4 /MP /nologo")
else()
	# Assume GCC-style arguments
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions -std=c++11 -Wfatal-errors -Wall -W -Werror -Wfloat-equal -Wundef -Wendif-labels -Wshadow -pedantic-errors")
endif()


# --------------
# Build tests
# --------------
if(BUILD_TESTS)
	include_directories("${PROJECT_SOURCE_DIR}")

	file(GLOB IglooTestSourceFiles tests/*.cpp tests/**/*.cpp)
	add_executable(igloo-tests ${IglooTestSourceFiles})
	set_target_properties(igloo-tests PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT "tests/igloo_self_test.h")

	cotire(igloo-tests)

	file(GLOB SnowhouseSpecSourceFiles igloo/external/snowhouse/example/*.cpp)
	add_executable(snowhouse-tests ${SnowhouseSpecSourceFiles})
	set_target_properties(snowhouse-tests PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT "tests/igloo_self_test.h")
	target_include_directories(snowhouse-tests PUBLIC igloo/external/snowhouse)

	cotire(snowhouse-tests)

	enable_testing()
	add_test(igloo-tests igloo-tests)
	add_test(snowhouse-tests snowhouse-tests)
endif()

# --------------
# Install
# --------------

# Install destinations
set( INSTALL_BIN bin CACHE PATH "Where to install binaries to." )
set( INSTALL_LIB lib CACHE PATH "Where to install lib to." )
set( INSTALL_INC include CACHE PATH "Directory for library headers." )
set( INSTALL_CONF ${INSTALL_LIB}/cmake/Igloo CACHE PATH "Where to install package conf.")

set( INSTALL_DATA share/igloo CACHE PATH "Directory for shared data." )
set( INSTALL_DOC ${INSTALL_DATA}/doc CACHE PATH "Recommended directory to install documentation into." )
set( INSTALL_TEST ${INSTALL_DATA}/test CACHE PATH "Test files." )

# Install igloo library
add_library(igloo INTERFACE)
target_include_directories( igloo INTERFACE "$<INSTALL_INTERFACE:${INSTALL_INC}>" )

include(CMakePackageConfigHelpers)
set(igloo_config "${CMAKE_CURRENT_BINARY_DIR}/IglooConfig.cmake")
configure_package_config_file(
	"IglooConfig.cmake.in"
	"${igloo_config}"
	INSTALL_DESTINATION ${INSTALL_CONF}
	PATH_VARS INSTALL_CONF
	)

install(TARGETS igloo DESTINATION ${INSTALL_LIB} EXPORT IglooTargets)
install(EXPORT IglooTargets DESTINATION ${INSTALL_CONF})
install(DIRECTORY igloo DESTINATION ${INSTALL_INC}
	FILES_MATCHING PATTERN "*.h"
	PATTERN "igloo/external/snowhouse/example" EXCLUDE)
install(FILES ${igloo_config} DESTINATION ${INSTALL_CONF})

# Install docs
install(FILES README.md LICENSE_1_0.txt DESTINATION ${INSTALL_DOC})

# Install tests
install( TARGETS igloo-tests snowhouse-tests DESTINATION ${INSTALL_TEST} )

	# add custom install_unity target
	if( COMMAND cotire )
		add_custom_target( install_unity
			COMMAND ${CMAKE_COMMAND} -P "cmake_install.cmake" )
		add_dependencies( install_unity igloo-tests_unity snowhouse-tests_unity)
	endif()
