cmake_minimum_required (VERSION 3.3.0 FATAL_ERROR)

set (examplesources
  config.cpp
  validate_event_order.cpp
  validate_integer.cpp
)

# file (GLOB ...) is used to validate the above list of example_sources
file (GLOB glob_example_sources RELATIVE ${CMAKE_CURRENT_LIST_DIR} *.cpp)

foreach (examplesourcefile ${examplesources})
  if (${examplesourcefile} IN_LIST glob_example_sources)
    list (REMOVE_ITEM glob_example_sources ${examplesourcefile})
  else ()
    message (SEND_ERROR "File ${examplesourcefile} is missing from src/example/json")
  endif ()
  get_filename_component (exename ${examplesourcefile} NAME_WE)
  add_executable (${exename} ${examplesourcefile})
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
endforeach (examplesourcefile)

if (glob_example_sources)
  foreach (ignored_source_file ${glob_example_sources})
    message (SEND_ERROR "File ${ignored_source_file} in src/example/json is ignored")
  endforeach (ignored_source_file)
endif ()
