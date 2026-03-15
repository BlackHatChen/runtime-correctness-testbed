cmake_minimum_required(VERSION 3.28)

project(runtime-correctness-testbed VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)              # Require C++17 features.
set(CMAKE_CXX_STANDARD_REQUIRED ON)     # Force compiler to do C++17.
set(CMAKE_CXX_EXTENSIONS OFF)           # Ensure cross-platform portability.

# Generate compile_commands.json for IDEs and tooling.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(CTest)
enable_testing()    # Enable testing (CTest) workflows.

include(FetchContent)   # Allow fetching third-party projects.

# ------------------------------------------------------------
# Third-party dependencies (GoogleTest and Google Benchmark)
# ------------------------------------------------------------

FetchContent_Declare(   # Declare where to fetch GoogleTest.
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
if(MSVC)
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

FetchContent_Declare(   # Declare where to fetch Google Benchmark.
    googlebenchmark
    URL https://github.com/google/benchmark/archive/refs/tags/v1.8.3.zip
)
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)   # Disable built-in tests.
set(BENCHMARK_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)   # Just use the tool, not install it.

FetchContent_MakeAvailable(googletest googlebenchmark)

# ------------------------------------------------------------
# Project-local warning policy
# ------------------------------------------------------------

# Create an interface library to carry compile warning flags.
add_library(rct_project_warnings INTERFACE)
target_compile_options(rct_project_warnings
    INTERFACE
    $<$<CXX_COMPILER_ID:MSVC>: /W4 /WX>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>: -Wall -Wextra -Werror -Wpedantic>
)

# ------------------------------------------------------------
# Subdirectories
# ------------------------------------------------------------

add_subdirectory(scenarios)
add_subdirectory(tests)
add_subdirectory(benchmarks)