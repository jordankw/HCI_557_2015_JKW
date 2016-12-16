#include "car.h"
using namespace std;

const float Car::accel = 3.5f;
const float Car::topSpeed = 3.f;
const float pi2 = 3.1415926535f * 2;

Car::Car() {
	currentState = idle;

	speed = 0.0f;
	vel[0] = 0.0f; vel[1] = 0.0f; vel[2] = 0.0f;

	pos[0] = 0.0f;
	pos[1] = 0.0f;
	pos[2] = 0.0f;
}
Car::Car(vector<float> position) {
	currentState = idle;

	speed = 0.0f;
	vel[0] = 0.0f; vel[1] = 0.0f; vel[2] = 0.0f;

	pos[0] = position[0];
	pos[1] = position[1];
	pos[2] = position[2];
}
void Car::update(float dt) {
	glm::quat q;
	

	normVec = (glm::vec3)(glm::mat4_cast(carT))[1];
	forwardVec = (glm::vec3)(glm::mat4_cast(carT))[2];
	sideVec = (glm::vec3)(glm::mat4_cast(carT))[0];

	//accelerate
	if (currentState == forwardThrottle) {
		speed += accel * dt;
	}
	else if (currentState == reverseThrottle) {
		speed -= accel * dt;
	}

	//speed *= 1.f - (abs(wheelTurn) * 0.05f);

	if (speed > topSpeed) {
		speed = topSpeed;
	}
	else if (speed < (-1 * topSpeed)) {
		speed = (-1 * topSpeed);
	}

	else if (currentState == idle) {
		if (abs(speed) < 0.1) {
			speed = 0;
		}
		else {
			if (speed > 0) {
				speed -= (dt * 3) + abs(wheelTurn) * dt;
			}
			else if (speed < 0) {
				speed += dt * 3;
			}
		}
		//speed -= dt;
	}

	//turn wheel
	if (isTurning) {
		if (!isLeft) {
			wheelTurn -= dt * 3;// *abs(speed);
		}
		else {
			wheelTurn += dt * 3;// *abs(speed);
		}
	}

	
	// else, return wheel to straight
	else {
		if (abs(wheelTurn) < 0.05) {
			wheelTurn = 0;
		} else if (wheelTurn > 0) {
			wheelTurn -= dt;
		}
		else if (wheelTurn < 0) {
			wheelTurn += dt;
		}
	}
	
	//check for max turn
	if (wheelTurn > 0.3f) {
		wheelTurn = 0.3f;
	} else if (wheelTurn < -0.3f) {
		wheelTurn = -0.3f;
	}

	wheelTurn *= 1.f - (abs(speed) / 20.f);

	wheelRot += speed / 5.f;
	//check for rotation
	if (wheelRot < 0) {
		wheelRot += pi2;
	}
	else if (wheelRot > pi2) {
		wheelRot -= pi2;
	}

	
	pos += forwardVec * speed;
	pos += sideVec * speed * wheelTurn * 1.f;
	carT *= glm::rotate(q, speed * dt * wheelTurn * 8, normVec);
	
}
float Car::getSpeed() {
	return speed;
}
float Car::getTurn() {
	return wheelTurn;
}
glm::vec3 Car::getVel() {
	return vel;
}
glm::vec3 Car::getPos() {
	return pos;
}
glm::vec3 Car::getForward() {
	return forwardVec;
}
glm::vec3 Car::getNorm() {
	return normVec;
}
glm::vec3 Car::getSide() {
	return sideVec;
}
float Car::getWheelRot() {
	return wheelRot;
}
glm::mat4 Car::getTransform() {
	//return carTransform;
	return (glm::translate(pos) * glm::mat4_cast(carT));
}
