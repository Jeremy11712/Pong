#include "Paddle.h"

Paddle::Paddle() {}

Paddle::Paddle(bool isArrowKeys) {
	bl[0] = isArrowKeys ? 0.9 : -0.9;
	bl[1] = isArrowKeys ? -0.2 : -0.2;
	br[0] = isArrowKeys ? 0.95 : -0.95;
	br[1] = isArrowKeys ? -0.2 : -0.2;
	tr[0] = isArrowKeys ? 0.95 : -0.95;
	tr[1] = isArrowKeys ? 0.2 : 0.2;
	tl[0] = isArrowKeys ? 0.9 : -0.9;
	tl[1] = isArrowKeys ? 0.2 : 0.2;
	bufferData[0] = bl[0];
	bufferData[1] = bl[1];
	bufferData[2] = br[0];
	bufferData[3] = br[1];
	bufferData[4] = tr[0];
	bufferData[5] = tr[1];
	bufferData[6] = tl[0];
	bufferData[7] = tl[1];
	arrowKeys = isArrowKeys;
}

void Paddle::moveUp() {
	if (bufferData[5] < 1 /* top y-coord */) {
		bufferData[1] += 0.0004;
		bufferData[3] += 0.0004;
		bufferData[5] += 0.0004;
		bufferData[7] += 0.0004;
	}
}

void Paddle::moveDown() {
	if (bufferData[1] > -1 /* bottom y-coord */) {
		bufferData[1] -= 0.0004;
		bufferData[3] -= 0.0004;
		bufferData[5] -= 0.0004;
		bufferData[7] -= 0.0004;
	}
}

bool Paddle::isUsingArrowKeys() {
	return arrowKeys;
}

GLfloat* Paddle::getBufferData() {
	return bufferData;
}