
cmake_minimum_required(VERSION 3.5)
project(Sakana_C
    VERSION 0.0.1
    DESCRIPTION "a demo for sakana"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

# 启动测试
enable_testing()

# 设置可执行文件的输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

# 设置文件目录
set(Game_Dir ${PROJECT_SOURCE_DIR}/src/game)

set(Game_Base_Dir ${PROJECT_SOURCE_DIR}/src/game_base)
set(Game_Resources_Dir ${PROJECT_SOURCE_DIR}/src/game_resources)
set(Game_Collision_Dir ${PROJECT_SOURCE_DIR}/src/game_collision)
set(Game_State_Machine_Dir ${PROJECT_SOURCE_DIR}/src/game_state_machine)
set(Game_Character_Dir ${PROJECT_SOURCE_DIR}/src/game_character)
set(Game_Bullet_Time_Manager_Dir ${PROJECT_SOURCE_DIR}/src/bullet_time_manager)
set(Game_Effects_Dir ${PROJECT_SOURCE_DIR}/src/game_effects)

# 添加子目录
add_subdirectory(src)
add_subdirectory(test/testA)
