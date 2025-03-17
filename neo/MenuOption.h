#include <iostream>


// Menu option coordinates
struct MenuOption {
	int x, y;
	char text[50];
};

MenuOption menu[] = {
	{ 50, 400, "Play" },
	{ 50, 340, "Instructions" },
	{ 50, 280, "About Us" },
	{ 50, 220, "Exit" }
};