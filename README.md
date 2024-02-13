# Compilation instructions:
In the root directory, run `make`.
If this doesn't work, I am so sorry. I have no further words. I made a WSL distro soley for this project because I struggled so much with installing GLEW/GLFW3.

The program is compiled to build/raytracer.exe Run it with the -a flag to render a predefined animation (exported to output/). 

Otherwise, the program runs in a `live` mode:
WASD: move
LEFT/RIGHT ARROW: Cycle object look direction focus
UP ARROW: Look at origin

P: Perspective
O: Orthographic

Note: input is processed only on frame updates, so you'll probably have to press/hold.