cmake_minimum_required(VERSION 3.6)
project(rer_tree)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp rer_tree/Point.cpp rer_tree/Point.h rer_tree/Rectangle.cpp rer_tree/Rectangle.h rer_tree/rer_node.cpp rer_tree/rer_node.h rer_tree/rer_tree.cpp rer_tree/rer_tree.h)
add_executable(rer_tree ${SOURCE_FILES})


INCLUDE(FindPkgConfig)

PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)

INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES})
