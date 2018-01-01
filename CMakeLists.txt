cmake_minimum_required(VERSION 3.5)
add_definitions(-std=c++11 -pthread -g)
include(GNUInstallDirs)
set(CMAKE_CXX_COMPILER "/usr/bin/g++-7")

project(raiGraphicsOpengl)

# --------------------------------------------------------------------
# install path
set(CMAKE_PACKAGE_INSTALL_DIR
        "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}/cmake"
        CACHE PATH "The directory relative to CMAKE_PREFIX_PATH where raiGraphicsConfig.cmake is installed")

set(RAI_GRAPHICS_OPENGL_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# ----------------------------------------------------------------------------------------------------------------------
# names
set(CONFIG_FILE_NAME ${PROJECT_NAME}Config.cmake)
set(TARGETS_EXPORT_NAME ${PROJECT_NAME}Targets)
set(TARGETS_LIB_NAME ${PROJECT_NAME}Lib)

# --------------------------------------------------------------------
# files
file(GLOB_RECURSE
        SOURCE_FILES
        "src/*.cpp")

file(GLOB_RECURSE
        HEADER_FILES
        "include/*.hpp"
        "include/*.h")

# --------------------------------------------------------------------
# package: Eigen3
find_package( Eigen3 REQUIRED )
include_directories( ${EIGEN3_INCLUDE_DIR} )
link_directories(/usr/lib)

find_package(raiCommon CONFIG REQUIRED)
include_directories(${RAI_COMMON_INCLUDE_DIR})

# --------------------------------------------------------------------
# include directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# link libraries
add_library(${TARGETS_LIB_NAME}
        SHARED ${SOURCE_FILES} ${HEADER_FILES})
set(RAI_GRAPHICS_OPENGL_LINK ${TARGETS_LIB_NAME} glut GLU GL GLEW SDL2 SDL2_ttf assimp SOIL freeimage pthread ${RAI_COMMON_LINK})

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