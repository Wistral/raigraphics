from libcpp cimport bool

cdef extern from "raiGraphics/RAI_graphics.hpp" namespace "rai_graphics":
    cdef cppclass RAI_graphics:
        RAI_graphics(int, int);
        void start();
        void end();
        int isQuitting();


cdef class PyRaiGraphics:
    cdef RAI_graphics *thisptr      # hold a C++ instance which we're wrapping

    def __cinit__(self, int window_width, int window_height):
        self.thisptr = new RAI_graphics(window_width, window_height)

    def __dealloc__(self):
        del self.thisptr

    def start(self):
        return self.thisptr.start()

    def end(self):
        return self.thisptr.end()

    def isQuitting(self):
        return self.thisptr.isQuitting()