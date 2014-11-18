csc425-assigment4
=================
### Build Instructions:

Add the opengl redbook headers and libs to
```
C:\opengl\
```
or make the necessary changes to the project settings.

Additionally add the include directory for [GLM](http://glm.g-truc.net/0.9.5/index.html) to the c:\opengl directory.

### Description:

This is an ongoing project that is constantly improved as I learn more advanced graphics programming concepts and OpenGL skills.

Currently, it has the following features:
* the ability to define and load in an arbitrary combination of different vertex and fragment shaders as "materials"
* the ability to programmatically load the total number of models set up in main.cpp
* model, view, projection and world coordinates
* model rotation in place
* texture mapping
* some basic phong-esque shading

The world currently contains a collection of cubes (right now, 12 arranged in a circle) that can rotate freely in space.
Each is mapped with a Minecraft-like dirt texture (albeit at a lower resolution).

### Future plans:
* mouse and keyboard events to move around the scene
* fancier lighting
* shadows
* AI
