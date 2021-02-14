project(raigraphics)

cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_COMPILER "/usr/bin/g++")
set(CMAKE_CXX_STANDARD 14)

add_definitions(-g -fPIC)

option(BUILD_SHARED_LIBS "Build the shared library" ON)
option(BUILD_DEMOS "Build the demos" ON)
option(BUILD_PYTHON "Build python library" OFF)
option(INSTALL_LIBS "Build the shared library" ON)

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
set(TARGETS_LIB_NAME ${PROJECT_NAME})

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
find_package(Eigen3 REQUIRED)

# package: Boost
find_package(Boost COMPONENTS thread system filesystem REQUIRED)

# lib (raicommon)
find_package(raicommon CONFIG REQUIRED)

# system libraries
SET(SYSTEM_LIBRARIES_NAME
        opengl32
        glew32
        SDL2
        SDL2_ttf
        assimp
        SOIL
        pthread)

foreach(LIB ${SYSTEM_LIBRARIES_NAME})
    find_library(${LIB}_PATH ${LIB})

    if(NOT ${LIB}_PATH)
        message( FATAL_ERROR "Cannot find ${LIB}" )
    else()
        message( STATUS "Found ${LIB}: ${${LIB}_PATH}" )
        list(APPEND FOUND_SYSTEM_LIBRARIES ${${LIB}_PATH})
    endif()
endforeach()


# --------------------------------------------------------------------
# create config.hpp
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.hpp.in
        ${CMAKE_CURRENT_SOURCE_DIR}/include/raiGraphics/config.hpp @ONLY)

# add libraries
if(BUILD_SHARED_LIBS)
    add_library(${TARGETS_LIB_NAME}
            SHARED
            ${SOURCE_FILES}
            ${HEADER_FILES}
            )
else(BUILD_SHARED_LIBS)
    add_library(${TARGETS_LIB_NAME}
            STATIC
            ${SOURCE_FILES}
            ${HEADER_FILES}
            )
endif(BUILD_SHARED_LIBS)

# include directories for lib
target_include_directories(${TARGETS_LIB_NAME}
        SYSTEM
        PUBLIC
        "include"
        ${raicommon_INCLUDE_DIRS}
        ${EIGEN3_INCLUDE_DIR}
        )

# link directory
# NOTE it's better to use target_link_directories but cmake 3.5 doesn't support it
link_directories(
        ${raicommon_LIBRARIES_DIR}
)

# link libraries for lib
target_link_libraries(${TARGETS_LIB_NAME}
        ${FOUND_SYSTEM_LIBRARIES}
        ${Boost_LIBRARIES}
        ${raicommon_LIBRARIES}
        )

get_target_property(${PROJECT_NAME}_LINK
        ${TARGETS_LIB_NAME}
        LINK_LIBRARIES
        )

set(${PROJECT_NAME}_LINK
        ${${PROJECT_NAME}_LINK})

# --------------------------------------------------------------------
# demos
if(BUILD_DEMOS)
    # executable
    add_executable(testRAIGraphics raiGraphicsTest.cpp)
    target_link_libraries(testRAIGraphics ${TARGETS_LIB_NAME})
endif(BUILD_DEMOS)

# --------------------------------------------------------------------
# install
include(GNUInstallDirs)

if(INSTALL_LIBS)
    # package raiGraphics
    export(PACKAGE ${PROJECT_NAME})

    # build tree
    set(CONF_INCLUDE_DIRS
            "${CMAKE_CURRENT_SOURCE_DIR}/include"
            "${raicommon_INCLUDE_DIRS}")

    set(CONFIG_LIBRARIES_DIRS
            ${CMAKE_INSTALL_LIBDIR})

    # create raigraphicsConfig.cmake.in
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/${CONFIG_FILE_NAME}.in
            ${CMAKE_CURRENT_BINARY_DIR}/${CONFIG_FILE_NAME} @ONLY)

    # install package
    install(TARGETS                 ${TARGETS_LIB_NAME}
            EXPORT                  ${TARGETS_EXPORT_NAME}
            LIBRARY DESTINATION     ${CMAKE_INSTALL_LIBDIR}
            ARCHIVE DESTINATION     ${CMAKE_INSTALL_LIBDIR}
            INCLUDES DESTINATION    include
            )

    # install raiGraphicsConfig.cmake file to config cmake path
    install(FILES           ${CMAKE_CURRENT_BINARY_DIR}/${CONFIG_FILE_NAME}
            DESTINATION     ${CMAKE_INSTALL_LIBDIR}/cmake/raigraphics)

endif(INSTALL_LIBS)

# --------------------------------------------------------------------
# python
if(BUILD_PYTHON)
    add_subdirectory(bindings)
endif(BUILD_PYTHON)
