cmake_minimum_required(VERSION 2.8)

project(igloo)


option(BUILD_TESTS "Build tests" OFF)
set (CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cotire/CMake")
include(cotire)

include_directories("${PROJECT_SOURCE_DIR}")

if (MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /MP ")
else()
  # Assume GCC-style arguments
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions -Wfatal-errors -Wall -W -Werror -Wfloat-equal -Wundef -Wendif-labels -Wshadow -pedantic-errors")
endif()

if(BUILD_TESTS)
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
endif()

# Install config
set(CONF_DEST lib/cmake/Igloo)
set(INCL_DEST include)
set(LIB_DEST lib)

add_library(igloo INTERFACE)
target_include_directories(
    igloo INTERFACE "$<INSTALL_INTERFACE:${INCL_DEST}>"
)

include(CMakePackageConfigHelpers)
set(igloo_config "${CMAKE_CURRENT_BINARY_DIR}/IglooConfig.cmake")
configure_package_config_file(
    "IglooConfig.cmake.in"
    "${igloo_config}"
    INSTALL_DESTINATION ${CONF_DEST}
    PATH_VARS CONF_DEST
)

install(TARGETS igloo DESTINATION ${LIB_DEST} EXPORT IglooTargets)
install(EXPORT IglooTargets DESTINATION ${CONF_DEST})
install(DIRECTORY igloo DESTINATION ${INCL_DEST} FILES_MATCHING PATTERN "*.h")
install(FILES ${igloo_config} DESTINATION ${CONF_DEST})
