cmake_minimum_required (VERSION 3.2.0 FATAL_ERROR)

# set project and version
project (taocpp-json VERSION 1.0.0 LANGUAGES CXX)

# define a header-only library
add_library (taocpp-json INTERFACE)
add_library (taocpp::json ALIAS taocpp-json)
target_include_directories (taocpp-json INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

# features used by taocpp/json
target_compile_features (taocpp-json INTERFACE
  cxx_alias_templates
  cxx_auto_type
  cxx_constexpr
  cxx_decltype
  cxx_default_function_template_args
  cxx_defaulted_functions
  cxx_delegating_constructors
  cxx_deleted_functions
  cxx_explicit_conversions
  cxx_generalized_initializers
  cxx_inheriting_constructors
  cxx_inline_namespaces
  cxx_noexcept
  cxx_nonstatic_member_init
  cxx_nullptr
  cxx_range_for
  cxx_rvalue_references
  cxx_static_assert
  cxx_strong_enums
  cxx_template_template_parameters
  cxx_trailing_return_types
  cxx_uniform_initialization
  cxx_variadic_macros
  cxx_variadic_templates
)

# testing
enable_testing ()
option (TAOCPP_JSON_BUILD_TESTS "Build test programs" ON)
if (TAOCPP_JSON_BUILD_TESTS)
  add_subdirectory (src/test/json)
endif ()

# installation directories
set (TAOCPP_JSON_INSTALL_INCLUDE_DIR "include" CACHE STRING "The installation include directory")
set (TAOCPP_JSON_INSTALL_DOC_DIR "share/doc/tao/json" CACHE STRING "The installation doc directory")

# install
install (DIRECTORY include/ DESTINATION ${TAOCPP_JSON_INSTALL_INCLUDE_DIR})
install (FILES LICENSE DESTINATION ${TAOCPP_JSON_INSTALL_DOC_DIR})
