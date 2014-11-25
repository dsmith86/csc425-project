#include "PhysicsModel.h"
#include <iostream>

PhysicsModel::PhysicsModel(float acceleration, float mass)
{
	this->yAcceleration = acceleration;
	this->mass = mass;
	this->yVelocity = 0;
	this->jumpPending = false;
}


PhysicsModel::~PhysicsModel()
{
}

bool PhysicsModel::jumpIsPending()
{
	return this->jumpPending;
}

// Impulse is in Newton-seconds
void PhysicsModel::jump(float impulse)
{
	if (this->yVelocity > 0)
	{
		return;
	}

	// deltaV = impulse / mass
	this->yVelocity = impulse / this->mass;
	this->jumpPending = true;
	this->onGround = false;
}

float PhysicsModel::deltaY(float deltaTime)
{
	if (onGround)
	{
		return 0;
	}

	this->yVelocity -= this->yAcceleration;

	return this->yVelocity * deltaTime * SMOOTHING;
}

void PhysicsModel::contactedGround()
{
	this->yVelocity = 0;
	this->jumpPending = false;
	this->onGround = true;
}