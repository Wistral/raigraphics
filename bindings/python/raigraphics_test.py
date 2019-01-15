from raigraphics import PyRaiGraphics, PySphere, PyCone, PyBox, PyCylinder, PyCapsule, PyCheckerboard
from time import sleep

sim = PyRaiGraphics(800, 600)

sphere = PySphere(1)
cone = PyCone(1, 2)
box = PyBox(2, 2, 2)
cylinder = PyCylinder(1, 2)
capsule = PyCapsule(1, 2)
checkerboard = PyCheckerboard(5, 100, 100)

sim.add_sphere(sphere)
sim.add_cone(cone)
sim.add_box(box)
sim.add_cylinder(cylinder)
sim.add_capsule(capsule)
sim.add_checkerboard(checkerboard)

sphere.set_position(0, 0, 3)
cone.set_position(10, 0, 3)
box.set_position(0, 10, 3)
cylinder.set_position(0, 0, 10)
capsule.set_position(0, -10, 3)

sim.start()
while not sim.is_quitting():
    sleep(1e-6)
sim.end()