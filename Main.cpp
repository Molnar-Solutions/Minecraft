#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Texture.h"

#define GLCheck(x) GLClearAllErrors(); x; GLCheckErrorStatus(#x, __LINE__);

/* Error handling routines */
static void GLClearAllErrors() {
	while (glGetError() != GL_NO_ERROR) {

	}
}

static bool GLCheckErrorStatus(const char* function, int line) {
	while (GLenum error = glGetError()) {
		std::cout << "OpenGL Error: " << error
			<< "\tLine: " << line
			<< "\tFunction: " << function << std::endl;
		return true;
	}

	return false;
}

const unsigned int width = 800;
const unsigned int height = 800;

// Vertices coordinates
GLfloat vertices[] =
{ //    COORDINATES     /     COLORS			/ TEXTURE /
	-0.5f, -0.5f, 0.0f,     1.0f, 0.0f,  0.00f,  0.0f, 0.0f,	// 0 - Lower left corner
	-0.5f,  0.5f, 0.0f,     0.0f, 1.0f,  0.00f,  0.0f, 1.0f,	// 1 - Lower right corner
	 0.5f,  0.5f, 0.0f,     1.0f, 0.0f,  1.0f,   1.0f, 1.0f,    // 2 - Upper corner
	 0.5f,  -0.5f, 0.0f,    1.9f, 1.0f,  1.0f,   1.0f, 0.0f,     // 3 - Inner left

	 -0.5f, -0.5f, 0.5f,     1.0f, 0.0f,  0.00f,  0.0f, 0.0f,	// 4 - Lower left corner
	-0.5f,  0.5f,  0.5f,     0.0f, 1.0f,  0.00f,  0.0f, 1.0f,	// 5 - Lower right corner
	 0.5f,  0.5f,  0.5f,     1.0f, 0.0f,  1.0f,   1.0f, 1.0f,   // 6 - Upper corner
	 0.5f,  -0.5f, 0.5f,    1.9f, 1.0f,  1.0f,   1.0f, 0.0f     // 7 - Inner left
};

GLfloat vertices2[] =
{
	// COORDINATES       /   COLORS              / TEXTURE
	// X    Y      Z
	-0.25f, 0.0f,  -0.25f,     1.0f, 0.0f,  0.00f,  0.0f, 0.0f, // 0
	-0.25f,  0.0f, 0.25f,     0.0f, 1.0f,  0.00f,   0.0f, 1.0f, // 1
	0.25f,  0.0f,  0.25f,     1.0f, 0.0f,  1.0f,    1.0f, 1.0f, // 2
	0.25f,  0.0f,  -0.25f,     1.9f, 1.0f,  1.0f,   1.0f, 0.0f,  // 3

	-0.25f, 0.25f,  -0.25f,     1.0f, 0.0f,  0.00f,   0.0f, 1.0f, // 4
	0.25f, 0.25f,  -0.25f,      1.0f, 0.0f,  0.00f,   1.0f, 1.0f, // 5

	-0.25f, 0.25f,  0.25f,      1.0f, 0.0f,  0.00f,   0.0f, 1.0f, // 6
	0.25f, 0.25f,  0.25f,       1.0f, 0.0f,  0.00f,   1.0f, 0.0f, // 7
};

GLuint indices2[] =
{
	// bottom rectangle
	0, 1, 2, 
	0, 2, 3, 

	// back rectangle
	4, 0, 3,
	4, 3, 5,

	// front rectangle
	6, 1, 2,
	6, 2, 7,

	// right side rectangle
	7, 2, 3,
	7, 3, 5,

	// left side rectangle
	6, 1, 0,
	6, 0, 4,

	// top rectangle
	6, 7, 5,
	6, 5, 4,

	// bottom rectangle
	1, 2, 3,
	1, 3, 0
};

// Indices for vertices order
GLuint indices[] =
{
	0, 2, 1, // Upper triangle
	0, 3, 2 //  Lower triangle
};

int main()
{
	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object of 800 by 800 pixels, naming it "YoutubeOpenGL"
	GLFWwindow* window = glfwCreateWindow(800, 800, "Minecraft", NULL, NULL);

	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OpenGL
	gladLoadGL();

	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, 800, 800);



	// Generates Shader object using shaders defualt.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");

	// Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();

	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO1(vertices2, sizeof(vertices2));
	// Generates Element Buffer Object and links it to indices
	EBO EBO1(indices2, sizeof(indices2));

	// Links VBO attributes such as coordinates and colors to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();


	// Gets ID of uniform called "scale"
	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

	// Texture
	std::string texPath = "block.png";

	Texture block(texPath.c_str(), GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
	block.texUnit(shaderProgram, "tex0", 0);

	// Variables that help the rotation of the pyramid
	float rotation = 0.0f;
	double prevTime = glfwGetTime();

	// Enables the depth buffer
	glEnable(GL_DEPTH_TEST);

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Tell OpenGL which Shader Program we want to use
		shaderProgram.Activate();

		// Simple timer
		double crntTime = glfwGetTime();
		if (crntTime - prevTime >= 1 / 60)
		{
			rotation += 0.025f;
			prevTime = crntTime;
		}

		// Initializes matrices so they are not the null matrix
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 proj = glm::mat4(1.0f);

		// Assigns different transformations to each matrix
		model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
		view = glm::translate(view, glm::vec3(0.0f, -0.5f, -2.0f));
		proj = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f);

		// Outputs the matrices into the vertex shader
		GLuint modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		GLuint viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		GLuint projLoc = glGetUniformLocation(shaderProgram.ID, "proj");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

		// Assigns a value to the uniform; NOTE: Must always be done after activating the Shader Program
		glUniform1f(uniID, 0.5f);
		block.Bind();

		// Bind the VAO so OpenGL knows to use it
		VAO1.Bind();

		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, sizeof(indices2) / sizeof(GLuint), GL_UNSIGNED_INT, 0);

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);

		// Take care of all GLFW events
		glfwPollEvents();
	}


	// Delete all the objects we've created
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	block.Delete();
	shaderProgram.Delete();

	// Delete window before ending the program
	glfwDestroyWindow(window);

	// Terminate GLFW before ending the program
	glfwTerminate();

	return 0;
}