cmake_minimum_required(VERSION 2.8)
add_definitions(-std=c++11 -pthread)
set(CMAKE_AUTOMOC ON)

file(GLOB SRCfiles
        "RAI_graphics.hpp"
        "RAI_graphics.cpp"
        "src/imp/*.cpp"
        "src/obj/*.cpp"
        )

file(GLOB objHDRfiles
        "src/obj/*.hpp"
        "src/obj/*.h"
        )

file(GLOB impHDRfiles
        "src/imp/*.hpp"
        "src/imp/*.h"
        )

# package: Eigen3
find_package( Eigen3 REQUIRED )
include_directories( ${EIGEN3_INCLUDE_DIR} )
link_directories(/usr/lib)

#include(FindPkgConfig) TODO is this required?

# include directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# link libraries
add_library(raiGraphics ${SRCfiles})
set(RAI_Graphics raiGraphics glut GLU GL GLEW SDL2 assimp SOIL glog freeimage pthread)

# executable
add_executable(testRAIGraphics test.cpp)
target_link_libraries(testRAIGraphics ${RAI_Graphics})

# install files
install(FILES           ${objHDRfiles}
        DESTINATION     ${CMAKE_BINARY_DIR}/include/obj)
install(FILES           ${impHDRfiles}
        DESTINATION     ${CMAKE_BINARY_DIR}/include/imp)
install(FILES           RAI_graphics.hpp RAI_keyboard.hpp
        DESTINATION     ${CMAKE_BINARY_DIR}/include)
install(TARGETS         raiGraphics EXPORT RAITargets
        DESTINATION     ${CMAKE_BINARY_DIR}/lib)

# set RAI_LINK
set(RAI_LINK glog gflags CACHE INTERNAL "")
set(RAI_LINK rai_logger rai_timer rai_graph ${RAI_LINK})
set(RAI_LINK ${RAI_Graphics} ${RAI_LINK})

# --------------------------------------------------------------------
# package raiGraphics
export(PACKAGE raiGraphics)

# create raiGraphicsConfig.cmake.in
set(RAI_GRAPHICS_CONFIG_CMAKE_PATH lib/cmake/raiGraphics)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/raiGraphicsConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/raiGraphicsConfig.cmake @ONLY)

# build tree
set(CONF_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include")

# install raiGraphicsConfig.cmake file to config cmake path
install(FILES           ${CMAKE_CURRENT_BINARY_DIR}/raiGraphicsConfig.cmake
        DESTINATION     ${RAI_GRAPHICS_CONFIG_CMAKE_PATH})