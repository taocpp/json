cmake_minimum_required (VERSION 3.8.0 FATAL_ERROR)

project (taocpp-json VERSION 1.0.0 LANGUAGES CXX)

# installation directories
set (TAOCPP_JSON_INSTALL_INCLUDE_DIR "include" CACHE STRING "The installation include directory")
set (TAOCPP_JSON_INSTALL_DOC_DIR "share/doc/tao/json" CACHE STRING "The installation doc directory")
set (TAOCPP_JSON_INSTALL_CMAKE_DIR "share/taocpp-json/cmake" CACHE STRING "The installation cmake directory")

# define a header-only library
add_library (json INTERFACE)
add_library (taocpp::json ALIAS json)
target_include_directories (json INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${TAOCPP_JSON_INSTALL_INCLUDE_DIR}>
)

# require C++17
target_compile_features (json INTERFACE cxx_std_17)

option(TAOCPP_JSON_PEGTL_USE_BOOST_FILESYSTEM "Override the auto-detection of std::filesystem and use Boost.Filesystem" OFF)

# Try compiling a test program with std::filesystem or one of its alternatives
function(check_filesystem_impl FILESYSTEM_HEADER FILESYSTEM_NAMESPACE OPTIONAL_LIBS OUT_RESULT)
  set(TEST_FILE "test_${OUT_RESULT}.cpp")
  configure_file(cmake/test_filesystem.cpp.in ${TEST_FILE} @ONLY)

  try_compile(TEST_RESULT
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}/${TEST_FILE}
    CXX_STANDARD 17)

  if(NOT TEST_RESULT)
    # Retry with each of the optional libraries
    foreach(OPTIONAL_LIB IN LISTS OPTIONAL_LIBS)
      try_compile(TEST_RESULT
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}/${TEST_FILE}
        LINK_LIBRARIES ${OPTIONAL_LIB}
        CXX_STANDARD 17)

      if(TEST_RESULT)
        # Looks like the optional library was required, go ahead and add it to the link options.
        message(STATUS "Adding ${OPTIONAL_LIB} to taoJSON to build with ${FILESYSTEM_NAMESPACE}.")
        target_link_libraries(${PROJECT_NAME} INTERFACE ${OPTIONAL_LIB})
        break()
      endif()
    endforeach(OPTIONAL_LIB)
  endif()

  set(${OUT_RESULT} ${TEST_RESULT} PARENT_SCOPE)
endfunction(check_filesystem_impl)

if(TAOCPP_JSON_PEGTL_USE_BOOST_FILESYSTEM)
  # Force the use of Boost.Filesystem: #include <boost/filesystem.hpp> // boost::filesystem
  find_package(Boost REQUIRED COMPONENTS filesystem)
  target_link_libraries(${PROJECT_NAME} INTERFACE Boost::filesystem)
  target_compile_definitions(${PROJECT_NAME} INTERFACE TAO_JSON_PEGTL_BOOST_FILESYSTEM)
else()
  # Try compiling a minimal program with each header/namespace, in order of preference:
  #   C++17: #include <filesystem> // std::filesystem
  #   Experimental C++17: #include <experimental/filesystem> // std::experimental::filesystem
  #   Boost.Filesystem: #include <boost/filesystem.hpp> // boost::filesystem
  check_filesystem_impl("filesystem" "std::filesystem" "stdc++fs;c++fs" STD_FILESYSTEM)
  if(STD_FILESYSTEM)
    message(STATUS "Using std::filesystem")
  else()
    check_filesystem_impl("experimental/filesystem" "std::experimental::filesystem" "stdc++fs;c++fs" STD_EXPERIMENTAL_FILESYSTEM)
    if(STD_EXPERIMENTAL_FILESYSTEM)
      target_compile_definitions(${PROJECT_NAME} INTERFACE TAO_JSON_PEGTL_STD_EXPERIMENTAL_FILESYSTEM)
      message(WARNING "Using std::experimental::filesystem as a fallback")
    else()
      find_package(Boost COMPONENTS filesystem)
      check_filesystem_impl("boost/filesystem.hpp" "boost::filesystem" Boost::filesystem BOOST_FILESYSTEM)
      if(BOOST_FILESYSTEM)
        target_compile_definitions(${PROJECT_NAME} INTERFACE TAO_JSON_PEGTL_BOOST_FILESYSTEM)
        message(WARNING "Using Boost.Filesystem as a fallback")
      else()
        message(FATAL_ERROR "taoJSON requires C++17, including an implementation of std::filesystem, which your compiler toolchain does not seem to support. You can try installing Boost.Filesystem as a temporary workaround, but there's no guarantee taoJSON will keep working with your project. Consider upgrading your compiler toolchain or downgrading taoJSON to the previous version.")
      endif()
    endif()
  endif()
endif()

# testing
enable_testing ()
option (TAOCPP_JSON_BUILD_TESTS "Build test programs" ON)
if (TAOCPP_JSON_BUILD_TESTS)
  add_subdirectory (src/test/json)
endif ()

# examples
option (TAOCPP_JSON_BUILD_EXAMPLES "Build example programs" ON)
if (TAOCPP_JSON_BUILD_EXAMPLES)
  add_subdirectory (src/example/json)
endif ()

# install and export target
install (TARGETS json EXPORT taocpp-json-targets)

install (EXPORT taocpp-json-targets
  FILE taocpp-json-config.cmake
  NAMESPACE taocpp::
  DESTINATION ${TAOCPP_JSON_INSTALL_CMAKE_DIR}
)

install (DIRECTORY include/ DESTINATION ${TAOCPP_JSON_INSTALL_INCLUDE_DIR})
install (FILES LICENSE DESTINATION ${TAOCPP_JSON_INSTALL_DOC_DIR})
