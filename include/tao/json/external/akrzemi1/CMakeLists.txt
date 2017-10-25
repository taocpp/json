# if CMAKE_VERSION >= 3.0 this project installs an INTERFACE target for
# the optional.hpp file.
#
# Usage:
#
# In your project's CMakeLists.txt:
#
#     find_package(akrzemi1_optional REQUIRED)
#     ...
#     target_link_libraries(mytarget ... akrzemi1::optional ...)
#
# In your C++ source file:
#
#     #include "akrzemi1/optional.hpp"
#

project(optional)
cmake_minimum_required(VERSION 2.8)
enable_testing()

if(CMAKE_VERSION VERSION_LESS 3.1)
    set(CMAKE_CXX_FLAGS "-std=c++11")
elseif(NOT CMAKE_CXX_STANDARD) # don't override c++ standard if already set
    set(CMAKE_CXX_STANDARD 11)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

# if CMAKE_VERSION >= 3.0
if(NOT (CMAKE_VERSION VERSION_LESS 3.0))
    add_library(optional INTERFACE)
    target_include_directories(optional INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>)
    install(TARGETS optional EXPORT optional-targets)
    install(EXPORT optional-targets DESTINATION lib/cmake/akrzemi1_optional
        FILE akrzemi1_optional-config.cmake
        NAMESPACE akrzemi1::)
    install(FILES optional.hpp DESTINATION include/akrzemi1)
endif()

add_executable(test_optional test_optional.cpp)
add_executable(test_type_traits test_type_traits.cpp)

add_test(test_optional test_optional)
add_test(test_type_traits test_type_traits)
