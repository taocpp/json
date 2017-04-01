cmake_minimum_required (VERSION 3.2.0 FATAL_ERROR)

# set project and version
project (taocpp-json VERSION 1.0.0 LANGUAGES CXX)

# set C++ language standard
set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

# define a header-only library
add_library (taocpp-json INTERFACE)
target_include_directories (taocpp-json INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

# testing
enable_testing ()
add_subdirectory (src/test/json)

# install
install (DIRECTORY include/ DESTINATION include)
install (FILES LICENSE DESTINATION include/tao/json)
