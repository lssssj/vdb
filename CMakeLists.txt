cmake_minimum_required(VERSION 3.10)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # For clang-tidy.
set(BUILD_SHARED_LIBS OFF) # We expect external libraries to be linked statically.
set(CMAKE_CXX_STANDARD 17) # Compile as C++17.
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Require C++17 support.

project(VDB
        VERSION 2023.1
        DESCRIPTION "The CPP Vector Database Management System"
        LANGUAGES C CXX
)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
        message(STATUS "Setting build type to `Debug` as none was specified.")
        set(CMAKE_BUILD_TYPE "Debug")
endif()



# 自动识别Apple Silicon
if(APPLE AND CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "arm64")
    # 设置专用优化参数
    string(REGEX REPLACE "-march=[^ ]*" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    string(REGEX REPLACE "-march=[^ ]*" "" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
    message(STATUS "hello")
    add_compile_options("-mcpu=apple-m1")
endif()

# Output directory.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Includes.
set(VDB_SRC_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/src)

set(VDB_THIRD_PARTY_INCLUDE_DIR
        ${PROJECT_SOURCE_DIR}/third_party
        ${PROJECT_SOURCE_DIR}/third_party/cpp-httplib-0.18.1
        ${PROJECT_SOURCE_DIR}/third_party/faiss-1.9.0
        ${PROJECT_SOURCE_DIR}/third_party/spdlog-1.14.1/include
        ${PROJECT_SOURCE_DIR}/third_party/rapidjson-1.1.0/include
)

# 设置编译faiss时的环境变量
set(FAISS_ENABLE_GPU OFF CACHE BOOL  "Disable GPU support")
set(FAISS_ENABLE_PYTHON OFF CACHE BOOL  "Disable Python bindings")
set(BUILD_TESTING OFF CACHE BOOL "Disable BUILD_TESTINs")
set(RAPIDJSON_BUILD_EXAMPLES OFF CACHE BOOL "Disable RAPIDJSON_BUILD_EXAMPLES")

include_directories(${VDB_SRC_INCLUDE_DIR} ${VDB_THIRD_PARTY_INCLUDE_DIR})
include_directories(BEFORE src) # This is needed for gtest.


# Subdirectory
add_subdirectory(src)
add_subdirectory(third_party)


