cmake_minimum_required (VERSION 3.10)

project (WildcatApp)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

set (CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/lib/sdl2)
find_package (SDL2 REQUIRED)
find_package (SDL2_image REQUIRED)
find_package (SDL2_ttf REQUIRED)
find_package (SDL2_mixer REQUIRED)

include_directories (
    ${SDL2_INCLUDE_DIR} 
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${SDL2_MIXER_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
)

include_directories ("src/core")

file (GLOB SRC "src/core/*/*.cc" "src/core/*.cc" "src/*.cc")

link_libraries (
    ${SDL2_LIBRARY} 
    ${SDL2_IMAGE_LIBRARIES} 
    ${SDL2_MIXER_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
)

add_executable (wkt_app "${SRC}")