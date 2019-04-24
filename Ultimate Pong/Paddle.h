#pragma once
#include <GL/glew.h>
#include <GL/glut.h>

class Paddle {

private:
	GLfloat bufferData[8];
	GLfloat bl[2], br[2], tl[2], tr[2];
	bool arrowKeys;
	float defaultSpeed = 0.0007;
	float speed = defaultSpeed;

public:
	Paddle();
	Paddle(bool isArrowKeys);
	void moveUp();
	void moveDown();
	bool isUsingArrowKeys();
	GLfloat* getBufferData();
	float getSpeed();
	void setSpeed(float speed);
	void reset();

};