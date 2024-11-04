
project(main)

# 添加子目录
add_subdirectory(game)

add_subdirectory(game_base) # 基础
add_subdirectory(game_resources) # 资源
add_subdirectory(game_character) # 角色
add_subdirectory(game_state_machine) # 状态机
add_subdirectory(game_collision) # 碰撞检测
add_subdirectory(game_effects) # 游戏特效

# add_subdirectory(bullet_time_manager)

# 添加可执行文件
add_executable(${PROJECT_NAME} main.cpp)

# 添加头文件
target_include_directories(${PROJECT_NAME} PUBLIC ${Game_Dir})

# 添加链接库
target_link_libraries(${PROJECT_NAME} game)
