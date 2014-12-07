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
* Support for multiple materials, each with their own shaders and textures
* Diffuse lighting with distance-based falloff
* Full camera controls (*WASD* to move, *arrow keys* or *mouse* to rotate the view)
* A controllable light (*IJKL* to move, *[* and *]* to change height

### Controls:
#### Movement
*W*, *A*, *S*, *D* move the camera forward, left, back, and right, respectively. The *Shift* modifier allows finer movement. Any combination of *WASD* can be used to move diagonally.

**Click here for a demonstration:**
[![Demonstration](http://thumbs.gfycat.com/DefiantDisfiguredGreathornedowl-poster.jpg)](http://gfycat.com/DefiantDisfiguredGreathornedowl)

The *arrow keys* can be used to rotate the first-person camera to the left or right, or change the pitch up or down. Additionally, holding down the right mouse button and moving the mouse can be used to accomplish the same thing.
