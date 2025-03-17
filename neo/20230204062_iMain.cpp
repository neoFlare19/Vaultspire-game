#include "iGraphics.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

int logoX = SCREEN_WIDTH / 14;
int logoY = SCREEN_HEIGHT / 10;
int logoWidth = 50;
int logoHeight = 50;

int dx = 0;
int dy = 0;

int backgroundImage; // Variable to store the loaded image ID

void drawLogo() {
	iSetColor(0, 0, 0); // Outer black rectangle
	iFilledRectangle(logoX, logoY, logoWidth, logoHeight);
	iSetColor(255, 255, 255); // Inner white rectangle
	iFilledRectangle(logoX + 15, logoY + 15, logoWidth - 30, logoHeight - 30);
}

void iDraw() {
	iClear();

	// Draw the background image
	iShowImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, backgroundImage);

	// Draw the centered rectangle
	iSetColor(0, 0, 0); // Black border for the rectangle
	iRectangle(SCREEN_WIDTH / 2 - 25, SCREEN_HEIGHT / 2 - 25, 50, 50);

	// Draw the moving logo
	drawLogo();
}

void moveLogo() {
	// Update logo position
	logoX += dx;
	logoY += dy;

	// Stop logo in the center of the screen
	if (abs(logoX - (SCREEN_WIDTH / 2 - logoWidth / 2)) < 5 &&
		abs(logoY - (SCREEN_HEIGHT / 2 - logoHeight / 2)) < 5) {
		logoX = SCREEN_WIDTH / 2 - logoWidth / 2;
		logoY = SCREEN_HEIGHT / 2 - logoHeight / 2;
		dx = 0;
		dy = 0;
	}

	// Bounce off the edges
	if (logoX + logoWidth > SCREEN_WIDTH || logoX < 0) {
		dx *= -1;
	}
	if (logoY + logoHeight > SCREEN_HEIGHT || logoY < 0) {
		dy *= -1;
	}
}

void iMouseMove(int mx, int my) {}

void iPassiveMouseMove(int mx, int my) {}

void iMouse(int button, int state, int mx, int my) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {}

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {}
}

void iKeyboard(unsigned char key) {
	if (key == '\r') {}
}

void iSpecialKeyboard(unsigned char key) {
	if (key == GLUT_KEY_RIGHT) {
		dx = 5;
		dy = 0;
	}
	else if (key == GLUT_KEY_LEFT) {
		dx = -5;
		dy = 0;
	}
	else if (key == GLUT_KEY_UP) {
		dx = 0;
		dy = 5;
	}
	else if (key == GLUT_KEY_DOWN) {
		dx = 0;
		dy = -5;
	}
}

int main() {
	iInitialize(SCREEN_WIDTH, SCREEN_HEIGHT, "Neo's logo bouncer");

	// Load the background image
	backgroundImage = iLoadImage("door.jpg");

	iSetTimer(5, moveLogo);
	iStart();
	return 0;
}
