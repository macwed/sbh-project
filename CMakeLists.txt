cmake_minimum_required(VERSION 3.10)
project(sbh-project
        VERSION 2.0.0
        LANGUAGES CXX
)

if(MSVC)
 add_compile_options(/W4 /permissive-)
else()
 add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(CMAKE_CXX_STANDARD       20)
set(CMAKE_CXX_EXTENSIONS    OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenMP REQUIRED)

add_library(common
        src/common/graph.cpp
        src/common/dijkstra.cpp
)
target_include_directories(common
        PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)
# target_link_libraries(common PUBLIC OpenMP::OpenMP_CXX)

# —————— Greedy Solver ——————
add_library(greedy_solver STATIC
        src/greedy-solver/greedy.cpp
)
target_include_directories(greedy_solver PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)
target_link_libraries(greedy_solver PUBLIC common)

# —————— Generator ——————
add_executable(generator
        src/generator/main.cpp
        src/generator/cli.cpp
        src/generator/runner.cpp
        src/generator/instgen.cpp
        src/generator/io.cpp
)
target_include_directories(generator
        PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)
target_link_libraries(generator
        PRIVATE
        common
        OpenMP::OpenMP_CXX
        # cxxopts::cxxopts
)
set_target_properties(generator PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin
)

# —————— Tests ——————
enable_testing()

add_executable(test_graph
        test/test_graph.cpp
        src/common/graph.cpp
)
target_include_directories(test_graph
        PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)
add_test(NAME GraphTest COMMAND test_graph)

add_executable(test_greedy_manual
        test/test_greedy_manual.cpp
        src/generator/io.cpp
)

target_include_directories(test_greedy_manual PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(test_greedy_manual PRIVATE
        common
        greedy_solver
)

add_test(NAME GreedyTest COMMAND test_greedy_manual)
