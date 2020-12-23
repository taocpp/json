option(ENABLE_CLANG_TIDY "Run clang-tidy automatically as prebuild step" ON)
find_program(CLANG_TIDY
    NAMES clang-tidy clang-tidy-7 clang-tidy-8 clang-tidy-9
        clang-tidy-10 clang-tidy-11 clang-tidy-12
    HINTS /usr/local/opt/llvm/bin/ /usr/local/bin /usr/bin
)
if(CLANG_TIDY)
    if(ENABLE_CLANG_TIDY)
        message(STATUS "set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY})")
        set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY_CMD} CACHE STRING "forced!" FORCE)
    else()
        set(CMAKE_CXX_CLANG_TIDY "" CACHE STRING "forced!" FORCE) # delete it
    endif()
endif()


