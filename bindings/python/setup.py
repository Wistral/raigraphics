import os

from distutils.core import setup
from distutils.extension import Extension
from Cython.Build import cythonize

PROJECT_NAME = 'pyraigraphics'

# source
ROOT_DIR = '../../'                 # project root
SRC_DIR = ROOT_DIR                  # source dir
HDR_DIR = ROOT_DIR + 'include'      # header dir

# compiler
COMPILER = '/usr/bin/g++-7'         # compiler path
os.environ["CC"] = COMPILER
os.environ["CXX"] = COMPILER

# libs
EIGEN = '/usr/include/eigen3'       # eigen path
BOOST = '/usr/lib/x86_64-linux-gnu' # boost path

############

raigraphics_extension = Extension(
    PROJECT_NAME,
    sources=['raigraphics.pyx'],
    libraries=['raigraphics',
               'GL',
               'GLEW',
               'SDL2',
               'SDL2_ttf',
               'assimp',
               'SOIL',
               'pthread',
               'boost_thread',
               'boost_system',
               'boost_filesystem'],
    library_dirs=['/usr/local/lib', '/usr/lib', BOOST],
    include_dirs=[HDR_DIR],
    extra_compile_args=['-std=c++11', '-pthread', '-g', '-isystem' + EIGEN],
    language='c++',
)

setup(
    name=PROJECT_NAME,
    author='donghokang',
    ext_modules=cythonize([raigraphics_extension]),
)
