#include "Camera.h"
#include <gtx\rotate_vector.hpp>
#include <iostream>

Camera::Camera()
{
	this->position = glm::vec3(0.0, 0.0, -1.0);
	this->gaze = glm::vec3(0.0, 0.0, 0.0);
	this->up = glm::vec3(0.0, 1.0, 0.0);
}

Camera::Camera(glm::vec3 position, glm::vec3 gaze)
{
	this->position = position;
	this->gaze = glm::normalize(gaze - position);
	this->front = this->gaze;
	this->up = glm::normalize(glm::cross(glm::cross(this->front, glm::vec3(0.0, 1.0, 0.0)), this->front));
	this->right = glm::normalize(glm::cross(this->front, this->up));

}

Camera::~Camera()
{
}

bool Camera::valid()
{
	return (&this->position && &this->gaze && &this->up);
}


void Camera::pitch(float y)
{
	this->up = glm::normalize(glm::rotate(this->up, y, this->right));
	this->front = glm::normalize(glm::rotate(this->front, y, this->right));
	this->gaze = this->position + this->front;
}

void Camera::yaw(float x)
{
	this->right = glm::rotate(this->right, -x, this->up);
	this->front = glm::rotate(this->front, -x, this->up);
	this->gaze = this->position + this->front;
}

void Camera::translate(glm::vec3 direction)
{
	this->position = this->position + direction;
	this->gaze = this->gaze + direction;
}