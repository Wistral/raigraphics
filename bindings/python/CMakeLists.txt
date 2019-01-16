# variables
#
# - RAI_GRAPHICS_OPENGL_ROOT
# - EIGEN3_INCLUDE_DIR
# - Boost_LIBRARIES
# - SYSTEM_LIBRARIES

# python >= 3.5 is required
find_package(PythonInterp 3.5 REQUIRED)

# find libraigraphics.a
find_library(RAI_GRAPHICS_PATH
        raigraphics
        HINTS ${CMAKE_BINARY_DIR}
        NO_DEFAULT_PATH
        )

message(${RAI_GRAPHICS_PATH})

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/path.py.in
        ${CMAKE_CURRENT_SOURCE_DIR}/pyraigraphics_path.py @ONLY)

# build pyraigraphics
add_custom_target(pyraigraphics_build
        COMMAND ${PYTHON_EXECUTABLE} setup.py build_ext --inplace
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )

add_dependencies(pyraigraphics_build ${TARGETS_LIB_NAME})

# clean pyraigraphics
add_custom_target(pyraigraphics_clean
        COMMAND rm -rf ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/build ${CMAKE_CURRENT_SOURCE_DIR}/*.so
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )