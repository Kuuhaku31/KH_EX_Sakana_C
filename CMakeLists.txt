
cmake_minimum_required(VERSION 3.5)
project(Sakana_C
    VERSION 0.0.1
    DESCRIPTION "a demo for sakana"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

# 设置可执行文件的输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

# 设置文件目录
set(Game_Base_Dir ${PROJECT_SOURCE_DIR}/src/game_base)
set(Game_Manager_Dir ${PROJECT_SOURCE_DIR}/src/game_manager)
set(Game_State_Machine_Dir ${PROJECT_SOURCE_DIR}/src/game_state_machine)
set(Game_Character_Dir ${PROJECT_SOURCE_DIR}/src/game_character)

# 添加子目录
add_subdirectory(src)
