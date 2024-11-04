
project(game_character)

# 递归查找当前目录及其子目录下的所有源文件
# 并将名称保存到 DIR_LIB_SRCS 变量
file(GLOB_RECURSE DIR_LIB_SRCS "*.cpp")

# 生成链接库
add_library(${PROJECT_NAME} SHARED ${DIR_LIB_SRCS})

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
    ./
    ./player
    ./player/player_state_nodes
    ./enemy
    ./enemy/enemy_state_nodes
    ./enemy/enemy_skills
    ./bullet_time_manager
    ./character_manager
)

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
    ${Game_Collision_Dir}
    ${Game_Resources_Dir}
    ${Game_State_Machine_Dir}
    ${Game_Effects_Dir}
)

target_link_libraries(
    ${PROJECT_NAME}
    game_effects
    game_collision
    game_resources
    game_state_machine
)
