cmake_minimum_required(VERSION 3.28)
project(minesweeper LANGUAGES C VERSION 1.0)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -save-temps=obj")

include(FetchContent)
set(FETCHCONTENT_QUIET OFF)

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
        src/initialization/init_header.c
        src/initialization/init_freetype.c

        src/text/text.c
        src/text/render.c

        src/ui/button.c
        src/ui/btn_runtime.c
        src/ui/gradient.c
        src/ui/mouse_evt.c

        src/draw.c
        src/loop.c
        src/main.c
        src/utils.c
)

set(INCLUDE_FILES
        include/debug.h
        include/defines.h
        include/minesweeper.h
        include/structs.h
        include/color.h
        include/text.h
        include/utils.h

        include/ui/button.h
        include/ui/gradient.h
        include/ui/ui.decl.h

        src/initialization/borders_utils.h
        src/ui/button.h
        src/ui/mouse_evt.h
)

add_subdirectory(lib/logger)

find_package(Freetype)

add_executable(minesweeper ${SRCS} ${INCLUDE_FILES})
target_include_directories(minesweeper PRIVATE include/)
target_link_libraries(minesweeper PRIVATE logger SDL2::SDL2 SDL2_image SDL2_ttf Freetype::Freetype)


add_executable(generate_tile_numbers src/generators/generate_tile_numbers.c src/utils.c)
target_include_directories(generate_tile_numbers PRIVATE include/)
target_link_libraries(generate_tile_numbers PRIVATE logger SDL2::SDL2 SDL2_image SDL2_ttf)
