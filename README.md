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

Here is the scene with complex shading on:

![](http://i.imgur.com/KFBvqT2.png)

And here it is with complex shading off:

![](http://i.imgur.com/apF8nR4.png)

Rather large textures will definitely work, but they'll take a while to load in from disk. Here is a 2048x2048 texture applied to a cube:

![](http://i.imgur.com/s4YSv6w.png)

The skybox was created by using two different textures (one for each wall, and one for the ceiling. First, I took an image of clouds and offset the image in Photoshop so that I could make the edges line up almost seamlessly:

![](http://i.imgur.com/8qSuTDk.png)

I then layed out the resulting image four times in the following pattern:

![](http://i.imgur.com/DEg2GJA.png)

I then filled in the middle by cloning certain regions of the original image to create my own artificial sky portion for the ceiling:

![](http://i.imgur.com/csWL0aS.png) ![](http://i.imgur.com/UQNFEIm.png) 

And here's the resulting ceiling:

![](http://i.imgur.com/ZH5xj6m.png)

Here are some screenshots of a few scenes in the simulation, both with complex shading and without:

![](http://i.imgur.com/KFBvqT2.png) ![](http://i.imgur.com/apF8nR4.png) ![](http://i.imgur.com/NqEYaiT.png) ![](http://i.imgur.com/ejZ6mtx.png) ![](http://i.imgur.com/gZLBhpz.png) ![](http://i.imgur.com/4SpcBpo.png) ![](http://i.imgur.com/qg5lksE.png) ![](http://i.imgur.com/3pTBaN2.png) ![](http://i.imgur.com/2w98jAu.png) ![](http://i.imgur.com/6wbXCn7.png) ![](http://i.imgur.com/4HgLt6m.png) ![](http://i.imgur.com/Jm0VRru.png) 

### Controls:
#### Movement
*W*, *A*, *S*, *D* move the camera forward, left, back, and right, respectively. The *Shift* modifier allows finer movement. Any combination of *WASD* can be used to move diagonally.

**[Click here for a demonstration.](http://gfycat.com/DefiantDisfiguredGreathornedowl)**

The *arrow keys* can be used to rotate the first-person camera to the left or right, or change the pitch up or down. Additionally, holding down the right mouse button and moving the mouse can be used to accomplish the same thing.

**[Click here for a demonstration.](http://gfycat.com/SpanishVelvetyAfghanhound)**

#### Light controls
Complex lighting can be toggled on and off using the *T* key.

**[Click here for a demonstration.](http://gfycat.com/WellinformedRashHorsechestnutleafminer)**

Additionally, the light can be moved using the *IJKL* keys. They can also be raised/lowered using *[* and *]*, respectively. Light movement takes place in their respective directions from the perspective of the camera.

**[Click here for a demonstration.](http://gfycat.com/PastelEasygoingHare)**

#### Miscellaneous
The program can be closed by hitting *Q*.

### Discussion

#### Keyboard input
Keyboard input is handled using a special KeyBuffer class I wrote. This class simply sets and unsets keys in unordered maps in order to track their state. Here is how that's done:

```cpp
void KeyBuffer::set(unsigned char key)
{
	this->keys[key] = true;
}

void KeyBuffer::unset(unsigned char key)
{
	this->keys[key] = false;
}

bool KeyBuffer::isSet(unsigned char key)
{
	if (this->keys.find(key) != this->keys.end())
	{
		return this->keys[key];
	}
	return false;
}
```

When a key down event is triggered, that key is set using KeyBuffer#set. When a key up event is triggered, that key is unset using KeyBuffer#unset.

#### Camera
Rotating the camera is as simple as a bit of vector math. I keep track of an up, front, and right vector for my camera, and I rotate based on those vectors. Here's a sample:

```cpp
void Camera::pitch(float y)
{
	this->up = glm::normalize(glm::rotate(this->up, y, this->right));
	this->front = glm::normalize(glm::rotate(this->front, y, this->right));
	this->gaze = this->position + this->front;
}

void Camera::yaw(float x)
{
	this->right = glm::rotate(this->right, x, glm::vec3(0, 1, 0));
	this->front = glm::rotate(this->front, x, glm::vec3(0, 1, 0));
	this->gaze = this->position + this->front;

	// correct up vector's tilt
	this->right.y = 0;
	this->up = glm::cross(this->right, this->front);
}
```

That's pretty much it! Moving the camera is even simpler:

```cpp
void Camera::translate(glm::vec3 direction, float userSmoothing)
{
	this->position = this->position + direction * this->smoothing * userSmoothing;
	this->gaze = this->gaze + direction * this->smoothing * userSmoothing;
}
```

####Lighting

Lighting is computed per fragment. Here is the fragment shader:

```glsl
#version 430 core

in vec4 color;
in vec2 Texcoord;
in vec3 position;
in vec3 normal;

out vec4 fColor;

uniform sampler2D uTex;
uniform sampler2D uNormalMap;
uniform vec3 uLight;
uniform bool uLightingEnabled;
uniform bool uNormalMapEnabled;

void main()
{
	float lightMultiplier = 1;

	if (uLightingEnabled)
	{
		vec3 light = uLight - position;
		float lightDistance = length(light);
		light = normalize(light);

		float lightIntensity = 30.0;
		float diffuseConstant = 0.1;

		float ambient = 0.05;
		float diffuse = diffuseConstant * max(dot(light, normal), 0.0) * lightIntensity / lightDistance;

		lightMultiplier = ambient + diffuse;
	}

	fColor = texture(uTex, Texcoord) * color * lightMultiplier;

	
}
```

As you can see, the amount of light contributed by diffuse shading is simply derived from the dot product of the light direction unit vector and the surface normal. Dividing by lightDistance produces a nice falloff effect that makes a light less intense as the distance between a light and some surface is increased.

####Textures
Textures are loaded using the third-party library stb_image.h. I've simply included it in my project, so no additional dependencies are needed. I've only tested with PNG files, but other file types should be supported as well. The image loader detects whether an image has an alpha channel or not, and my code can react accordingly. I've figured out how to pass two or more textures in to the same shader, and I plan to eventually do this to add normal maps to certain objects.

Materials can be defined like so:

```cpp
GLContext::material materials[] = {
			{ "wood", "texture.vert", "texture.frag", "wood.png", NULL, 1 },
			{ "rubiks", "texture.vert", "texture.frag", "rubiks.png", NULL, 1 },
			{ "rust", "texture.vert", "texture.frag", "rust.png", NULL, 1 },
			{ "fabric", "texture.vert", "texture.frag", "fabric.png", NULL, 1 },
			{ "nicolascage", "texture.vert", "texture.frag", "nicolascage.png", NULL, 1 },
			{ "ground", "texture.vert", "texture.frag", "ground.png", NULL, 100 },
			{ "sky", "texture.vert", "texture.frag", "sky.png", NULL, 1 },
			{ "ceiling", "texture.vert", "texture.frag", "ceiling.png", NULL, 1 },
	};
```

Then, they can be applied to whatever model they need to be:

```cpp
place_cubes(models, 1, 1, glm::vec3(0, 0, 0), "wood");
	place_cubes(models, 1, 1, glm::vec3(5, 0, 0), "rubiks");
	place_cubes(models, 1, 10, glm::vec3(-20, 0, 0), "rust");
	place_cubes(models, 1, 5, glm::vec3(20, 0, 0), "fabric");
	place_cubes(models, 4, 3, glm::vec3(2, 0, 10), "nicolascage");
```

Future plans (depending on work and school schedules):
* Shadow mapping
* Bump mapping
* Tessellation for displacement mapping
* Lerp functions for simple animations
* OBJ support
* Geometry shading for light flare effects, etc.
