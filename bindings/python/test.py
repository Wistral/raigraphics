from raigraphics import PyRaiGraphics, PySphere
from time import sleep

sim = PyRaiGraphics(800, 600)

sphere = PySphere(1.0)
sim.addSphere(sphere)

sim.start()

while not sim.isQuitting():
    sleep(1e-6)

sim.end()