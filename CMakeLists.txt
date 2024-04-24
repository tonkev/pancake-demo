cmake_minimum_required(VERSION 3.00)

project(demo_and_all)

add_compile_definitions(PANCAKE_ENABLE_IMGUI)

add_subdirectory(pancake)
add_subdirectory(demo)