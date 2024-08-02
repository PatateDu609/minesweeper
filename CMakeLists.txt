cmake_minimum_required(VERSION 3.28)
project(minesweeper LANGUAGES C VERSION 1.0)

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

FetchContent_Declare(SDL2
        GIT_REPOSITORY https://github.com/libsdl-org/SDL
        GIT_TAG 2eef7ca475decd2b864214cdbfe72b143b16d459   # release-2.30.5
        GIT_PROGRESS TRUE
)

FetchContent_Declare(SDL2_IMAGE
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_image
        GIT_TAG abcf63aa71b4e3ac32120fa9870a6500ddcdcc89   # release-2.8.3
        GIT_PROGRESS TRUE
)

FetchContent_Declare(SDL2_TTF
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf
        GIT_TAG 4a318f8dfaa1bb6f10e0c5e54052e25d3c7f3440   # release-2.22.0
        GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(SDL2 SDL2_IMAGE SDL2_TTF)

set(SRCS
        src/debug/print_field.c

        src/game/events.c
        src/game/map.c

        src/initialization/init_borders.c
        src/initialization/init_game.c
        src/initialization/init_sprites.c
        src/initialization/initialization.c

        src/draw.c
        src/generate_tile_numbers.c
        src/loop.c
        src/main.c
        src/utils.c
)

set(INCLUDE_FILES
        include/debug.h
        include/defines.h
        include/minesweeper.h
        include/structs.h
        include/utils.h

        src/initialization/borders_utils.h
)

add_subdirectory(lib/logger)

add_executable(minesweeper ${SRCS} ${INCLUDE_FILES})
target_include_directories(minesweeper PRIVATE include/)
target_link_libraries(minesweeper PRIVATE logger SDL2::SDL2 SDL2_image SDL2_ttf)
