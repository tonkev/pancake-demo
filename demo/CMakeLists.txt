cmake_minimum_required(VERSION 3.21)

project(demo)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(demo
    src/systems/menu.cpp
    src/systems/mover.cpp
    src/systems/pick_mouse_entity.cpp
    src/demo_session.cpp
    src/demo.cpp
    src/session_config_rules.cpp
)

target_include_directories(demo PUBLIC
    ../pancake/pancake/include
    include/demo
)

target_link_libraries(demo
    pancake
)

if (WIN32)
    add_custom_command(TARGET demo POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:demo> $<TARGET_FILE_DIR:demo>
        COMMAND_EXPAND_LISTS
    )
endif()