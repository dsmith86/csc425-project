#include <vec3.hpp>

#pragma once
class Camera
{
public:
	Camera();
	Camera(glm::vec3 position, glm::vec3 gaze);
	~Camera();
	bool valid();
	glm::vec3 getPosition();
	glm::vec3 getGaze();
	glm::vec3 getUp();
private:
	glm::vec3 position;
	glm::vec3 gaze;
	glm::vec3 up;
};

