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

find_package( Eigen3 REQUIRED )
include_directories( ${EIGEN3_INCLUDE_DIR} )
link_directories(/usr/lib)

INCLUDE(FindPkgConfig)

# include directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(rai_graphics ${SRCfiles})
set(RAI_Graphics rai_graphics glut GLU GL GLEW SDL2 assimp SOIL glog freeimage pthread)
add_executable(testRAIGraphics test.cpp)
target_link_libraries(testRAIGraphics ${RAI_Graphics})

install(FILES ${objHDRfiles} DESTINATION ${CMAKE_BINARY_DIR}/include/obj)
install(FILES ${impHDRfiles} DESTINATION ${CMAKE_BINARY_DIR}/include/imp)
install(FILES RAI_graphics.hpp RAI_keyboard.hpp DESTINATION ${CMAKE_BINARY_DIR}/include)
install(TARGETS rai_graphics EXPORT RAITargets DESTINATION ${CMAKE_BINARY_DIR}/lib)

# set RAI_LINK
set(RAI_LINK glog gflags CACHE INTERNAL "")
set(RAI_LINK rai_logger rai_timer rai_graph ${RAI_LINK})
set(RAI_LINK ${RAI_Graphics} ${RAI_LINK})