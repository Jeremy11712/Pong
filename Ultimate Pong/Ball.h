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
	float defaultSpeed = 0.001;
	float speed = defaultSpeed;
	float r = 0.5;
	float g = 1.0;
	float b = 1.0;
	Paddle* paddle1;
	Paddle* paddle2;
	bool isTravellingDown();
	void updateBuffer();

public:
	Ball(Paddle* paddle1, Paddle* paddle2);
	void move();
	GLfloat* getBufferData();
	void reset();
	void randBallColor();
	float getR();
	float getG();
	float getB();
};