cmake_minimum_required(VERSION 3.8...3.19)

set(perfsources
  benchmark.cpp
  parse_file.cpp
  pretty_print_file.cpp
  print_double.cpp
  print_file.cpp
  sizes.cpp
  syntax_only.cpp
)

# file(GLOB ...) is used to validate the above list of perf_sources
file(GLOB glob_perf_sources RELATIVE ${CMAKE_CURRENT_LIST_DIR} *.cpp)

foreach(perfsourcefile ${perfsources})
  if(${perfsourcefile} IN_LIST glob_perf_sources)
    list(REMOVE_ITEM glob_perf_sources ${perfsourcefile})
  else()
    message(SEND_ERROR "File ${perfsourcefile} is missing from src/perf/json")
  endif()
  get_filename_component(exename ${perfsourcefile} NAME_WE)
  set(exename "tao-json-perf-${exename}")
  add_executable(${exename} ${perfsourcefile})
  target_link_libraries(${exename} PRIVATE taocpp::json)
  set_target_properties(${exename} PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
  )
  if(MSVC)
    target_compile_options(${exename} PRIVATE /W4 /WX /utf-8)
  else()
    target_compile_options(${exename} PRIVATE -pedantic -Wall -Wextra -Wshadow -Werror)
  endif()
endforeach()

if(glob_perf_sources)
  foreach(ignored_source_file ${glob_perf_sources})
    message(SEND_ERROR "File ${ignored_source_file} in src/perf/json is ignored")
  endforeach()
endif()
