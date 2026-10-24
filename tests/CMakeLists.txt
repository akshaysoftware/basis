set(
    BLOCH_TEST_SOURCES
    test_main.cpp
    test_ast.cpp
    test_lexer.cpp
    test_parser.cpp
    test_semantics.cpp
    test_runtime.cpp
)

if(NOT WIN32)
    list(APPEND BLOCH_TEST_SOURCES test_integration.cpp)
endif()

add_executable(
    basis_tests
    ${BLOCH_TEST_SOURCES}
)

target_compile_features(
    basis_tests
    PRIVATE
        cxx_std_20
)

target_link_libraries(
    basis_tests
    PRIVATE
        bloch::runtime
        bloch::project_warnings
)

set_target_properties(
    basis_tests
    PROPERTIES
        CXX_EXTENSIONS OFF
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

add_test(
    NAME basis.tests
    COMMAND basis_tests
)