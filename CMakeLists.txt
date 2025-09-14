cmake_minimum_required(VERSION 3.20)
project(SgSimlCompiler LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCE_FILES "src/*.cpp" "src/*.h")

add_library(SgSimlCompiler STATIC ${SOURCE_FILES})

target_include_directories(SgSimlCompiler PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
#set_target_properties(SgSimlCompiler PROPERTIES
#    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
#)

enable_testing()

add_executable(SgSimlCompilerTests tests/main.cpp)

target_link_libraries(SgSimlCompilerTests PRIVATE SgSimlCompiler)

target_include_directories(SgSimlCompilerTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_test(NAME SgSimlCompilerTests COMMAND SgSimlCompilerTests)

# set_target_properties(SgSimlCompilerTests PROPERTIES
#    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
#)