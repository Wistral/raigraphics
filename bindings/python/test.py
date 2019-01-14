from raigraphics import PyRaiGraphics
from time import sleep

sim = PyRaiGraphics(800, 600)
sim.start()

while not sim.isQuitting():
    sleep(1e-6)

sim.end()