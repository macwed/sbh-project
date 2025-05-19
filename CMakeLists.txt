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
Include(FetchContent)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.8.1
)
FetchContent_MakeAvailable(Catch2)

enable_testing()

add_executable(test_graph_catch test/test_graph.cpp)
target_link_libraries(test_graph_catch PRIVATE Catch2::Catch2WithMain common)
add_test(NAME GraphTests COMMAND test_graph_catch)


add_executable(test_greedy_catch test/test_greedy.cpp src/generator/io.cpp)
target_link_libraries(test_greedy_catch PRIVATE Catch2::Catch2WithMain common greedy_solver)
add_test(NAME GreedyTests COMMAND test_greedy_catch)
