cmake_minimum_required(VERSION 3.20)
project(GI LANGUAGES C)
set(CMAKE_C_STANDARD 11) 
aux_source_directory(src SRCS)
add_executable(${PROJECT_NAME} ${SRCS})

target_compile_definitions(${PROJECT_NAME} PRIVATE _CRT_SECURE_NO_WARNINGS)