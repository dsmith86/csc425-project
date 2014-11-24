#include <vec3.hpp>
#include <glm.hpp>



#pragma once
class Camera
{
public:
	Camera();
	Camera(glm::vec3 position, glm::vec3 gaze);
	~Camera();
	enum DIRECTION{ FORWARD, BACK, LEFT, RIGHT };
	bool valid();
	void pitch(float y);
	void yaw(float x);
	void translate(DIRECTION direction);
	glm::vec3 position;
	glm::vec3 gaze;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 front;
private:

};

