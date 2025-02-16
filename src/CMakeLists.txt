# 定义可执行文件
add_library(
    index
    OBJECT
    faiss_index.cpp
    http_server.cpp
    index_factory.cpp
    logger.cpp
)

add_executable(main vdb_server.cpp)


# 链接第三方库（关键步骤）
target_link_libraries(main PRIVATE index httplib faiss spdlog)

# 包含头文件路径（若库未自动导出）
target_include_directories(main PRIVATE
    ${CMAKE_SOURCE_DIR}/third_party/cpp-httplib-0.18.1
    ${CMAKE_SOURCE_DIR}/third_party/faiss-1.9.0
    ${CMAKE_SOURCE_DIR}/third_party/rapidjson-1.1.0 
    ${CMAKE_SOURCE_DIR}/third_party/spdlog-1.14.1
)

# 重新设置二进制名字main为vdb-server
set_target_properties(main PROPERTIES OUTPUT_NAME vdb-server)