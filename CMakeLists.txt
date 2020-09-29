cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

project(mybase LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# 添加cmake打印色彩
include(colors)
include(flags)
define_colors()

message(STATUS "${BoldMagenta}This is bold${ColourReset}")

configure_file(src/core/configure_in.h.in ${CMAKE_CURRENT_SOURCE_DIR}/src/core/configure_in.h @ONLY)

# 主程序代码
add_subdirectory(src)
add_executable(mybase ${SRC_FILES})

target_include_directories(mybase PUBLIC src/)
list(APPEND APP_DEFS "SOFT_VERSION=\"1.0.0\"")
# target_compile_definitions(mybase PUBLIC ${ARCH_DEFS})
target_compile_definitions(mybase PUBLIC ${APP_DEFS})

# 测试程序代码
enable_testing()
add_subdirectory(test)
add_executable(unit_test ${TEST_SRC_FILES})
target_compile_definitions(unit_test PUBLIC ${ARCH_DEFS})
