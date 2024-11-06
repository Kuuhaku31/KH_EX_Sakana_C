
project(game_effects)

aux_source_directory(. DIR_LIB_SRCS)

add_library(
    ${PROJECT_NAME}
    SHARED
    ${DIR_LIB_SRCS}
)

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
    ${Game_Resources_Dir}
)

target_link_libraries(
    ${PROJECT_NAME}
    easyx
    game_resources
)
