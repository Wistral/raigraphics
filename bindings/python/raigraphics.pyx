from libcpp cimport bool

cdef extern from "raiGraphics/obj/Sphere.hpp" namespace "rai_graphics::object":
    cdef cppclass Sphere:
        Sphere(float);

cdef extern from "raiGraphics/obj/Box.hpp" namespace "rai_graphics::object":
    cdef cppclass Box:
        Box(float, float, float);

cdef extern from "raiGraphics/obj/Capsule.hpp" namespace "rai_graphics::object":
    cdef cppclass Capsule:
        Capsule(float, float);


cdef extern from "raiGraphics/RAI_graphics.hpp" namespace "rai_graphics":
    cdef cppclass RAI_graphics:
        RAI_graphics(int, int);
        void start();
        void end();

        void addObject(Sphere *obj);

        int isQuitting();

cdef class PySphere:
    cdef Sphere *thisptr

    def __cinit__(self, float radius):
        self.thisptr = new Sphere(radius)

    def __dealloc__(self):
        del self.thisptr

cdef class PyBox:
    cdef Box *thisptr

    def __cinit__(self, float xLength, float yLength, float zLength):
        self.thisptr = new Box(xLength, yLength, zLength)

    def __dealloc__(self):
        del self.thisptr

cdef class PyRaiGraphics:
    cdef RAI_graphics *thisptr

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

    def addSphere(self, PySphere sphere):
        return self.thisptr.addObject(sphere.thisptr)