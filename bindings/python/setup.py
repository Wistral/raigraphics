import os

from distutils.core import setup
from distutils.extension import Extension
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
        'raigraphics',
        'GL',
        'GLEW',
        'SDL2',
        'SDL2_ttf',
        'assimp',
        'SOIL',
        'pthread',
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
