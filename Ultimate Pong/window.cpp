#include "Window.h"

Window::Window() {
	//defines dashed line cord.
	seconds = 0;
	minutes = 0;
	runtime = "";
	scoreLeft = 0;
	scoreRight = 0;
}
void Window::addLeft() {
	scoreLeft++;
}

void Window::addRight() {
	scoreRight++;
}

string Window::getLeft() {
	return to_string(scoreLeft);
}

string Window::getRight() {
	return to_string(scoreRight);
}

void Window::startTime() {
	start = time(0);
}

string Window::getTime() {
	string temp1 = "";
	string temp2 = "";

	seconds = difftime(time(0), start);
	//cout << secondsSinceStart << endl;
	if (seconds % 60 == 0 && seconds != 0) {
		start = time(0);
		minutes++;
	}

	temp1 = to_string(minutes);
	//cout << temp1.length() <<"-> "<<temp1<< endl;
	if (temp1.length() == 1) {
		temp2 = temp1;
		temp1 = "0" + temp2;
	}
	runtime = temp1;

	temp1 = to_string(seconds);
	if (temp1.length() == 1) {
		temp2 = temp1;
		temp1 = "0" + temp2;
	}
	runtime += " : " + temp1;
	//cout << runtime << endl;
	return runtime;
}
