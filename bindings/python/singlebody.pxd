cdef extern from "raiGraphics/obj/SingleBodyObject.hpp" namespace "rai_graphics::object":
    cdef cppclass SingleBodyObject:
        void setPos(double, double, double);
        void setOri(double, double, double, double);

cdef extern from "raiGraphics/obj/Sphere.hpp" namespace "rai_graphics::object":
    cdef cppclass Sphere(SingleBodyObject):
        Sphere(float);

cdef extern from "raiGraphics/obj/Box.hpp" namespace "rai_graphics::object":
    cdef cppclass Box(SingleBodyObject):
        Box(float, float, float);

cdef extern from "raiGraphics/obj/Capsule.hpp" namespace "rai_graphics::object":
    cdef cppclass Capsule(SingleBodyObject):
        Capsule(float, float);

cdef extern from "raiGraphics/obj/CheckerBoard.hpp" namespace "rai_graphics::object":
    cdef cppclass CheckerBoard(SingleBodyObject):
        CheckerBoard(float, float, float, float);

cdef extern from "raiGraphics/obj/Cone.hpp" namespace "rai_graphics::object":
    cdef cppclass Cone(SingleBodyObject):
        Cone(float, float);

cdef extern from "raiGraphics/obj/Cylinder.hpp" namespace "rai_graphics::object":
    cdef cppclass Cylinder(SingleBodyObject):
        Cylinder(float, float);