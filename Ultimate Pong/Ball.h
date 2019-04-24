#pragma once
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include "Paddle.h"

class Ball {

private:
	GLfloat bufferData[8];
	glm::vec2 pos;
	float radius = 0.04;
	float direction = 60;
	float defaultSpeed = 0.0003;
	float speed = defaultSpeed;
	Paddle* paddle1;
	Paddle* paddle2;
	bool isTravellingDown();
	void updateBuffer();

public:
	Ball(Paddle* paddle1, Paddle* paddle2);
	void move();
	GLfloat* getBufferData();
	void reset();
};