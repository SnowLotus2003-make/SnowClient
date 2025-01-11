cmake_minimum_required(VERSION 3.28)
include(cmake/CPM.cmake)
project(Solstice)

# 检查是否为 x86 架构并报错
if (CMAKE_SIZEOF_VOID_P EQUAL 4)
    message(FATAL_ERROR "Solstice only supports x64 builds.")
endif ()

# 定义构建类型相关的宏定义
if (PRIVATE_DEBUGBUILD)
    add_compile_definitions(__PRIVATE_DEBUG__)
    add_compile_definitions(__PRIVATE_BUILD__)
    add_compile_definitions(__DEBUG__)
    message(STATUS "Private+Debug build enabled")
endif ()

if (PRIVATE_BUILD)
    add_compile_definitions(__PRIVATE_BUILD__)
    message(STATUS "Private build enabled")
endif ()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(__PRIVATE_BUILD__)
    message(STATUS "Debug build enabled")
endif ()

# 查找 ld 命令
find_program(LD_COMMAND ld)
if (NOT LD_COMMAND)
    find_program(LD_COMMAND ld HINTS "C:/msys64/mingw64/bin")
elseif (NOT LD_COMMAND)
    message(STATUS "ld found using find_program")
endif ()

# 检查 ld 命令是否可用
execute_process(COMMAND ${LD_COMMAND} --version OUTPUT_VARIABLE LD_COMMAND_AVAILABLE)
message(STATUS "ld found at ${LD_COMMAND}")
if (NOT LD_COMMAND_AVAILABLE)
    message(FATAL_ERROR "The ld command is not available on your system. \nPlease install using `pacman -S mingw-w64-x86_64-binutils` in MSYS2, then add the bin directory to your PATH.\nAfter that, restart your IDE and CMake should be able to find the ld command.")
endif ()

set(CMAKE_UNITY_BUILD TRUE)

# 依赖项管理，修改 DirectXTK 为稳定版本，假设为 v1.6.22，需根据实际情况调整
CPMAddPackage(
        NAME spdlog
        GITHUB_REPOSITORY gabime/spdlog
        VERSION 1.9.2
)
CPMAddPackage(
        NAME magic_enum
        GITHUB_REPOSITORY Neargye/magic_enum
        VERSION 0.9.5
)
CPMAddPackage(
        NAME glm
        GITHUB_REPOSITORY g-truc/glm
        GIT_TAG 1.0.1
)
CPMAddPackage(
        NAME nlohmann_json
        GITHUB_REPOSITORY nlohmann/json
        VERSION 3.11.3
)
CPMAddPackage(
        NAME DirectXTK
        GITHUB_REPOSITORY microsoft/DirectXTK
        GIT_TAG v1.6.22
)
CPMAddPackage(
        NAME cryptopp-cmake
        GITHUB_REPOSITORY abdes/cryptopp-cmake
        GIT_TAG master
)
CPMAddPackage(
        NAME LuaBridge3
        GITHUB_REPOSITORY kunitoki/LuaBridge3
        GIT_TAG dbd132e727d0701bbfbefe83c403d8e5e640886c
)

if(MSVC)
    add_compile_options(/bigobj)
endif()

set(SHADER_DIR ${CMAKE_CURRENT_LIST_DIR}/src/Utils/Shaders)
set(SHADER_BIN_DIR ${CMAKE_BINARY_DIR}/SolsticeShaders)
set(HLSL_FILES_VS ${SHADER_DIR}/BlockInstanceVS.hlsl)
set(HLSL_FILES_PS ${SHADER_DIR}/DefaultPS.hlsl)
list(LENGTH HLSL_FILES_VS HLSL_FILES_VS_COUNT)
list(LENGTH HLSL_FILES_PS HLSL_FILES_PS_COUNT)
math(EXPR HLSL_FILES_COUNT "${HLSL_FILES_VS_COUNT} + ${HLSL_FILES_PS_COUNT}")

add_custom_target(SolsticeShaders ALL COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/SolsticeShaders COMMENT "Building ${HLSL_FILES_COUNT} shaders" VERBATIM)

foreach(FILE ${HLSL_FILES_VS})
    get_filename_component(FILE_WE ${FILE} NAME_WE)
    add_custom_command(TARGET SolsticeShaders
            COMMAND cmd /c fxc.exe /nologo /Evertex_main /T vs_5_0 /Zi /Fh ${SHADER_BIN_DIR}/${FILE_WE}.inc /Fd ${CMAKE_BINARY_DIR}/${FILE_WE}.pdb ${FILE} > nul
            DEPENDS ${FILE}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMENT "Building shader ${FILE}"
            VERBATIM
    )
endforeach(FILE)

foreach(FILE ${HLSL_FILES_PS})
    get_filename_component(FILE_WE ${FILE} NAME_WE)
    add_custom_command(TARGET SolsticeShaders
            COMMAND cmd /c fxc.exe /nologo /Epixel_main /T ps_5_0 /Zi /Fh ${SHADER_BIN_DIR}/${FILE_WE}.inc /Fd ${CMAKE_BINARY_DIR}/${FILE_WE}.pdb ${FILE} > nul
            DEPENDS ${FILE}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMENT "Building shader ${FILE}"
            VERBATIM
    )
endforeach(FILE)

include_directories(${SHADER_BIN_DIR})
find_package(OpenMP)

add_custom_target(generate_build_info
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/GenerateBuildInfo.cmake
        COMMENT "Generating build info [${CMAKE_BINARY_DIR}/build_info.h]"
)

include_directories(${CMAKE_BINARY_DIR})

set(CMAKE_CXX_STANDARD 26)
add_compile_definitions(NOMINMAX)
add_compile_definitions(JM_XORSTR_DISABLE_AVX_INTRINSICS)
add_compile_definitions(ENTT_SPARSE_PAGE=2048)
add_compile_definitions(ENTT_PACKED_PAGE=128)

set(IS_DEBUG_BUILD CMAKE_BUILD_TYPE STREQUAL "Debug")

if (${IS_DEBUG_BUILD})
        add_compile_definitions(__DEBUG__)
endif ()

# 定义函数设置 Debug 选项
function(set_debug_options)
    add_link_options($<$<CONFIG:Debug>:/INCREMENTAL>) # 启用增量链接
    add_link_options($<$<CONFIG:Debug>:/DEBUG>) # 启用调试
    add_compile_options($<$<CONFIG:Debug>:/Zi>)
endfunction()

# 定义函数设置 Release 选项
function(set_release_options)
    add_link_options($<$<CONFIG:Release>:/Zi>) # 生成 pdb
    add_link_options($<$<CONFIG:Release>:/INCREMENTAL:NO>) # 禁用增量链接
    add_link_options($<$<CONFIG:Release>:/LTCG>) # 链接时代码生成
    add_link_options($<$<CONFIG:Release>:/DEBUG>) # 启用调试
    add_compile_options($<$<CONFIG:Release>:/O2>) # 优化速度
    add_compile_options($<$<CONFIG:Release>:/Oi>) # 启用内在函数
    add_compile_options($<$<CONFIG:Release>:/GL>) # 启用链接时代码生成
    add_compile_options($<$<CONFIG:Release>:/Gy>) # 分离函数给链接器
    add_compile_options($<$<CONFIG:Release>:/Gw>) # 优化全局数据
    add_compile_options($<$<CONFIG:Release>:/Gm->) # 禁用最小重建
    add_compile_options($<$<CONFIG:Release>:/GF>) # 启用字符串池
    add_compile_options($<$<CONFIG:Release>:/fp:fast>) # 优化浮点
    add_compile_options($<$<CONFIG:Release>:/GR->) # 禁用 RTTI
    add_compile_options($<$<CONFIG:Release>:/Gd>) # 使用 __cdecl 调用约定
    add_compile_options($<$<CONFIG:Release>:/MT>) # 使用静态多线程运行时
    add_compile_options($<$<CONFIG:Release>:/Ob2>) # 内联合适函数
    add_compile_options($<$<CONFIG:Release>:/Ot>) # 倾向快速代码
    add_compile_options($<$<CONFIG:Release>:/Oy->) # 省略帧指针
endfunction()

# 根据构建类型设置选项
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set_debug_options()
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
    set_release_options()
endif ()

set(SOLSTICE_VERSION "v1.0.0")
add_compile_definitions(SOLSTICE_VERSION="${SOLSTICE_VERSION}")

# 根据操作系统添加特定定义
if(WIN32)
    add_definitions(-DWIN32_LEAN_AND_MEAN)
endif()

include_directories(.)
include_directories(src)
include_directories(include)
include_directories(include/minhook/include)
include_directories(include/libhat)
include_directories(include/entt)
include_directories(include/libhat/libhat)
include_directories(include/ImGui)
include_directories(include/Kiero)
include_directories(include/nes)
include_directories(include/Luau)
include_directories(include/Luau/VM/include)

file(GLOB_RECURSE sourceFiles "src/*.cpp")
file(GLOB_RECURSE includeFiles "include/*.hpp")
set_source_files_properties(${sourceFiles} PROPERTIES CXX_SCAN_FOR_MODULES OFF)
set_source_files_properties(${includeFiles} PROPERTIES CXX_SCAN_FOR_MODULES OFF)

# Krazy 资源嵌入代码
file(GLOB_RECURSE resource_files_list "resources/*")

# 提前计算资源文件相对路径，避免重复计算
set(resource_files_rel "")
foreach(file ${resource_files_list})
    file(RELATIVE_PATH relative_file ${CMAKE_CURRENT_SOURCE_DIR}/resources ${file})
    list(APPEND resource_files_rel ${relative_file})
endforeach()

function(add_resources out_var)
    set(result)
    foreach(in_f ${resource_files_rel})
        file(RELATIVE_PATH src_f ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/resources/${in_f})
        set(out_f "${PROJECT_BINARY_DIR}/${in_f}.o")
        add_custom_command(OUTPUT ${out_f}
                COMMAND ${LD_COMMAND} -r -b binary -o ${out_f} ${src_f}
                DEPENDS ${in_f}
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                COMMENT "Building object ${out_f}"
                VERBATIM
        )
        list(APPEND result ${out_f})
        message(STATUS "Embedding resource: ${in_f}")
    endforeach()
    set(${out_var} "${result}" PARENT_SCOPE)
endfunction()

add_resources(resource_files ${resource_files_rel})

add_library(Solstice SHARED ${sourceFiles} ${resource_files})
add_dependencies(${PROJECT_NAME} generate_build_info SolsticeShaders)

set(RESOURCE_HEADER_FILE "${CMAKE_CURRENT_SOURCE_DIR}/src/Utils/Resources.hpp")
set(RESOURCE_SOURCE_FILE "${CMAKE_CURRENT_SOURCE_DIR}/src/Utils/Resources.cpp")

# 生成 Resources.hpp 内容
set(RESOURCE_HEADER_CONTENT "// This file is generated by CMake. Do not edit it manually.\n")
set(RESOURCE_HEADER_CONTENT "${RESOURCE_HEADER_CONTENT}#pragma once\n")
set(RESOURCE_HEADER_CONTENT "${RESOURCE_HEADER_CONTENT}#include <string>\n")
set(RESOURCE_HEADER_CONTENT "${RESOURCE_HEADER_CONTENT}#include <unordered_map>\n")
set(RESOURCE_HEADER_CONTENT "${RESOURCE_HEADER_CONTENT}#include \"Resource.hpp\"\n")
set(RESOURCE_HEADER_CONTENT "${RESOURCE_HEADER_CONTENT}\n")

foreach(file ${resource_files_list})
    file(RELATIVE_PATH relative_file ${CMAKE_CURRENT_SOURCE_DIR}/resources ${file})
    string(REPLACE "/" "_" resource_name ${relative_file})
    string(REPLACE "." "_" resource_name ${resource_name})
    string(REPLACE "-" "_" resource_name ${resource_name})
    set(RESOURCE_HEADER_CONTENT "${RESOURCE_HEADER_CONTENT}LOAD_RESOURCE(${resource_name});\n")
endforeach()

set(RESOURCE_HEADER_CONTENT "${RESOURCE_HEADER_CONTENT}\n")
set(RESOURCE_HEADER_CONTENT "${RESOURCE_HEADER_CONTENT}class ResourceLoader {\n")
set(RESOURCE_HEADER_CONTENT "${RESOURCE_HEADER_CONTENT}public:\n")
set(RESOURCE_HEADER_CONTENT "${RESOURCE_HEADER_CONTENT}    static inline std::unordered_map<std::string, Resource> Resources;\n")
set(RESOURCE_HEADER_CONTENT "${RESOURCE_HEADER_CONTENT}    static void loadResources();\n")
set(RESOURCE_HEADER_CONTENT "${RESOURCE_HEADER_CONTENT}};\n")

# 写入 Resources.hpp 文件
file(WRITE ${RESOURCE_HEADER_FILE} "${RESOURCE_HEADER_CONTENT}")

message(STATUS "Generated Resources.hpp")

# 生成 Resources.cpp 内容
set(RESOURCE_SOURCE_CONTENT "// This file is generated by CMake. Do not edit it manually.\n")
set(RESOURCE_SOURCE_CONTENT "${RESOURCE_SOURCE_CONTENT}#include \"Resources.hpp\"\n")
set(RESOURCE_SOURCE_CONTENT "${RESOURCE_SOURCE_CONTENT}#include \"FontHelper.hpp\"\n")
set(RESOURCE_SOURCE_CONTENT "${RESOURCE_SOURCE_CONTENT}\n")
set(RESOURCE_SOURCE_CONTENT "${RESOURCE_SOURCE_CONTENT}void ResourceLoader::loadResources() {\n")

set(RESOURCE_SOURCE_CONTENT "${RESOURCE_SOURCE_CONTENT}    ImFontConfig font_config;\n")
set(RESOURCE_SOURCE_CONTENT "${RESOURCE_SOURCE_CONTENT}    font_config.FontBuilderFlags = ImGuiFreeTypeBuilderFlags_NoHinting;\n")

foreach(file ${resource_files_list})
    file(RELATIVE_PATH relative_file ${CMAKE_CURRENT_SOURCE_DIR}/resources ${file})

    # 获取文件名无扩展名并替换 / 为 _
    string(REGEX REPLACE "(.*)(/)(.*)(\\.)(.*)" "\\3" friendly_name ${relative_file})
    string(REPLACE "-" "_" friendly_name ${friendly_name})
    string(TOLOWER ${friendly_name} friendly_name)

    # 构建资源名
    string(REPLACE "/" "_" resource_name ${relative_file})
    string(REPLACE "." "_" resource_name ${resource_name})
    string(REPLACE "-" "_" resource_name ${resource_name})

    set(RESOURCE_SOURCE_CONTENT "${RESOURCE_SOURCE_CONTENT}    auto ${resource_name} = GET_RESOURCE(${resource_name});\n")
    set(RESOURCE_SOURCE_CONTENT "${RESOURCE_SOURCE_CONTENT}    Resources.emplace(\"${friendly_name}\", ${resource_name});\n")

    if(NOT ${relative_file} MATCHES ".*\\.ttf")
        continue()
    endif()

    set(RESOURCE_SOURCE_CONTENT "${RESOURCE_SOURCE_CONTENT}    FontHelper::Fonts.emplace(\"${friendly_name}\", ImGui::GetIO().Fonts->AddFontFromMemoryTTF(${resource_name}.data2(), ${resource_name}.size(), 20, &font_config));\n")
    set(RESOURCE_SOURCE_CONTENT "${RESOURCE_SOURCE_CONTENT}    FontHelper::Fonts.emplace(\"${friendly_name}_large\", ImGui::GetIO().Fonts->AddFontFromMemoryTTF(${resource_name}.data2(), ${resource_name}.size(), 42, &font_config));\n")
endforeach()

set(RESOURCE_SOURCE_CONTENT "${RESOURCE_SOURCE_CONTENT}}\n")

# 写入 Resources.cpp 文件
file(WRITE ${RESOURCE_SOURCE_FILE} "${RESOURCE_SOURCE_CONTENT}")

message(STATUS "Generated Resources.cpp")

add_subdirectory(include/minhook)
add_subdirectory(include/libhat)
add_subdirectory(include/ImGui)
add_subdirectory(include/Kiero)
add_subdirectory(include/nes)
add_subdirectory(include/Luau)

set_property(TARGET DirectXTK PROPERTY UNITY_BUILD FALSE)
set_property(TARGET minhook PROPERTY UNITY_BUILD FALSE)

target_include_directories(Solstice PRIVATE include/ImGui)
target_include_directories(Solstice PRIVATE include/Kiero)
target_include_directories(Solstice PRIVATE include/Luau)
target_include_directories(Solstice PRIVATE include/Luau/Compiler/include)
target_include_directories(Solstice PRIVATE include/Luau/VM/include)

target_link_libraries(Solstice PRIVATE OpenMP::OpenMP_CXX LuaBridge DirectXTK Luau.Compiler Luau.VM Luau.Ast spdlog version.lib nlohmann_json glm minhook kiero nuvola_event_system libhat dbghelp.lib winmm.lib magic_enum d3d12.lib d3d11.lib d2d1.lib dxgi.lib dxguid.lib dwrite.lib winmm.lib dbghelp.lib comctl32.lib ws2_32.lib cryptopp ole32.lib Mmdevapi.lib)

set_property(TARGET Solstice nlohmann_json minhook spdlog LuaBridge cryptopp Luau.Compiler Luau.VM Luau.Ast DirectXTK glm kiero nuvola_event_system magic_enum libhat PROPERTY MSVC_RUNTIME_LIBRARY "$<$<CONFIG:Debug>:MultiThreadedDLL>$<$<CONFIG:Release>:MultiThreaded>")


# 修正后的 target_precompile_headers
target_precompile_headers(Solstice PRIVATE
        $<$<COMP
