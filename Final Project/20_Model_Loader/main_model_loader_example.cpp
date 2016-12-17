//
//  HCI 557 Final Project
//  Jordan Williams
//
//  
//  
//

// stl include
#include <iostream>
#include <string>

// GLEW include
#include <GL/glew.h>

// GLM include files
#define GLM_FORCE_INLINE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>


// glfw includes
#include <GLFW/glfw3.h>


// include local files
#include "common/controls.h"
#include "common/HCI557Common.h"
#include "common/CoordSystem.h"
#include "common/Sphere3D.h"
#include "ImgLoader.h"

#include "common/GLObjectObj.h"
#include "Car.h"
using namespace std;


// The handle to the window object
GLFWwindow*         window;

// Define some of the global variables we're using for this sample
GLuint program;
GLuint program2, program3, program4;
/* A trackball to move and rotate the camera view */
extern Trackball trackball;

glm::mat4 tranform = glm::scale(glm::vec3(10.0, 10.0f, 10.0f));
int startTime = 0;
const float pi2 = 2.0f * 3.1415926535;
bool throttle = false;
bool rev = false;
bool isLeft = false;
bool isTurning = false;
bool reset = false;
bool headlights = false;
unsigned char * tex;
float prevTime = 0.f;
vector<glm::vec2> conePositions;
vector<glm::vec3> streetPositions;
enum CameraView {follow, stationary};
CameraView cv;
//declare car object
Car blueCar({ -2.f,0.f,1.f });
glm::vec4 initLightPos;
glm::vec3 carLocation;
glm::mat4 carRotation;

void moveCar(GLFWwindow *wind, int key, int scancode, int action, int mods) {
	// Forward
	if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
		if (!throttle) {
			rev = false;
			throttle = true;
		}
		else if (throttle) {
			rev = false;
		}
	}
	else if (key == GLFW_KEY_UP && action == GLFW_RELEASE) {
		if (throttle && !rev) {
			throttle = false;
		}
	}
	// Backward
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
		rev = true;
		if (!throttle) {
			
			throttle = true;
		}
		else if (throttle) {
			rev = true;
		}
	}
	else if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE) {
		if (throttle && rev) {
			throttle = false;
		}
	}
	// Left
	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
		isLeft = true;
		isTurning = true;
	}
	else if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE) {
		if (isLeft) {
			isTurning = false;
		}
	}
	// Right
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
		isLeft = false;
		isTurning = true;
	}
	else if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE) {
		if (!isLeft) {
			isTurning = false;
		}
	}
		
	// Toggle headlights on
	if (key == GLFW_KEY_H && action == GLFW_PRESS) {
		if (headlights) {
			headlights = false;

		}
		else {
			headlights = true;
		}
		
	}

	// Change camera view
	if (key == GLFW_KEY_V && action == GLFW_RELEASE) {
		if (cv == stationary) {
			cv = follow;
		}
		else {
			cv = stationary;
		}
	}
	if (key == GLFW_KEY_B && action == GLFW_PRESS) {
		trackball.resetTrackball();
		
		reset = true;
	}
}

int main(int argc, const char * argv[])
{
	srand(glfwGetTime());
	carLocation = glm::vec3(0, 0, 0);
	carRotation = glm::rotate(0.0f, glm::vec3(0, 1, 0));
	
	//for (float i = 0; i <= pi2; i += (pi2 / 24)) {
	//	conePositions.push_back(glm::vec2(sin(i), cos(i)) * 10.f);
	//	conePositions.push_back(glm::vec2(sin(i), cos(i)) * 20.f);
	//}

	for (float i = 0; i <= 50; i += 1.f) {
		float xPosRand = ((rand() / (float)RAND_MAX) - 0.5f) * 1.f;
		conePositions.push_back(glm::vec2(xPosRand, 2.f * i) * 10.f);
		conePositions.push_back(glm::vec2(xPosRand + 1.f, 2.f * i) * 10.f);
		streetPositions.push_back(glm::vec3(0, 0, i * 10.f));
		//streetPositions.push_back(glm::vec3(0, 0, i * 20.f));
	}


	vector<glm::vec2>::iterator it;
	vector<glm::vec3>::iterator it2;
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Init glfw, create a window, and init glew
    
    // Init the GLFW Window
    window = initWindow();
	//glfwSetWindowSize(window, 900, 900);
    
    // Init the glew api
    initGlew();
    SetCameraManipulator(CameraTypes::CAMERA_MANIPULATOR);

    
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Init Keyboard input

	glfwSetKeyCallback(window, moveCar);


    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    

	
	//// Create some models
    
    // coordinate system
    CoordSystem* cs = new CoordSystem(40.0);
    
    
    // create an appearance object.
    GLAppearance* appearance_0 = new GLAppearance("../shaders/multi_pixel_lights.vs", "../shaders/multi_pixel_lights.fs");
	GLAppearance* appearance_1 = new GLAppearance("../shaders/multi_vertex_lights.vs", "../shaders/multi_vertex_lights.fs");
	GLAppearance* appearance_2 = new GLAppearance("../shaders/multi_vertex_lights.vs", "../shaders/multi_vertex_lights.fs");
	GLAppearance* appearance_3 = new GLAppearance("../shaders/displacement_texture.vs", "../shaders/displacement_texture.fs");


	//Create Lights
    

	GLPointLightSource point_1;
	point_1._lightPos = glm::vec4(0.0, 50.0, 0.0, 0.0);
	point_1._ambient_intensity = 0.8;
	point_1._specular_intensity = 5.5;
	point_1._diffuse_intensity = 2.0;
	point_1._attenuation_coeff = 0.0001;

	GLDirectLightSource  light_source;
    light_source._lightPos = glm::vec4(0.0f,-8.514f,2.501f, 0.0);
	//initLightPos = light_source._lightPos;
    light_source._ambient_intensity = 0.2;
    light_source._specular_intensity = 2.5;
    light_source._diffuse_intensity = 1.0;
    light_source._attenuation_coeff = 0.0;
    
	GLDirectLightSource  light_source2;
	light_source2._lightPos = glm::vec4(0.0f, 2.514f, 2.501f, 0.0);
	initLightPos = light_source._lightPos;
	light_source2._ambient_intensity = 0.2;
	light_source2._specular_intensity = 3.5;
	light_source2._diffuse_intensity = 2.0;
	light_source2._attenuation_coeff = 0.0;

	//add light

	appearance_0->addLightSource(light_source2);
	appearance_0->addLightSource(point_1);
	appearance_2->addLightSource(light_source2);
	appearance_2->addLightSource(point_1);
	appearance_3->addLightSource(light_source);

    // Create a material object
    GLMaterial material_0, material_1, material_2, material_3;
    material_0._diffuse_material = glm::vec3(0.0, 0.0, 0.0);
    material_0._ambient_material = glm::vec3(0.0, 0.0, 0.0);
    material_0._specular_material = glm::vec3(1.0, 1.0, 1.0);
    material_0._shininess = 12.0;
    material_0._transparency = 1.0;

	material_1._diffuse_material = glm::vec3(0.0, 0.0, 0.0);
	material_1._ambient_material = glm::vec3(0.0, 0.0, 0.0);
	material_1._specular_material = glm::vec3(1.0, 1.0, 1.0);
	material_1._shininess = 40.0;
	material_1._transparency = 1.0;

	material_2._diffuse_material = glm::vec3(0.0, 0.0, 0.0);
	material_2._ambient_material = glm::vec3(0.0, 0.0, 0.0);
	material_2._specular_material = glm::vec3(1.0, 1.0, 1.0);
	material_2._shininess = 20.0;
	material_2._transparency = 1.0;

	material_3._diffuse_material = glm::vec3(0.1, 0.1, 0.1);
	material_3._ambient_material = glm::vec3(0.1, 0.1, 0.1);
	material_3._specular_material = glm::vec3(1.0, 1.0, 1.0);
	material_3._shininess = 50.0;
	material_3._transparency = 1.0f;
    
    // Add the material to the appearance object
    appearance_0->setMaterial(material_0);
    appearance_0->finalize();


	appearance_1->setMaterial(material_1);
	appearance_1->finalize();

	appearance_2->setMaterial(material_2);
	appearance_2->finalize();

	appearance_3->setMaterial(material_3);
	appearance_3->finalize();

    // update lights
    appearance_0->updateLightSources();
	appearance_1->updateLightSources();
	appearance_2->updateLightSources();
	appearance_3->updateLightSources();
    
    GLObjectObj* loadedModel1 = new GLObjectObj("car.obj");
    loadedModel1->setApperance(*appearance_0);
    loadedModel1->init();
    
	GLObjectObj* loadedWheel = new GLObjectObj("wheel.obj");
	loadedWheel->setApperance(*appearance_1);
	loadedWheel->init();

	GLObjectObj* loadedCone = new GLObjectObj("cone.obj");
	loadedCone->setApperance(*appearance_2);
	loadedCone->init();
	
	GLObjectObj* street = new GLObjectObj("street.obj");
	street->setApperance(*appearance_3);
	street->init();

    
    loadedModel1->setMatrix(tranform);

	
	loadedCone->setMatrix(glm::scale(glm::vec3(4.0, 4.0f, 4.0f)));

	program = appearance_0->getProgram();
	program2 = appearance_1->getProgram();
	program3 = appearance_2->getProgram();
	program4 = appearance_3->getProgram();
	//load textures

	glUseProgram(program);

	std::vector<std::string> textureNames;
	textureNames.push_back("../textures/car2_texture.bmp");
	textureNames.push_back("../textures/tire_texture.bmp");
	textureNames.push_back("../textures/cone.bmp");
	textureNames.push_back("../textures/street.bmp");
	textureNames.push_back("../textures/street_normal.bmp");
	

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	int texPosition = glGetUniformLocation(program, "texture1");
	GLuint textures[5];
	
	//generate texture names and place into array
	glGenTextures(5, textures);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0]);

	//set texture parameters
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	unsigned char * img = ImgLoader::Load(textureNames.at(0));
	//load texture to gpu
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 1024, 0, GL_BGR, GL_UNSIGNED_BYTE, img);

	glUniform1i(texPosition, 0);



	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	glUseProgram(program2);
	

	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	int texPosition2 = glGetUniformLocation(program2, "texture1");
	//GLuint textures[1];

	//generate texture names and place into array
	//glGenTextures(1, textures);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textures[1]);

	//set texture parameters
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	unsigned char *img2 = ImgLoader::Load(textureNames.at(1));
	//load texture to gpu
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256, 256, 0, GL_BGR, GL_UNSIGNED_BYTE, img2);

	glUniform1i(texPosition2, 1);

	///////////////////////////////////////////////////////////////////////
	glUseProgram(program3);


	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	int texPosition3 = glGetUniformLocation(program3, "texture1");
	//GLuint textures[1];

	//generate texture names and place into array
	//glGenTextures(1, textures);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, textures[2]);

	//set texture parameters
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	unsigned char *img3 = ImgLoader::Load(textureNames.at(2));
	//load texture to gpu
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 1024, 0, GL_BGR, GL_UNSIGNED_BYTE, img3);

	glUniform1i(texPosition3, 2);


	///////////////////////////////////////////////////////////////////////
	glUseProgram(program4);


	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	int texPosition4 = glGetUniformLocation(program4, "texture_background");
	int texPosition5 = glGetUniformLocation(program4, "texture_foreground");
	//GLuint textures[1];

	//generate texture names and place into array
	//glGenTextures(1, textures);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, textures[3]);

	//set texture parameters
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	unsigned char *img4 = ImgLoader::Load(textureNames.at(3));
	
	//load texture to gpu
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 512, 512, 0, GL_BGR, GL_UNSIGNED_BYTE, img4);

	glUniform1i(texPosition4, 4);
	
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, textures[4]);

	//set texture parameters
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	unsigned char *img5 = ImgLoader::Load(textureNames.at(4));
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 512, 512, 0, GL_BGR, GL_UNSIGNED_BYTE, img5);

	glUniform1i(texPosition5, 3);
	int deltaPos = glGetUniformLocation(program4, "texture_delta");
	glUniform1f(deltaPos, 1.f);

	int bumpPos = glGetUniformLocation(program4, "bump_mode");
	glUniform1i(bumpPos, 0);

	glUseProgram(0);
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Main render loop
    
    // Set up our green background color
    static const GLfloat clear_color[] = { 0.6f, 0.7f, 1.0f, 1.0f };
	//static const GLfloat clear_color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	static const GLfloat clear_depth[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    
    // This sets the camera to a new location
    // the first parameter is the eye position, the second the center location, and the third the up vector. 
    SetViewAsLookAt(glm::vec3(12.0f, 12.0f, 65.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    
    
    
	cv = stationary;

	//////////////////////////////////////
	
    
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Blending
    
    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0f);
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Main render loop
    
    // This is our render loop. As long as our window remains open (ESC is not pressed), we'll continue to render things.
    while(!glfwWindowShouldClose(window))
    {
		

		// Enable depth test
		// ignore this line, it allows us to keep the distance value after we proejct each object to a 2d canvas.
		glEnable(GL_DEPTH_TEST);


		float curTime = glfwGetTime();

		float timeStep = curTime - prevTime;
		prevTime = curTime;

        // clear buffer
        glClearBufferfv(GL_COLOR , 0, clear_color);
        glClearBufferfv(GL_DEPTH , 0, clear_depth);
        
    
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //// This renders the objects
        

		if (throttle) {
			if (rev) {

				blueCar.currentState = Car::State::reverseThrottle;
			}
			else {
				blueCar.currentState = Car::State::forwardThrottle;
			}
			
		}
		else {
			blueCar.currentState = Car::State::idle;
		}

		blueCar.isTurning = isTurning;

		blueCar.isLeft = isLeft;
		blueCar.update(timeStep * 0.8f);

        // Set the trackball locatiom
		if (reset) {
			reset = false;
			trackball.resetTrackball();
			SetTrackballLocation(trackball.getRotationMatrix());
		}
        SetTrackballLocation(GetCurrentCameraMatrix(), GetCurrentCameraTranslation());

		//Get transformation for car based on velocities, rotation, and position
		glm::mat4 rotationMatrix = glm::rotate((pi2 * curTime / 4.f) + (pi2 * 0.25f), glm::vec3(0.0f, 1.0f, 0.0f));
		float curHeight = sin(curTime * 30) / 4;

		glm::vec3 posTrans = glm::vec3(50.f * sin((curTime / 4) * 2 * 3.1415926535f), curHeight, 50.f * cos((curTime / 4) * 2 * 3.1415926535f));
		glm::mat4 curTransform = glm::translate(posTrans);
		glm::mat4 result = blueCar.getTransform() *  glm::translate(glm::vec3(0, curHeight, 0)) * tranform ;


		// Set Camera view
		if (cv == stationary) {
			float zpos = blueCar.getPos().z;
			SetViewAsLookAt(glm::vec3(0.f,50.f,zpos - 200.f), glm::vec3(0.f, 50.f, zpos - 100.f), glm::vec3(0,1,0));
		}
		else {
			SetViewAsLookAt(blueCar.getPos() - (blueCar.getForward() * 100.f) + (blueCar.getNorm() * 34.f), blueCar.getPos() + (blueCar.getForward() * 20.f), blueCar.getNorm());
		}
		
		//SetViewAsLookAt(glm::vec3(12.0f, 40.0f, 85.5f), glm::vec3(result[3].x, 0, result[3].z), glm::vec3(0.0f, 1.0f, 0.0f));

		//Update position of headlights
		glm::vec3 middleCar = blueCar.getPos();
		middleCar += blueCar.getForward() * 20.0f;
		middleCar.y = 2.0f;
		point_1._lightPos = glm::vec4(middleCar, 1.0);
		//light_source2._lightPos = glm::vec4(blueCar.getForward().x, blueCar.getForward().y, blueCar.getForward().z, 0.0f);

		
		if (headlights) {
			point_1.setSpecular(5.5f);
		}
		else {
			point_1.setSpecular(0.0f);

		}
		

		//spot_1.setAmbient(0.0f);
		//spot_1.setSpecular(0.0f);
		appearance_0->updateLightSources();
		appearance_1->updateLightSources();
		appearance_2->updateLightSources();
		appearance_3->updateLightSources();

		float wheelYTrans = 0.36f - curHeight*0.1f;
		glm::mat4 wheelTurn = glm::rotate(blueCar.getTurn(), blueCar.getNorm());
		glm::mat4 wheelRotation = glm::rotate(blueCar.getWheelRot(), glm::vec3(1,0,0));

		// Draw 4 wheels
		loadedWheel->setMatrix(result * glm::translate(glm::vec3(1.0f, wheelYTrans, 1.50f)) * wheelTurn * wheelRotation);
		loadedWheel->draw();
		loadedWheel->setMatrix(result * glm::translate(glm::vec3(-1.0f, wheelYTrans, 1.50f)) * wheelTurn * wheelRotation);
		loadedWheel->draw();
		loadedWheel->setMatrix(result * glm::translate(glm::vec3(1.0f, wheelYTrans, -1.20f)) * wheelRotation);
		loadedWheel->draw();
		loadedWheel->setMatrix(result * glm::translate(glm::vec3(-1.0f, wheelYTrans, -1.20f)) * wheelRotation);
		loadedWheel->draw();
		
		//cs->draw();

		//Draw car body
		loadedModel1->setMatrix(result);
		loadedModel1->draw();


		//Draw road cones
		for (it = conePositions.begin(); it < conePositions.end(); it++) {
			loadedCone->setMatrix(glm::translate(glm::vec3((*it).x, 0.f, (*it).y) * 10.f) * glm::scale(glm::vec3(4.f, 4.f, 4.f)));
			loadedCone->draw();
		
		}

		//Draw street
		for (it2 = streetPositions.begin(); it2 < streetPositions.end(); it2++) {
			street->setMatrix(glm::translate(glm::vec3((*it2).x, 0.f, (*it2).z * 2.f) * 10.f) * glm::scale(glm::vec3(50.f, 4.f, 25.f)));
			street->draw();

		}

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
        
        // Swap the buffers so that what we drew will appear on the screen.
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }
    
    
    delete cs;


}

