#include "Ball.h"
#include "Paddle.h"
#include <glm\glm.hpp>
#include <iostream>
#include <math.h>
#include <ctime>
#include "Window.h"
#include <random>

#define PI 3.14159265

Ball::Ball(Window* window, Paddle* paddle1, Paddle* paddle2) {
	this->pos.x = 0.0;
	this->pos.y = 0.0;
	this->paddle1 = paddle1;
	this->paddle2 = paddle2;
	this->window = window;

	this->updateBuffer();
}

void Ball::updateBuffer() {
	this->bufferData[0] = this->pos.x + 0.0;
	this->bufferData[1] = this->pos.y + 0.0;
	this->bufferData[2] = this->pos.x + 0.0;
	this->bufferData[3] = this->pos.y + this->radius;
	this->bufferData[4] = this->pos.x + this->radius;
	this->bufferData[5] = this->pos.y + this->radius;
	this->bufferData[6] = this->pos.x + this->radius;
	this->bufferData[7] = this->pos.y + 0.0;
}

void Ball::move() {
	float dx = speed * cos(direction * PI / 180);
	float dy = speed * sin(direction * PI / 180);
	this->pos.x += dx;
	this->pos.y += dy;

	printf("%f, %f\n", this->pos.x, this->pos.y);

	if (this->pos.x > 1 - this->radius) {
		this->pos.x = 1 - this->radius;
	}
	else if (this->pos.x < -1) {
		this->pos.x = -1;
	}

	if (this->pos.y > 1 - this->radius) {
		this->pos.y = 1 - this->radius;
	}
	else if (this->pos.y < -1) {
		this->pos.y = -1;
	}

	bool top_wall = false;
	bool bottom_wall = false;
	bool left_wall = false;
	bool right_wall = false;

	if (this->pos.x + this->radius == 1) {
		right_wall = true;
	}
	else if (this->pos.x == -1) {
		left_wall = true;
	}

	if (this->pos.y + this->radius == 1) {
		top_wall = true;
	}
	else if (this->pos.y == -1) {
		bottom_wall = true;
	}

	if (top_wall || bottom_wall) {
		direction = 360 - direction;
	}

	if (left_wall || right_wall) {
		this->reset();
	}

	if (left_wall) {
		this->window->addRight();
	}
	else if (right_wall) {
		this->window->addLeft();
	}

	// Right paddle
	bool paddle1_top = false;
	bool paddle1_bottom = false;
	bool paddle1_left = false;
	float paddle1_xmin = this->paddle1->getBufferData()[0];
	float paddle1_xmax = this->paddle1->getBufferData()[2];
	float paddle1_ymin = this->paddle1->getBufferData()[1];
	float paddle1_ymax = this->paddle1->getBufferData()[5];

	if (
		this->pos.x + this->radius >= paddle1_xmin &&
		this->pos.x + this->radius < paddle1_xmin + this->speed &&
		this->pos.y < paddle1_ymax &&
		this->pos.y + this->radius > paddle1_ymin
	) {
		paddle1_left = true;
		this->speed *= 1.15;
		this->paddle1->setSpeed(this->paddle1->getSpeed() * 1.15);
		this->paddle2->setSpeed(this->paddle2->getSpeed() * 1.15);
	}
	
	if (
		this->pos.y <= paddle1_ymax &&
		this->pos.y > paddle1_ymax - this->speed &&
		this->pos.x + this->radius > paddle1_xmin &&
		this->pos.x < paddle1_xmax &&
		this->isTravellingDown()
	) {
		paddle1_top = true;
	}
	else if (
		this->pos.y + this->radius >= paddle1_ymin &&
		this->pos.y + this->radius < paddle1_ymin + this->speed &&
		this->pos.x + this->radius > paddle1_xmin &&
		this->pos.x < paddle1_xmax &&
		!this->isTravellingDown()
	) {
		paddle1_bottom = true;
	}

	if (paddle1_top || paddle1_bottom) {
		this->direction = 360 - this->direction;
	}

	if (paddle1_left) {
		this->direction = 180 - this->direction;
		this->randBallColor();
	}

	// Left paddle
	bool paddle2_top = false;
	bool paddle2_bottom = false;
	bool paddle2_right = false;
	float paddle2_xmin = this->paddle2->getBufferData()[2];
	float paddle2_xmax = this->paddle2->getBufferData()[0];
	float paddle2_ymin = this->paddle2->getBufferData()[1];
	float paddle2_ymax = this->paddle2->getBufferData()[5];

	if (
		this->pos.x <= paddle2_xmax &&
		this->pos.x > paddle2_xmax - this->speed &&
		this->pos.y < paddle2_ymax &&
		this->pos.y + this->radius > paddle2_ymin
		) {
		paddle2_right = true;
		this->speed *= 1.15;
		this->paddle1->setSpeed(this->paddle1->getSpeed() * 1.15);
		this->paddle2->setSpeed(this->paddle2->getSpeed() * 1.15);
	}

	if (
		this->pos.y <= paddle2_ymax &&
		this->pos.y > paddle2_ymax - this->speed &&
		this->pos.x + this->radius > paddle2_xmin &&
		this->pos.x < paddle2_xmax &&
		this->isTravellingDown()
		) {
		paddle2_top = true;
	}
	else if (
		this->pos.y + this->radius >= paddle2_ymin &&
		this->pos.y + this->radius < paddle2_ymin + this->speed &&
		this->pos.x + this->radius > paddle2_xmin &&
		this->pos.x < paddle2_xmax &&
		!this->isTravellingDown()
		) {
		paddle2_bottom = true;
	}

	if (paddle2_top || paddle2_bottom) {
		this->direction = 360 - this->direction;
	}

	if (paddle2_right) {
		this->direction = 180 - this->direction;
		this->randBallColor();
	}

	this->updateBuffer();
}

GLfloat* Ball::getBufferData() {
	return bufferData;
}

void Ball::reset() {
	this->pos.x = 0;
	this->pos.y = 0;
	this->direction = 60;
	this->speed = this->defaultSpeed;
	this->paddle1->reset();
	this->paddle2->reset();
}

bool Ball::isTravellingDown() {
	while (this->direction < 0) {
		this->direction += 360;
	}

	return this->direction - 180 > 0;
}

float Ball::getR() {
	return r;
}

float Ball::getG() {
	return g;
}

float Ball::getB() {
	return b;
}

void Ball::randBallColor() {
	srand(time(NULL));
	float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	this->r = r;

	float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	this->g = g;

	float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	this->b = b;
}