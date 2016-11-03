//
//  main.cpp
//  Texture template for Homework 4
//
//  Created by Rafael Radkowski on 5/28/15.
//  Copyright (c) 2015 -. All rights reserved.
//



/*---------------------------------------------------------------------------------
Note,

- The code gives you some hints what to do. I added some comments
- The shader code is stored in two files. HW4_shader.vs/.fs
  You must edit this shader programs in order to get the shader code to work with textures. 

In addition:
- The class (file) ShaderFileUtils provides functions to load shader code from a file. 
- The class (file) ImgLoader provides a function to load a bitmap .bmp image from a file. 

-----------------------------------------------------------------------------------*/

// stl include
#include <iostream>
#include <string>

// GLEW include
#include <GL/glew.h>

// GLM include files
#define GLM_FORCE_INLINE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


// glfw includes
#include <GLFW/glfw3.h>


// include local files
#include "controls.h"
#include "HCI557Common.h"
#include "CoordSystem.h"
#include "ShaderFileUtils.h"
#include "ImgLoader.h"

//#include "CImg.h"

// this line tells the compiler to use the namespace std.
// Each object, command without a namespace is assumed to be part of std. 
using namespace std;


/// Camera control matrices
glm::mat4 projectionMatrix; // Store the projection matrix
glm::mat4 viewMatrix; // Store the view matrix
glm::mat4 modelMatrix; // Store the model matrix




// The handle to the window object
GLFWwindow*         window;


// Define some of the global variables we're using for this sample
GLuint program;




// USE THESE vertex array objects to define your objects
unsigned int vaoID[2];

unsigned int vboID[4];

//array of three texture names
GLuint textures[3];
GLuint texture1, texture2, texture3;

//unsigned char * img_data[3]; //textures
std::vector<unsigned char *> img_data;
unsigned char *img_data1;


/**
This function checks your shader code
@param shader - the id of the shader
@param shader_type - type = GL_FRAGMENT_SHADER or GL_VERTEX_SHADER
*/
bool CheckShaderLocal(GLuint shader, GLenum shader_type)
{
	GLint compiled = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

	if (!compiled)
	{
		GLint info_len = 0;

		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_len);

		if (info_len)
		{
			char* buf = (char*)malloc(info_len);
			if (buf) {
				glGetShaderInfoLog(shader, info_len, NULL, buf);
				cout << "Could not compile shader shader_type " << shader_type << ":\n" << buf << endl;
				free(buf);
			}
			glDeleteShader(shader);
			shader = 0;
		}
		return false;
	}
	return true;

}




/*!
This function creates a single square
 */
void createSquare(void)
{
	float* vertices = new float[18];  // Vertices for our square
	float* vertices2 = new float[30]; //vertices for square with UV coordinates
	float *colors = new float[18]; // Colors for our vertices
	

	vertices[0] = -0.5; vertices[1] = -0.5; vertices[2] = 0.5; // Bottom left corner
	vertices2[0] = -0.5; vertices2[1] = -0.5; vertices2[2] = 0.5; vertices2[3] = 0.0; vertices2[4] = 0.0; // Bottom left corner
	colors[0] = 1.0; colors[1] = 1.0; colors[2] = 1.0; // Bottom left corner

	vertices[3] = -0.5; vertices[4] = 0.5; vertices[5] = 0.5; // Top left corner
	vertices2[5] = -0.5; vertices2[6] = 0.5; vertices2[7] = 0.5; vertices2[8] = 0.0; vertices2[9] = 1.0; //top left corner
	colors[3] = 1.0; colors[4] = 0.0; colors[5] = 0.0; // Top left corner

	vertices[6] = 0.5; vertices[7] = 0.5; vertices[8] = 0.5; // Top Right corner
	vertices2[10] = 0.5; vertices2[11] = 0.5; vertices2[12] = 0.5; vertices2[13] = 1.0; vertices2[14] = 1.0; //top right corner
	colors[6] = 0.0; colors[7] = 1.0; colors[8] = 0.0; // Top Right corner

	vertices[9] = 0.5; vertices[10] = -0.5; vertices[11] = 0.5; // Bottom right corner
	vertices2[15] = 0.5; vertices2[16] = -0.5; vertices2[17] = 0.5; vertices2[18] = 1.0; vertices2[19] = 0.0; //bottom right corner
	colors[9] = 0.0; colors[10] = 0.0; colors[11] = 1.0; // Bottom right corner

	vertices[12] = -0.5; vertices[13] = -0.5; vertices[14] = 0.5; // Bottom left corner
	vertices2[20] = -0.5; vertices2[21] = -0.5; vertices2[22] = 0.5; vertices2[23] = 0.0; vertices2[24] = 0.0; // Bottom left corner
	colors[12] = 1.0; colors[13] = 1.0; colors[14] = 1.0; // Bottom left corner

	vertices[15] = 0.5; vertices[16] = 0.5; vertices[17] = 0.5; // Top Right corner
	vertices2[25] = 0.5; vertices2[26] = 0.5; vertices2[27] = 0.5; vertices2[28] = 1.0; vertices2[29] = 1.0; //top right corner
	colors[15] = 0.0; colors[16] = 1.0; colors[17] = 0.0; // Top Right corner


	/*---------------------------------------------------------------------------------
	  TODO:
	  - Add texture coordinates to the vertices above
	  - Assign the texture coordinates to a vertex buffer objects as discussed in class.
	    Note, they ned to be inside the same vbo in which the vertices are defined. 
	-----------------------------------------------------------------------------------*/



	glGenVertexArrays(2, &vaoID[0]); // Create our Vertex Array Object
	glBindVertexArray(vaoID[0]); // Bind our Vertex Array Object so we can use it


	glGenBuffers(2, vboID); // Generate our Vertex Buffer Object

							// vertices
	glBindBuffer(GL_ARRAY_BUFFER, vboID[0]); // Bind our Vertex Buffer Object
	//glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(GLfloat), vertices, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW
	glBufferData(GL_ARRAY_BUFFER, 30 * sizeof(GLfloat), vertices2, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW


	//glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
	
	int locPos = glGetAttribLocation(program, "in_Position");
	glVertexAttribPointer((GLuint)locPos, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), NULL); // Set up our vertex attributes pointer
	glEnableVertexAttribArray(locPos); // Disable our Vertex Array Object

					//texture coordinates
	int texCoord = glGetAttribLocation(program, "in_TexCoord");
	glVertexAttribPointer((GLuint)texCoord, 2, GL_FLOAT, GL_TRUE, 5 * sizeof(GLfloat), (const GLvoid*) (3*sizeof(GLfloat))); // Set up our vertex attributes pointer
	glEnableVertexAttribArray(texCoord); // Enable the second vertex attribute array
								  //Color

	glBindBuffer(GL_ARRAY_BUFFER, vboID[1]); // Bind our second Vertex Buffer Object
	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(GLfloat), colors, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW

	int locColor = glGetAttribLocation(program, "in_Color");
	glVertexAttribPointer((GLuint)locColor, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
	glEnableVertexAttribArray(locColor); // Enable the second vertex attribute array

	
	
	
	glBindVertexArray(0); // Disable our Vertex Buffer Object


	delete[] vertices; // Delete our vertices from memory


	/*---------------------------------------------------------------------------------
	TODO:
	- Load the texture
	- Create a texture object
	- Set the correct texture unit active
	- Set all texture parameters as requried
	- Load the texture to your graphics card
	- Create a texture uniform variable
	-----------------------------------------------------------------------------------*/
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);


	//int texPos = glGetUniformLocation(program, "tex");
	int texPositions[3];
	texPositions[0] = glGetUniformLocation(program, "texture1");
	texPositions[1] = glGetUniformLocation(program, "texture2");
	texPositions[2] = glGetUniformLocation(program, "texture3");
	
	
	std::vector<std::string> textureNames;
	
	textureNames.push_back("../textures/comet_texture.bmp");
	textureNames.push_back("../textures/hedgehog_texture.bmp");
	textureNames.push_back("../textures/gradient_texture.bmp");
	//textureNames.push_back("bitmap1.bmp");
	//textureNames.push_back("bitmap2.bmp");
	//textureNames.push_back("bitmap3.bmp");
	
	//generate texture names and place into array
	glGenTextures(3, textures);
	
	//load textures
	for (int i = 0; i < 3; i++) {
		if (texPositions[i] != -1) {
			//bind texture
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, textures[i]);
	
			//set texture parameters
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			unsigned char * img = ImgLoader::Load(textureNames.at(i));
			//load texture to gpu
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 512, 512, 0, GL_BGR, GL_UNSIGNED_BYTE, img);
			
			glUniform1i(texPositions[i], i);

			
		}
		//textureNames.pop_back();
	}
	//glBindTexture(GL_TEXTURE_2D, 0);

	
	
}







/*!
 This code renders the plane
 */
void renderSquare(void)
{

	glUseProgram(program);
	//glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBindVertexArray(vaoID[0]); // Bind our Vertex Array Object

	glDrawArrays(GL_TRIANGLES, 0, 6); // Draw our square

	glBindVertexArray(0); // Unbind our Vertex Array Object

	glUseProgram(0);

}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*!
 This function creates the two models
 */
void setupScene(void) {
    
	createSquare();
    
}




int main(int argc, const char * argv[])
{
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Init glfw, create a window, and init glew
    
    // Init the GLFW Window
    window = initWindow();
    
    
    // Init the glew api
    initGlew();
    
	// Prepares some defaults
	CoordSystemRenderer* coordinate_system_renderer = new CoordSystemRenderer(10.0);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// The Shader Program starts here
    
    // Vertex shader source code. This draws the vertices in our window. We have 3 vertices since we're drawing an triangle.
    // Each vertex is represented by a vector of size 4 (x, y, z, w) coordinates.
	static const string vertex_code = ShaderFileUtils::LoadFromFile("hw4_shader.vs");
    static const char * vs_source = vertex_code.c_str();
    
    // Fragment shader source code. This determines the colors in the fragment generated in the shader pipeline. In this case, it colors the inside of our triangle specified by our vertex shader.
    static const string fragment_code = ShaderFileUtils::LoadFromFile("hw4_shader.fs");
    static const char * fs_source = fragment_code.c_str();


	// Fragment shader for part 2
	//static const string fragment_code = ShaderFileUtils::LoadFromFile("hw4_shader_part2.fs");
	//static const char * fs_source = fragment_code.c_str();
    
    // This next section we'll generate the OpenGL program and attach the shaders to it so that we can render our triangle.
    program = glCreateProgram();
    
    // We create a shader with our fragment shader source code and compile it.
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fs_source, NULL);
    glCompileShader(fs);
	bool ret = CheckShaderLocal(fs, GL_VERTEX_SHADER);
	if (!ret) { cout << "Problems compiling GL_VERTEX_SHADER" << endl; }
    
    // We create a shader with our vertex shader source code and compile it.
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vs_source, NULL);
    glCompileShader(vs);
	ret = CheckShaderLocal(vs, GL_VERTEX_SHADER);
	if (!ret) { cout << "Problems compiling GL_VERTEX_SHADER" << endl; }
    
    // We'll attach our two compiled shaders to the OpenGL program.
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    
    glLinkProgram(program);
    
    // We'll specify that we want to use this program that we've attached the shaders to.
    glUseProgram(program);
    
    //// The Shader Program ends here
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    

    
    // Set up our green background color
    static const GLfloat clear_color[] = { 0.6f, 0.7f, 1.0f, 1.0f };
    static const GLfloat clear_depth[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    
    projectionMatrix = glm::perspective(1.1f, (float)800 / (float)600, 0.1f, 100.f);
    modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)); // Create our model matrix which will halve the size of our model
    viewMatrix = glm::lookAt(glm::vec3(1.0f, 0.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    
    int projectionMatrixLocation = glGetUniformLocation(program, "projectionMatrix"); // Get the location of our projection matrix in the shader
    int viewMatrixLocation = glGetUniformLocation(program, "viewMatrix"); // Get the location of our view matrix in the shader
    int modelMatrixLocation = glGetUniformLocation(program, "modelMatrix"); // Get the location of our model matrix in the shader
    
    
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]); // Send our projection matrix to the shader
    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]); // Send our view matrix to the shader
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]); // Send our model matrix to the shader
    



   // glBindAttribLocation(program, 0, "in_Position");
    //glBindAttribLocation(program, 1, "in_Color");
	//glBindAttribLocation(program, 2, "in_TexCoord");

    
    // this creates the scene
    setupScene();
    
    int i=0;

    // Enable depth test
    // ignore this line, it allows us to keep the distance value after we proejct each object to a 2d canvas.
    glEnable(GL_DEPTH_TEST);
    
    // This is our render loop. As long as our window remains open (ESC is not pressed), we'll continue to render things.
    while(!glfwWindowShouldClose(window))
    {
        
        // Clear the entire buffer with our green color (sets the background to be green).
        glClearBufferfv(GL_COLOR , 0, clear_color);
        glClearBufferfv(GL_DEPTH , 0, clear_depth);
        
        // this draws the coordinate system
		coordinate_system_renderer->draw();
        
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //// This generate the object
        // Enable the shader program
        glUseProgram(program);
        
        // this changes the camera location
        glm::mat4 rotated_view = viewMatrix * GetRotationMatrix();
        glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &rotated_view[0][0]); // send the view matrix to our shader
        

        // This moves the model 
        modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]); // Send our model matrix to the shader
        
       
		// This line renders your Ppolygon model
		renderSquare();
        
        
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        
        
        // Swap the buffers so that what we drew will appear on the screen.
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }
    
	// delete the coordinate system object
	delete coordinate_system_renderer;

    // Program clean up when the window gets closed.
    glDeleteVertexArrays(2, vaoID);
    glDeleteProgram(program);
}

