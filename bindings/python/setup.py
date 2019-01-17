import os

from setuptools import setup
from setuptools.extension import Extension
from Cython.Build import cythonize

import pyraigraphics_path

# compiler
os.environ["CC"] = pyraigraphics_path.COMPILER
os.environ["CXX"] = pyraigraphics_path.COMPILER

############

raigraphics_extension = Extension(
    pyraigraphics_path.PROJECT_NAME,
    sources=[
        'raigraphics.pyx',
    ],
    libraries=[
        pyraigraphics_path.RAIGRAPHICS,
        pyraigraphics_path.LIB_GL,
        pyraigraphics_path.LIB_GLEW,
        pyraigraphics_path.LIB_SDL2,
        pyraigraphics_path.LIB_SDL2_ttf,
        pyraigraphics_path.LIB_assimp,
        pyraigraphics_path.LIB_SOIL,
        pyraigraphics_path.LIB_pthread,
        'boost_thread',
        'boost_system',
        'boost_filesystem',
    ],
    library_dirs=['/usr/local/lib', '/usr/lib', pyraigraphics_path.BOOST],
    include_dirs=[pyraigraphics_path.HDR_DIR],
    extra_compile_args=['-std=c++11', '-pthread', '-g', '-isystem' + pyraigraphics_path.EIGEN],
    language='c++',
)

setup(
    name=pyraigraphics_path.PROJECT_NAME,
    author='donghokang',
    ext_modules=cythonize([raigraphics_extension]),
)
