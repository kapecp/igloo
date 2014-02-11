cmake_minimum_required(VERSION 2.8)

project(igloo)

include(cmake/cotire.cmake)

include_directories("./")

if (MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /MP ")
else()
  # Assume GCC-style arguments
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wfatal-errors -Wall -W -Werror -Wfloat-equal -Wundef -Wendif-labels -Wshadow -pedantic-errors")
endif()


FILE(GLOB IglooTestSourceFiles tests/*.cpp tests/**/*.cpp)
add_executable(igloo-tests ${IglooTestSourceFiles})
set_target_properties(igloo-tests PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT "tests/igloo_self_test.h")
set_target_properties(igloo-tests PROPERTIES COTIRE_ADD_UNIT_BUILD FALSE)
cotire(igloo-tests)

FILE(GLOB SnowhouseSpecSourceFiles igloo/external/snowhouse/spec/*.cpp)
add_executable(snowhouse-tests ${SnowhouseSpecSourceFiles})
set_target_properties(snowhouse-tests PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT "tests/igloo_self_test.h")
set_target_properties(snowhouse-tests PROPERTIES COTIRE_ADD_UNIT_BUILD FALSE)
cotire(snowhouse-tests)

enable_testing()
add_test(igloo-tests igloo-tests)
add_test(snowhouse-tests snowhouse-tests)
