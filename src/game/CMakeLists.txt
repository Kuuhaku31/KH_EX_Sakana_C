
project(game)

# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_LIB_SRCS 变量
aux_source_directory(. DIR_LIB_SRCS)

# 生成链接库
add_library(${PROJECT_NAME} SHARED ${DIR_LIB_SRCS})

# 添加头文件
target_include_directories(${PROJECT_NAME} PUBLIC ${Game_Character_Dir}/character_manager)

target_link_libraries(${PROJECT_NAME} game_character)
