cmake_minimum_required(VERSION 3.20)
project(SgSimlCompiler LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCE_FILES "src/*.cpp" "src/*.h")

add_library(SgSimlCompiler STATIC ${SOURCE_FILES})

target_include_directories(SgSimlCompiler PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)


enable_testing()

add_executable(SgSimlCompilerTests tests/main.cpp)

target_link_libraries(SgSimlCompilerTests PRIVATE SgSimlCompiler)

target_include_directories(SgSimlCompilerTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_test(NAME SgSimlCompilerTests COMMAND SgSimlCompilerTests)