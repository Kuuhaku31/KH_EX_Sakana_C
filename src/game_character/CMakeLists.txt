
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
    ./character_manager
)

# 连接 game_manager 和 game_state_machine
target_include_directories(${PROJECT_NAME} PUBLIC ${Game_Resources_Dir} ${Game_State_Machine_Dir} ${Game_Collision_Dir})

target_link_libraries(${PROJECT_NAME} game_state_machine game_collision game_resources)
