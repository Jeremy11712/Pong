#include "Ball.h";
#include <glm\glm.hpp>
#include <iostream>
#include <math.h>

#define PI 3.14159265

Ball::Ball() {
	this->pos.x = 0.0;
	this->pos.y = 0.0;

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

	bool top = false;
	bool bottom = false;
	bool left = false;
	bool right = false;

	if (this->pos.x + this->radius == 1) {
		right = true;
	}
	else if (this->pos.x == -1) {
		left = true;
	}

	if (this->pos.y + this->radius == 1) {
		top = true;
	}
	else if (this->pos.y == -1) {
		bottom = true;
	}

	if (top || bottom) {
		direction = 360 - direction;
	}

	if (left || right) {
		direction = 180 - direction;
	}

	printf("x %f y %f\n", this->pos.x, this->pos.y);

	this->updateBuffer();
}

GLfloat* Ball::getBufferData() {
	return bufferData;
}

void Ball::reset() {
	this->pos.x = 0;
	this->pos.y = 0;
}