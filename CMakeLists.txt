cmake_minimum_required(VERSION 3.28)

project(
    Basis
    LANGUAGES CXX
)

option(
    BLOCH_WARNINGS_AS_ERRORS
    "Treat compiler warnings as errors"
    OFF
)

add_library(bloch_project_warnings INTERFACE)
add_library(bloch::project_warnings ALIAS bloch_project_warnings)

target_compile_options(
    bloch_project_warnings
    INTERFACE
        "$<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wall;-Wextra;-Wpedantic>"
        "$<$<CXX_COMPILER_ID:MSVC>:/W4;/permissive->"
)

if(BLOCH_WARNINGS_AS_ERRORS)
    target_compile_options(
        bloch_project_warnings
        INTERFACE
            "$<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Werror>"
            "$<$<CXX_COMPILER_ID:MSVC>:/WX>"
    )
endif()

file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/version.txt" BASIS_VERSION LIMIT_COUNT 1)
find_package(Git QUIET)
set(BASIS_COMMIT "unknown")
if(GIT_FOUND)
    execute_process(
        COMMAND "${GIT_EXECUTABLE}" rev-parse --short HEAD
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        OUTPUT_VARIABLE BASIS_COMMIT
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if(NOT BASIS_COMMIT)
        set(BASIS_COMMIT "unknown")
    endif()
endif()

add_subdirectory(src)

include(CTest)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()