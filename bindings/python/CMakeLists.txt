# variables
#
# - RAI_GRAPHICS_OPENGL_ROOT
# - EIGEN3_INCLUDE_DIR
# - Boost_LIBRARIES
# - SYSTEM_LIBRARIES

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/path.py.in
        ${CMAKE_CURRENT_SOURCE_DIR}/raigraphics_path.py @ONLY)

add_custom_target(pyraigraphics
        COMMAND python setup.py build_ext --inplace)