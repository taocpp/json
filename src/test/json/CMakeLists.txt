cmake_minimum_required (VERSION 3.3.0 FATAL_ERROR)

set(testsources
  access.cpp
  big_list_of_naughty_strings.cpp
  binding_array.cpp
  binding_factory.cpp
  binding_object.cpp
  binding_versions.cpp
  cbor.cpp
  compare.cpp
  composite_traits.cpp
  contrib_get.cpp
  contrib_reference.cpp
  contrib_schema.cpp
  create.cpp
  double.cpp
  dump_sizes.cpp
  escape.cpp
  events_compare.cpp
  events_debug.cpp
  events_hash.cpp
  events_to_stream.cpp
  events_to_string.cpp
  include_json.cpp
  index.cpp
  integer.cpp
  json_parts_parser.cpp
  json_patch.cpp
  json_pointer.cpp
  key_camel_case_to_snake_case.cpp
  key_snake_case_to_camel_case.cpp
  literal.cpp
  msgpack.cpp
  object_construction.cpp
  object_operators.cpp
  opaque.cpp
  ostream.cpp
  ostream_jaxn.cpp
  parse.cpp
  parse_jaxn.cpp
  pointer.cpp
  position.cpp
  self_contained.cpp
  sha256.cpp
  string_view.cpp
  temporary_parsing.cpp
  traits.cpp
  type.cpp
  ubjson.cpp
  uri_fragment.cpp
  user.cpp
  validate_event_interfaces.cpp
)

# file (GLOB ...) is used to validate the above list of test_sources
file (GLOB glob_test_sources RELATIVE ${CMAKE_CURRENT_LIST_DIR} *.cpp)

foreach (testsourcefile ${testsources})
  if (${testsourcefile} IN_LIST glob_test_sources)
    list (REMOVE_ITEM glob_test_sources ${testsourcefile})
  else ()
    message (SEND_ERROR "File ${testsourcefile} is missing from src/test/json")
  endif ()
  get_filename_component (exename ${testsourcefile} NAME_WE)
  set (exename "tao-json-test-${exename}")
  add_executable (${exename} ${testsourcefile})
  target_link_libraries (${exename} PRIVATE taocpp::json)
  set_target_properties (${exename} PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
  )
  if (MSVC)
    target_compile_options (${exename} PRIVATE /W4 /WX /utf-8)
  else ()
    target_compile_options (${exename} PRIVATE -pedantic -Wall -Wextra -Wshadow -Werror)
  endif ()
  add_test (NAME ${exename} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../../.. COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${exename})
endforeach (testsourcefile)

if (glob_test_sources)
  foreach (ignored_source_file ${glob_test_sources})
    message (SEND_ERROR "File ${ignored_source_file} in src/test/json is ignored")
  endforeach (ignored_source_file)
endif ()
