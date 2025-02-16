
add_subdirectory(cpp-httplib-0.18.1)

# 在包含faiss子目录前设置参数

add_subdirectory(faiss-1.9.0)

add_subdirectory(rapidjson-1.1.0)

add_subdirectory(spdlog-1.14.1)


# 获取 RapidJSON 的所有编译目标
get_directory_property(rapidjson_targets DIRECTORY rapidjson-1.1.0 DEFINED_TARGETS)

# 对每个目标禁用特定警告
foreach(target IN LISTS rapidjson_targets)
    target_compile_options(${target} PRIVATE
        -Wno-zero-as-null-pointer-constant    # 忽略警告
        -Wno-error=zero-as-null-pointer-constant  # 禁止将此警告视为错误
    )
endforeach()