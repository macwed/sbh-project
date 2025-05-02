cmake_minimum_required(VERSION 3.10)

project(sbh_project
        VERSION 2.0.0
        LANGUAGES CXX
)

if(MSVC)
 add_compile_options(/W4 /permissive-)
else()
 add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenMP REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(generator
        src/generator/main.cpp
        src/generator/cli.cpp
        src/generator/runner.cpp
        src/generator/instgen.cpp
        src/generator/io.cpp
)

target_link_libraries(generator
        PRIVATE
        OpenMP::OpenMP_CXX
        # cxxopts::cxxopts
)

set_target_properties(generator PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin
)

add_executable(greedy_solver
        src/solver/graph.cpp
        src/solver/dijkstra.cpp
        src/greedy/greedy.cpp
)

target_link_libraries(greedy_solver
        PRIVATE
        OpenMP::OpenMP_CXX
)

set_target_properties(greedy_solver PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin
)

#(meta_solver etc.)

enable_testing()
# add_subdirectory(test)
