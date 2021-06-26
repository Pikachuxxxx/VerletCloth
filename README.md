# VerletCloth
A simple implementation of Verlet cloth in OpenGL.

The particles are constrained using their rest distance, I used verlet integration to calculate the change in position of the particles. The corners of the cloth are just fixed using a boolean flag to not integrate them. Collision resolution is only available with a sphere using a simple radius check of the particle and the sphere.

![](https://github.com/Pikachuxxxx/VerletCloth/blob/master/verletcloth.png)

# controls
Right click on the mouse and use WASD to move around the scene.
Use the imgui controls to tweak the behaviour of the cloth.
