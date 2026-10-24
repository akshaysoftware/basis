find_package(Threads REQUIRED)

add_library(
    bloch_compiler
    STATIC
        bloch/compiler/import/module_loader.cpp
        bloch/compiler/lexer/lexer.cpp
        bloch/compiler/parser/parser.cpp
        bloch/compiler/semantics/built_ins.cpp
        bloch/compiler/semantics/semantic_analyser.cpp
        bloch/compiler/semantics/type_system.cpp
)

add_library(bloch::compiler ALIAS bloch_compiler)

target_compile_features(
    bloch_compiler
    PUBLIC
        cxx_std_20
)

target_include_directories(
    bloch_compiler
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(
    bloch_compiler
    PRIVATE
        bloch::project_warnings
)

add_library(
    bloch_runtime
    STATIC
        bloch/runtime/qasm_simulator.cpp
        bloch/runtime/runtime_evaluator.cpp
)

add_library(bloch::runtime ALIAS bloch_runtime)

target_compile_features(
    bloch_runtime
    PUBLIC
        cxx_std_20
)

target_include_directories(
    bloch_runtime
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(
    bloch_runtime
    PUBLIC
        bloch::compiler
        Threads::Threads
    PRIVATE
        bloch::project_warnings
)

add_library(
    bloch_cli
    STATIC
        bloch/cli/cli.cpp
)

add_library(bloch::cli ALIAS bloch_cli)

target_compile_features(
    bloch_cli
    PUBLIC
        cxx_std_20
)

target_include_directories(
    bloch_cli
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(
    bloch_cli
    PUBLIC
        bloch::runtime
    PRIVATE
        bloch::project_warnings
)

add_executable(
    basis
    main.cpp
)

target_compile_features(
    basis
    PRIVATE
        cxx_std_20
)

target_link_libraries(
    basis
    PRIVATE
        bloch::cli
        bloch::project_warnings
)

set_target_properties(
    bloch_compiler
    bloch_runtime
    bloch_cli
    basis
    PROPERTIES
        CXX_EXTENSIONS OFF
)

set_target_properties(
    basis
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

target_compile_definitions(bloch_cli PUBLIC
    BLOCH_VERSION="${BASIS_VERSION}"
    BLOCH_COMMIT_HASH="${BASIS_COMMIT}"
)

install(TARGETS basis RUNTIME DESTINATION bin)
install(DIRECTORY "${PROJECT_SOURCE_DIR}/library/"
    DESTINATION "share/basis/library"
    FILES_MATCHING PATTERN "*.bloch"
)
