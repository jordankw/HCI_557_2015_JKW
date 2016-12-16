#pragma once
#ifndef CAR_H
#define CAR_H

#include <string>
#include <stdio.h>
#include <vector>

// GLM include files
#define GLM_FORCE_INLINE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_inverse.hpp>


using namespace std;
class Car {
public:
	Car();
	Car(vector<float> position);
	void update(float dt);
	enum State {idle, forwardThrottle, brake, reverseThrottle};
	State currentState;
	bool isTurning;
	bool isLeft;
	float getSpeed(void);
	float getTurn(void);
	glm::vec3 getVel(void);
	glm::vec3 getPos(void);
	glm::mat4 getTransform();
	glm::vec3 getForward();
	glm::vec3 getNorm();
	glm::vec3 getSide();
	float getWheelRot();
	static const float accel;
	static const float topSpeed;
	//vector<float> getVel();
	//vector<float> getPos();

protected:
	glm::vec3 normVec;
	glm::vec3 forwardVec;
	glm::vec3 sideVec;
	glm::mat4 carTransform;
	glm::quat carT;
	float speed;
	glm::vec3 vel;
	glm::vec3 pos;
	float wheelRot;
	float wheelTurn; //-1 = all the way left, 0 = straight, 1 = all the way right

};

#endif