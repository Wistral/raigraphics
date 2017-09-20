cmake_minimum_required(VERSION 3.5)
add_definitions(-std=c++11 -pthread)
set(CMAKE_AUTOMOC ON)
include(GNUInstallDirs)

project(raiGraphics)

# --------------------------------------------------------------------
# install path
set(CMAKE_PACKAGE_INSTALL_DIR
        "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}/cmake"
        CACHE PATH "The directory relative to CMAKE_PREFIX_PATH where raiGraphicsConfig.cmake is installed")

#
#set(PKGCONFIG_INSTALL_DIR
#        "/${CMAKE_INSTALL_DATADIR}/pkgconfig"
#        CACHE PATH "The directory relative to CMAKE_PREFIX_PATH where RAI.pc is installed"
#        )

#set(RAI_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# --------------------------------------------------------------------
# files
file(GLOB SRCfiles
        "include/raiGraphics/RAI_graphics.hpp"      # TODO check
        "src/RAI_graphics.cpp"                      # TODO check
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

# --------------------------------------------------------------------
# package: Eigen3
find_package( Eigen3 REQUIRED )
include_directories( ${EIGEN3_INCLUDE_DIR} )
link_directories(/usr/lib)

# --------------------------------------------------------------------
#include(FindPkgConfig) TODO is this required?

# include directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# link libraries
add_library(raiGraphicsLib ${SRCfiles})
set(RAI_Graphics raiGraphicsLib glut GLU GL GLEW SDL2 assimp SOIL glog freeimage pthread)

# executable
add_executable(testRAIGraphics test.cpp)
target_link_libraries(testRAIGraphics ${RAI_Graphics})

# --------------------------------------------------------------------
# set RAI_LINK
#set(RAI_GRAPHICS_LINK glog gflags CACHE INTERNAL "")
#set(RAI_GRAPHICS_LINK rai_logger rai_timer rai_graph ${RAI_GRAPHICS_LINK})
#set(RAI_GRAPHICS_LINK ${RAI_Graphics} ${RAI_GRAPHICS_LINK})

# --------------------------------------------------------------------
# package raiGraphics
export(PACKAGE ${PROJECT_NAME})
include(CMakePackageConfigHelpers)  # TODO is this required?

# build tree
set(CONF_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include")

# create raiGraphicsConfig.cmake.in
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/raiGraphicsConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake @ONLY)

# install the export set
#install(EXPORT  # TODO
#        DESTINATION ${INSTALL_CMAKE_DIR})

# install raiGraphicsConfig.cmake file to config cmake path
install(FILES           ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
        DESTINATION     ${CMAKE_PACKAGE_INSTALL_DIR})