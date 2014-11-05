#include "Camera.h"

Camera::Camera()
{
	this->position = glm::vec3(0.0, 0.0, -1.0);
	this->gaze = glm::vec3(0.0, 0.0, 0.0);
	this->up = glm::vec3(0.0, 1.0, 0.0);
}

Camera::Camera(glm::vec3 position, glm::vec3 gaze)
{
	this->position = position;
	this->gaze = gaze;
	this->up = glm::vec3(0.0, 1.0, 0.0);
}

Camera::~Camera()
{
}

bool Camera::valid()
{
	return (&this->position && &this->gaze && &this->up);
}

glm::vec3 Camera::getPosition()
{
	return this->position;
}

glm::vec3 Camera::getGaze()
{
	return this->gaze;
}

glm::vec3 Camera::getUp()
{
	return this->up;
}