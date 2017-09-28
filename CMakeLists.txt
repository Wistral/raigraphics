cmake_minimum_required(VERSION 3.5)
add_definitions(-std=c++11 -pthread)
set(CMAKE_AUTOMOC ON)
include(GNUInstallDirs)

project(raiGraphicsOpengl)

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

set(RAI_GRAPHICS_OPENGL_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# ----------------------------------------------------------------------------------------------------------------------
# names
set(CONFIG_FILE_NAME ${PROJECT_NAME}Config.cmake)
set(TARGETS_EXPORT_NAME ${PROJECT_NAME}Targets)
set(TARGETS_LIB_NAME ${PROJECT_NAME}Lib)

# --------------------------------------------------------------------
# files
file(GLOB SOURCES
#        "include/raiGraphics/RAI_graphics.hpp"
        "src/RAI_graphics.cpp"
        "src/imp/*.cpp"
        "src/obj/*.cpp"
        )

#file(GLOB objHDRfiles
#        "src/obj/*.hpp"
#        "src/obj/*.h"
#        )
#
#file(GLOB impHDRfiles
#        "src/imp/*.hpp"
#        "src/imp/*.h"
#        )

# --------------------------------------------------------------------
# package: Eigen3
find_package( Eigen3 REQUIRED )
include_directories( ${EIGEN3_INCLUDE_DIR} )
link_directories(/usr/lib)

# --------------------------------------------------------------------
# include directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# link libraries
add_library(${TARGETS_LIB_NAME} SHARED ${SOURCES})
set(RAI_GRAPHICS_OPENGL_LINK ${TARGETS_LIB_NAME} glut GLU GL GLEW SDL2 assimp SOIL glog freeimage pthread)

# executable
add_executable(testRAIGraphics raiGraphicsTest.cpp)
target_link_libraries(testRAIGraphics ${RAI_GRAPHICS_OPENGL_LINK})

# --------------------------------------------------------------------
# package raiGraphics
export(PACKAGE ${PROJECT_NAME})

# build tree
set(CONF_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include")

# create raiGraphicsOpenglConfig.cmake.in
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/${CONFIG_FILE_NAME}.in
        ${CMAKE_CURRENT_BINARY_DIR}/${CONFIG_FILE_NAME} @ONLY)

# install package
install(TARGETS                 ${TARGETS_LIB_NAME}
        EXPORT                  ${TARGETS_EXPORT_NAME}
        LIBRARY DESTINATION     lib
        ARCHIVE DESTINATION     lib
#        INCLUDES DESTINATION    include
        )

# install raiGraphicsConfig.cmake file to config cmake path
install(FILES           ${CMAKE_CURRENT_BINARY_DIR}/${CONFIG_FILE_NAME}
        DESTINATION     ${CMAKE_PACKAGE_INSTALL_DIR})