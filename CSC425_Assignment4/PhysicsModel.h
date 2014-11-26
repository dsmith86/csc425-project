#define SMOOTHING 0.0001

#pragma once
class PhysicsModel
{
public:
	PhysicsModel(float acceleration, float mass);
	~PhysicsModel();
	void jump(float impulse);
	float deltaY(float deltaTime);
	void contactedGround();
	bool onGround;
	bool jumpIsPending();
private:
	float yVelocity;
	float yAcceleration;
	float mass;
	bool jumpPending;
};

