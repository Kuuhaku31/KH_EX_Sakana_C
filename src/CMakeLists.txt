
project(main)

# 添加子目录
add_subdirectory(game_base)
add_subdirectory(game_manager)
add_subdirectory(game_character)
add_subdirectory(game_state_machine)

# 添加可执行文件
add_executable(${PROJECT_NAME} main.cpp)

# 添加头文件
target_include_directories(${PROJECT_NAME} PUBLIC ${Game_Character_Dir})

# 添加链接库
target_link_libraries(${PROJECT_NAME} game_character)
