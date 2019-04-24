#pragma once
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>

class Ball {

private:
	GLfloat bufferData[8];
	glm::vec2 pos;
	float radius = 0.04;
	float direction = 60;
	float speed = 0.0004;
	void updateBuffer();

public:
	Ball();
	void move();
	GLfloat* getBufferData();
	void reset();
};