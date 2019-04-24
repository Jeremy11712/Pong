// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// Include GLEW
#include <GL/glew.h>
#include <GL/glut.h>
// Include GLFW
#include <GLFW/glfw3.h>

#include "Paddle.h"
#include "Ball.h"
#include "shader.hpp"
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <chrono>

GLFWwindow* window;

int main(void)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(800, 800, "Ultimate Pong", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Black background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	GLuint paddle1ID, ballID;
	glGenVertexArrays(1, &paddle1ID);
	glGenVertexArrays(1, &ballID);

	GLuint paddle1Program = LoadShaders("paddlevertex.vs", "paddleframe.fs");
	GLuint ballProgram = LoadShaders("ballvertex.vs", "ballframe.fs");

	// 2) Create buffers
	GLuint paddleBuffer, ballBuffer;
	glGenBuffers(1, &paddleBuffer);
	glGenBuffers(1, &ballBuffer);
	
	Paddle player1 = new Paddle(true);
	Paddle player2 = new Paddle(false);
	Ball ball = Ball();

	/* Debug for cjh
	GLfloat vertices[] = {
		-0.5, 0.5,
		0.5, 0.5,
		0.5, -0.5,
		-0.5, -0.5
	};
	*/
	
	// The program Loop
	do {

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT);
		

		//////////////////////////////////////////////
		// Paddle Data:
		// GLint paddle1PosAttrib = glGetAttribLocation(paddle1Program, "position");
		// GLfloat* paddle1Vertices = player1.getBufferData();

		// // Player 1 Paddle
		// glBindVertexArray(paddle1ID);
		// glBindBuffer(GL_ARRAY_BUFFER, paddleBuffer);
		// // std::cout << sizeof(paddle1Vertices[0]) << std::endl;
		// glBufferData(GL_ARRAY_BUFFER, /* Change this back to sizeof(GLfloat*) */32, paddle1Vertices, GL_STATIC_DRAW);
		// glEnableVertexAttribArray(paddle1PosAttrib);
		// glVertexAttribPointer(paddle1PosAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
		// glUseProgram(paddle1Program);
		// glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		// glDisableVertexAttribArray(paddle1PosAttrib);

		/////////////////////////////////////////////////////////////
		// End Paddle Data

		//////////////////////////////////////////////
		// Ball Data:
		GLint ballPosAttrib = glGetAttribLocation(ballProgram, "position");
		GLfloat* ballVertices = ball.getBufferData();

		// Ball
		glBindVertexArray(ballID);
		glBindBuffer(GL_ARRAY_BUFFER, ballBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 8, ballVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(ballPosAttrib);
		glVertexAttribPointer(ballPosAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glUseProgram(ballProgram);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		glDisableVertexAttribArray(0);

		/////////////////////////////////////////////////////////////
		// End Ball Data


		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

		// Move ball
		ball.move();
		printf("\n");

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	// Cleanup VBO
	glDeleteBuffers(1, &paddleBuffer);
	glDeleteVertexArrays(1, &paddle1ID);
	glDeleteProgram(paddle1Program);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

