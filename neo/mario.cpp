#include "iGraphics.h"
int i = 0;
bool musicOn = true;
int mario_running = 0;
int mario_jumping = 0;
int mario = 0;
bool jump = false;
bool jumpUp = false;
bool jumpDown = false;
int jumpLimit = 30;
int mario_axix_jump = 0;
void gravity();
void obstacle_movement();
void obstacle_movement_2();
void obstacle_movement_reverse();
void obstacle_movement_reverse_2();
int homepage = 1;
int startpage = 0;
int aboutpage = 0;
int startbuttonclick = 0;
int aboutbuttoncllick = 0;
int easypage = 0;
int mediumpage = 0;
int hardpage = 0;
int p, q;
int playerX = 470, playerY = 0;
int x = 1280, y = 0;
int a = 600, b = 0;
int c = 1280, d = 0;
int heart = 30;

void collision() {
	int marioWidth = 70;
	int marioHeight = 70;
	int pipeWidth = 100;
	int pipeHeight = 100;
	if ((mario_running + marioWidth >= x && mario_running <= x + pipeWidth) &&
		(mario_jumping + marioHeight >= y && mario_jumping <= y + pipeHeight)) {
		heart--;
		printf("%d ", heart);
	}
	if ((mario_running + marioWidth >= x + 400 && mario_running <= x + 400 + pipeWidth) &&
		(mario_jumping + marioHeight >= y && mario_jumping <= y + pipeHeight)) {
		heart--;
		printf("%d ", heart);
	}
	if ((mario_running + marioWidth >= x + 800 && mario_running <= x + 800 + pipeWidth) &&
		(mario_jumping + marioHeight >= y && mario_jumping <= y + pipeHeight)) {
		heart--;
		printf("%d ", heart);
	}

}

void life(){
	if (heart == 0)
	{

		exit(0);

	}
}

void home_page(){

	iShowBMP2(0, 0, "backgroundimage\\photo1.bmp", 0);
}
void start_page()
{
	iShowBMP(0, 0, "backgroundimage\\photo3.bmp");
	iShowBMP2(0, 650, "backbutton\\backbutton1.bmp", 0);

}
void about_page(){
	iShowBMP(0, 0, "backgroundimage\\photo4.bmp");
	iShowBMP2(0, 650, "backbutton\\backbutton1.bmp", 0);
}

void easy_page(){
	iShowBMP(0, 0, "backgroundimage\\photo2.bmp");
	iShowBMP2(x, y, "obstacles\\pipe.bmp", 0);
	iShowBMP2(x + 400, y, "obstacles\\pipe.bmp", 0);
	iShowBMP2(x + 800, y, "obstacles\\pipe.bmp", 0);
	char characterimage[8][30] = { "characterimage\\mario1.bmp", "characterimage\\mario2.bmp", "characterimage\\mario3.bmp", "characterimage\\mario4.bmp", "characterimage\\mario5.bmp", "characterimage\\mario6.bmp", "characterimage\\mario7.bmp", "characterimage\\mario8.bmp" };
	iShowBMP2(mario_running, mario_jumping, characterimage[mario], 0);

}
void medium_page(){
	iShowBMP(0, 0, "backgroundimage\\photo2.bmp");
	iShowBMP2(x, y, "obstacles\\pipe.bmp", 0);
	iShowBMP2(x + 400, y, "obstacles\\pipe.bmp", 0);
	iShowBMP2(x + 800, y, "obstacles\\pipe.bmp", 0);
	iShowBMP2(x + 25, y + 100, "obstacles\\spikes.bmp", 0);
	iShowBMP2(x + 400 + 25, y + 100, "obstacles\\spikes.bmp", 0);
	iShowBMP2(x + 800 + 25, y + 100, "obstacles\\spikes.bmp", 0);
	iShowBMP2(c + 220, d, "obstacles\\shellcreeper.bmp", 0);
	iShowBMP2(c + 450 + 220, d, "obstacles\\shellcreeper.bmp", 0);
	iShowBMP2(c + 850 + 220, d, "obstacles\\shellcreeper.bmp", 0);
	char characterimage[8][30] = { "characterimage\\mario1.bmp", "characterimage\\mario2.bmp", "characterimage\\mario3.bmp", "characterimage\\mario4.bmp", "characterimage\\mario5.bmp", "characterimage\\mario6.bmp", "characterimage\\mario7.bmp", "characterimage\\mario8.bmp" };
	iShowBMP2(mario_running, mario_jumping, characterimage[mario], 0);
}
void hard_page(){
	iShowBMP(0, 0, "backgroundimage\\photo2.bmp");
	iShowBMP2(x, y, "obstacles\\pipe.bmp", 0);
	iShowBMP2(x + 400, y, "obstacles\\pipe.bmp", 0);
	iShowBMP2(x + 800, y, "obstacles\\pipe.bmp", 0);
	iShowBMP2(x + 25, y + 100, "obstacles\\spikes.bmp", 0);
	iShowBMP2(x + 400 + 25, y + 100, "obstacles\\spikes.bmp", 0);
	iShowBMP2(x + 800 + 25, y + 100, "obstacles\\spikes.bmp", 0);

	iShowBMP2(c, d, "obstacles\\shellcreeper.bmp", 0);

	char characterimage[8][30] = { "characterimage\\mario1.bmp", "characterimage\\mario2.bmp", "characterimage\\mario3.bmp", "characterimage\\mario4.bmp", "characterimage\\mario5.bmp", "characterimage\\mario6.bmp", "characterimage\\mario7.bmp", "characterimage\\mario8.bmp" };
	iShowBMP2(mario_running, mario_jumping, characterimage[mario], 0);
}

void startbuttonclickhandler(){
	homepage = 0;
	startpage = 1;
}
void aboutbuttonclickhandler(){
	homepage = 0;
	startpage = 0;
	aboutpage = 1;
}
void easybuttonclickhandler(){
	homepage = 0;
	startpage = 0;
	aboutpage = 0;
	easypage = 1;
}
void mediumbuttonclickhandler(){
	homepage = 0;
	startpage = 0;
	aboutpage = 0;
	mediumpage = 1;
}
void hardbuttonclickhandler(){
	homepage = 0;
	startpage = 0;
	aboutpage = 0;
	hardpage = 1;
}
void backbuttonclickhandler(){
	homepage = 1;
	startpage = 0;
	aboutpage = 0;

}
void iDraw()
{
	iClear();
	if (homepage == 1)
	{
		home_page();
	}
	else if (startpage == 1)
	{
		start_page();
	}
	else if (aboutpage == 1)
	{
		about_page();
	}
	else if (easypage == 1)
	{
		easy_page();
	}
	else if (mediumpage == 1)
	{
		medium_page();
	}
	else if (hardpage == 1)
	{
		hard_page();
	}
}
void iMouseMove(int mx, int my)
{

}
void iPassiveMouseMove(int mx, int my)
{

}


void iMouse(int button, int state, int mx, int my)
{

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		//	printf("x =%d y=%d \n", mx, my);


		if (homepage == 1 && (mx >= 350 && mx <= 735) && (my > 312 && my <= 340))
		{
			startbuttonclickhandler();
		}
		else if (homepage == 1 && (mx >= 350 && mx <= 735) && (my > 230 && my <= 295))

		{
			aboutbuttonclickhandler();
		}
		else if (startpage == 1 && (mx >= 123 && mx <= 635) && (my > 452 && my <= 548)){
			easybuttonclickhandler();
		}
		else if (startpage == 1 && (mx >= 123 && mx <= 635) && (my > 300 && my <= 380)){
			mediumbuttonclickhandler();
		}
		else if (startpage == 1 && (mx >= 123 && mx <= 635) && (my > 100 && my <= 180)){
			hardbuttonclickhandler();
		}
		else if (startpage == 1 || aboutpage == 1 && (mx >= 1 && mx <= 50) && (my > 669 && my <= 690)){
			backbuttonclickhandler();
		}

		else{

			//homepage = 0;
			startpage = 0;
			aboutpage = 0;

		}
	}

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{

	}
}

void iKeyboard(unsigned char key)
{
	if (key == '\m')
	{

		if (musicOn)
		{
			musicOn = false;
			PlaySound(0, 0, 0);

		}
		else{
			musicOn = true;
			PlaySound("music\\sound.wav", NULL, SND_LOOP | SND_ASYNC);
		}
	}

}
void iSpecialKeyboard(unsigned char key)
{
	if (key == GLUT_KEY_RIGHT)
	{
		mario_running += 50;
		mario += 1;
		if (mario >= 7)
		{
			mario = 0;
		}
		if (mario_running >= 700)
		{
			mario_running = 700;
		}
		obstacle_movement();
		obstacle_movement_2();


	}
	if (key == GLUT_KEY_LEFT)
	{
		mario_running -= 50;
		mario -= 1;
		if (mario <= 0)
		{
			mario = 7;
		}
		if (mario_running <= 0)
		{
			mario_running = 0;
		}
		obstacle_movement_reverse();
		obstacle_movement_reverse_2();
	}

	if (key == GLUT_KEY_HOME)
	{

	}
	if (key == GLUT_KEY_UP)
	{
		mario_jumping += 70;
		if (mario_jumping > 250)
			mario_jumping = 250;

	}

}
void obstacle_movement(){
	x = x - 15;
	if (x + 800<0){
		x = 1280;
	}collision();
	life();

}
void obstacle_movement_2(){
	c = c - 15;
	if (c < 0){
		c = 1280;
	}
}
void obstacle_movement_reverse(){
	x = x + 15;
	if (x + 800>1280){
		x = 1280;
	}collision();
	life();

}
void obstacle_movement_reverse_2(){
	c = c + 15;
	if (c >1280){
		c = 1280;
	}
}
void gravity(){
	if (mario_jumping != 0)
	{
		mario_jumping -= 40;
	}
	if (mario_jumping < 0)
	{
		mario_jumping = 0;
	}
}
int main()
{
	if (musicOn)
	{
		PlaySound("music\\sound.wav", NULL, SND_LOOP | SND_ASYNC);
	}
	iSetTimer(300, gravity);
	iInitialize(1280, 704, "Super Mario");

	iStart();

	return 0;
}