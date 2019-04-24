#pragma once
#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>
#include <string>
#include <time.h>
using namespace std;

class Window {
private:
	time_t start;
	int seconds;
	int minutes;
	string runtime;
	int scoreLeft;
	int scoreRight;
public:
	Window();
	void startTime();
	string getTime();
	void addLeft();
	void addRight();
	string getLeft();
	string getRight();
};