add_executable(gate_contract_tests gate_contract_tests.cpp)

target_compile_features(gate_contract_tests PRIVATE cxx_std_17)

target_include_directories(gate_contract_tests PRIVATE ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(gate_contract_tests
    PRIVATE 
    rct_project_warnings
    GTest::gtest_main   # GoogleTest entry point.
)

# Import GoogleTest helper to auto discover TEST() and register them with CTest.
include(GoogleTest)
gtest_discover_tests(gate_contract_tests)