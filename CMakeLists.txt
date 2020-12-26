cmake_minimum_required(VERSION 3.8.0 FATAL_ERROR)

project(taocpp-json VERSION 1.0.0 LANGUAGES CXX)

# installation directories
set(TAOCPP_JSON_INSTALL_INCLUDE_DIR "include" CACHE STRING "The installation include directory")
set(TAOCPP_JSON_INSTALL_DOC_DIR "share/doc/tao/json" CACHE STRING "The installation doc directory")
set(TAOCPP_JSON_INSTALL_CMAKE_DIR "share/taocpp-json/cmake" CACHE STRING "The installation cmake directory")

# define a header-only library
add_library(json INTERFACE)
add_library(taocpp::json ALIAS json)
target_include_directories(json INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${TAOCPP_JSON_INSTALL_INCLUDE_DIR}>
)

# require C++17
target_compile_features(json INTERFACE cxx_std_17)

# find a suitable PEGTL
find_package(pegtl 3.1.0 QUIET CONFIG)
if(NOT pegtl_FOUND)
  # if a compatible version of PEGTL is not already installed, build and install it from the submodule directory
  message(STATUS "Adding PEGTL as submodule from external/PEGTL.")
  set(PEGTL_BUILD_TESTS OFF CACHE BOOL "Disable PEGTL tests")
  set(PEGTL_BUILD_EXAMPLES OFF CACHE BOOL "Disable PEGTL examples")
  set(PEGTL_INSTALL_INCLUDE_DIR ${TAOCPP_JSON_INSTALL_INCLUDE_DIR} CACHE STRING "Override PEGTL include install directory")
  set(PEGTL_INSTALL_CMAKE_DIR ${TAOCPP_JSON_INSTALL_CMAKE_DIR}/pegtl CACHE STRING "Override PEGTL cmake install directory")
  add_subdirectory(external/PEGTL)
endif()

# add pegtl as a dependency
target_link_libraries(json INTERFACE taocpp::pegtl)

# testing
option(TAOCPP_JSON_BUILD_TESTS "Build test programs" ON)
if(TAOCPP_JSON_BUILD_TESTS)
  enable_testing()
  add_subdirectory(src/test/json)
endif()

# examples
option(TAOCPP_JSON_BUILD_EXAMPLES "Build example programs" ON)
if(TAOCPP_JSON_BUILD_EXAMPLES)
  add_subdirectory(src/example/json)
endif()

# Make package findable
configure_file(.cmake/taocpp-json-config.cmake.in taocpp-json-config.cmake @ONLY)

# Ignore pointer width differences since this is a header-only library
unset(CMAKE_SIZEOF_VOID_P)

# install and export target
install(TARGETS json EXPORT taocpp-json-targets)

install(EXPORT taocpp-json-targets
  FILE taocpp-json-config.cmake
  NAMESPACE taocpp::
  DESTINATION ${TAOCPP_JSON_INSTALL_CMAKE_DIR}
)

install(DIRECTORY include/ DESTINATION ${TAOCPP_JSON_INSTALL_INCLUDE_DIR})
install(FILES LICENSE DESTINATION ${TAOCPP_JSON_INSTALL_DOC_DIR})
