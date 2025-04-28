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

 set(CMAKE_CXX_STANDARD 20)
 set(CMAKE_CXX_EXTENSIONS OFF)

 find_package(OpenMP)
 find_package(OpenMP REQUIRED)      # przenieś tu i wymagaj OpenMP

 file(GLOB SRCS ${PROJECT_SOURCE_DIR}/src/*.cpp)

 add_executable(generator
 src/generator/main.cpp
 src/generator/cli.cpp
 src/generator/runner.cpp
 src/generator/instgen.cpp
 src/generator/io.cpp
 )

 target_include_directories(generator PRIVATE
         ${PROJECT_SOURCE_DIR}/include
         find_package(OpenMP REQUIRED)
         target_link_libraries(generator PRIVATE OpenMP::OpenMP_CXX)
 )
 target_include_directories(generator
     PRIVATE
       ${PROJECT_SOURCE_DIR}/include
 )

 target_link_libraries(generator
     PRIVATE
       OpenMP::OpenMP_CXX
 )
 set_target_properties(generator PROPERTIES
 RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

 enable_testing()
 #add_subdirectory(test)
