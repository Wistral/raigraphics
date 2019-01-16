# distutils: language = c++

from libcpp cimport bool

from singlebody cimport SingleBodyObject, Box, Capsule, CheckerBoard, Cone, Cylinder, Sphere

cdef class PySingleBodyObject:
    cdef SingleBodyObject *thisptr

    def __cinit__(self):
        self.thisptr = new SingleBodyObject()

    def __dealloc__(self):
        del self.thisptr

    def set_position(self, double x, double y, double z):
        return self.thisptr.setPos(x, y, z)

    def set_orientation(self, double w, double x, double y, double z):
        return self.thisptr.setOri(w, x, y, z)

cdef class PyBox(PySingleBodyObject):
    # cdef Box *thisptr

    def __cinit__(self, float xlength, float ylength, float zlength):
        self.thisptr = new Box(xlength, ylength, zlength)

cdef class PyCapsule(PySingleBodyObject):
    # cdef Capsule *thisptr

    def __cinit__(self, float r, float l):
        self.thisptr = new Capsule(r, l)

cdef class PyCheckerboard(PySingleBodyObject):
    # cdef CheckerBoard *thisptr

    def __cinit__(self, float gridsize, float width, float length):
        self.thisptr = new CheckerBoard(gridsize, width, length, 0.05)

cdef class PyCone(PySingleBodyObject):
    # cdef Cone *thisptr

    def __cinit__(self, float r, float l):
        self.thisptr = new Cone(r, l)

cdef class PyCylinder(PySingleBodyObject):
    # cdef Cylinder *thisptr

    def __cinit__(self, float r, float l):
        self.thisptr = new Cylinder(r, l)

cdef class PySphere(PySingleBodyObject):
    # cdef Sphere *thisptr

    def __cinit__(self, float radius):
        self.thisptr = new Sphere(radius)

cdef extern from "raiGraphics/RAI_graphics.hpp" namespace "rai_graphics":
    cdef cppclass RAI_graphics:
        RAI_graphics(int, int);
        void start();
        void end();

        void addObject(SingleBodyObject *obj);

        int isQuitting();

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

    def is_quitting(self):
        return self.thisptr.isQuitting()

    def add_sphere(self, PySphere sphere):
        return self.thisptr.addObject(sphere.thisptr)

    def add_box(self, PyBox box):
        return self.thisptr.addObject(box.thisptr)

    def add_capsule(self, PyCapsule capsule):
        return self.thisptr.addObject(capsule.thisptr)

    def add_checkerboard(self, PyCheckerboard checkerboard):
        return self.thisptr.addObject(checkerboard.thisptr)

    def add_cone(self, PyCone cone):
        return self.thisptr.addObject(cone.thisptr)

    def add_cylinder(self, PyCylinder cylinder):
        return self.thisptr.addObject(cylinder.thisptr)
