cmake_minimum_required(VERSION 4.1)
project(SgSimlCompiler LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCE_FILES "src/*.cpp" "src/*.h")

add_library(SgSimlCompiler STATIC ${SOURCE_FILES})

target_include_directories(SgSimlCompiler PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
