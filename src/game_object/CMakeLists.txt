
project(game_object)

# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_LIB_SRCS 变量
aux_source_directory(. DIR_LIB_SRCS)

# 生成链接库
add_library(${PROJECT_NAME} ${DIR_LIB_SRCS})

# 添加头文件路径
target_include_directories(${PROJECT_NAME} PUBLIC ${Game_Resources_Dir} ${Game_Collision_Dir})

# 添加链接库
target_link_libraries(${PROJECT_NAME} game_resources game_collision)
