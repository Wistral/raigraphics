cmake_minimum_required(VERSION 3.5)
add_definitions(-std=c++11 -pthread -g)
include(GNUInstallDirs)
set(CMAKE_CXX_COMPILER "/usr/bin/g++-7")

project(raigraphics)
option(BUILD_SHARED_LIBS "Build the shared library" OFF)
option(BUILD_DEMOS "Build the demos" ON)
option(INSTALL_LIBS "Build the shared library" OFF)

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
find_package( Eigen3 REQUIRED)
include_directories(SYSTEM ${EIGEN3_INCLUDE_DIR})

# package: raicommon
find_package(raiCommon CONFIG REQUIRED)
include_directories(${RAI_COMMON_INCLUDE_DIR})

# system libraries
SET(SYSTEM_LIBRARIES "GL;GLEW;SDL2;SDL2_ttf;assimp;SOIL;pthread")

foreach(LIB ${SYSTEM_LIBRARIES})
    find_library(${LIB}_PATH ${LIB})

    if(NOT ${LIB}_PATH)
        message( FATAL_ERROR "Cannot find ${LIB}" )
    else()
        message( STATUS "Found ${LIB}: ${${LIB}_PATH}" )
    endif()
endforeach()


# --------------------------------------------------------------------
# include directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# create config.hpp
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.hpp.in
        ${CMAKE_CURRENT_SOURCE_DIR}/include/raiGraphics/config.hpp @ONLY)

# add libraries
if(BUILD_SHARED_LIBS)
    add_library(${TARGETS_LIB_NAME}
            SHARED ${SOURCE_FILES} ${HEADER_FILES})
else(BUILD_SHARED_LIBS)
    add_library(${TARGETS_LIB_NAME}
            STATIC ${SOURCE_FILES} ${HEADER_FILES})
endif(BUILD_SHARED_LIBS)

# link target library
target_link_libraries(${TARGETS_LIB_NAME}
        ${SYSTEM_LIBRARIES}
        ${raiCommon_LIBRARIES})
get_target_property(
        ${PROJECT_NAME}_LINK
        ${TARGETS_LIB_NAME}
        LINK_LIBRARIES
)
set(${PROJECT_NAME}_LINK
        ${${PROJECT_NAME}_LINK};${TARGETS_LIB_NAME})

# --------------------------------------------------------------------
# demos
if(BUILD_DEMOS)
    # executable
    add_executable(testRAIGraphics raiGraphicsTest.cpp)
    target_link_libraries(testRAIGraphics ${TARGETS_LIB_NAME})

    # executable
    add_executable(testRAIGraphics2 raiGraphicsTest2.cpp)
    target_link_libraries(testRAIGraphics2 ${TARGETS_LIB_NAME})
endif(BUILD_DEMOS)

# --------------------------------------------------------------------
# install
if(INSTALL_LIBS)
    # package raiGraphics
    export(PACKAGE ${PROJECT_NAME})

    # build tree
    set(CONF_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include")

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