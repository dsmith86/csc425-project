#include <vec3.hpp>
#include <glm.hpp>

#define DEFAULT_SMOOTHING 0.4

#pragma once
class Camera
{
public:
	Camera();
	Camera(glm::vec3 position, glm::vec3 gaze);
	~Camera();
	enum DIRECTION{ FORWARD, BACK, LEFT, RIGHT };
	bool valid();
	void setSmoothing(float smoothing);
	void pitch(float y);
	void yaw(float x);
	void translate(glm::vec3 direction, float userSmoothing = 1.0f);
	void translate(DIRECTION direction, float userSmoothing = 1.0f);
	glm::vec3 position;
	glm::vec3 gaze;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 front;
	bool cursorActive;
private:
	float smoothing;
};

