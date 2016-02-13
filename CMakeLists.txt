cmake_minimum_required(VERSION 2.8)
project(llvmlinux_script)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -save-temps=obj -g -D 'MYPREFIX=\"${CMAKE_SOURCE_DIR}\"'")

set(SOURCE_FILES clang-genbc.c ld-genbc.c ar-genbc.c filter.c)
add_executable(ld-genbc ld-genbc.c filter.c)
add_executable(ar-genbc ar-genbc.c filter.c)
add_executable(clang-genbc clang-genbc.c)
