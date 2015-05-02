#include <stdio.h>    	// - Just for some ASCII messages
#include <string>
#include <stdlib.h>     //srand, rand
#include <time.h>       //time
#include <math.h>
#include <iostream>
#include <fstream>
#include "windows.h"
#include "gl/glut.h"   //An interface and windows management library
#include "visuals.h"   //Header file for functions

#include "irk/ik_ISoundEngine.h"
#include "irk/irrKlang.h"
#pragma comment(lib, "irk/irrKlang.lib")
using namespace irrklang;
ISoundEngine* engine = createIrrKlangDevice();

model md;
static float tx = 0.0;
static float ty = -20.0;
static float rotx = 0.0;
static float roty = 0.0;

static float zoom = -100;
float zoomx = 0, zoomy = 0;
bool iAmPilot = false;

static float manspeed = 1.5;
static int manside = 0;
static int rmanside = -1;
static int mani = 0;
static int maniup = 0;

bool evaporation = true;
float bigInnerFlame=4.2;
float smallInnerFlames=2.9;

static bool animate = false;
static float sunred = 0.8;
static float sungreen = 0.8;
static float sunblue = 0.0;

static float rsunred = 1.0;
static float rsungreen = 0.0;
static float rsunblue = 0.0;
static float rsuntrans = 0.2;
static bool rsunt = false;
float rsunRadius = 15.0;

float angle = 0.0;
float rotangle = 0.0;
float rotangleup = 0.0;

bool pause = true;
bool pauseBeforeHelp = false;
bool help = true;

int numstars = 30;
float mystars[30][3];

float asteroid[3];
float aster_rot = 0.0;
float aster_scale = 0.015;
float asterRadius = 9.0;
int aster_color = 0;

float bulletx = 0.0;
float bullety = 0.0;
float bulletz = 0.0;
bool fire = false;
bool gotit = false;

using namespace std;
bool lost = false;
bool point = false;
int score=0;

char name[10] = "         ";
int character = 0;
bool changeName = false;

void keimeno(const char *str,float size)
{
	glPushMatrix();
	glScalef(size,size,size);
	int i = 0;
	for (i=0;i<strlen(str);i++)
		glutStrokeCharacter(GLUT_STROKE_ROMAN,str[i]);
	glPopMatrix();
}

void bullet(){
	glPushMatrix();
	glTranslatef(bulletx, bullety, bulletz);
	glScalef(0.15, 0.15, 1.5);
	glColor3f(1.0, 0.0, 0.0);
	glutSolidSphere(7.0, 24, 24);
	glPopMatrix();
}

void spaceship()
{
	glPushMatrix();
	glRotatef(180, 0, 1, 0);
	glTranslatef(tx, ty, 0.0);
	glRotatef(rotangle, 0, 0, 1);
	glRotatef(rotangleup, 1, 0, 0);
	//pilot's face
	glPushMatrix();
	glTranslatef(0.0, 7.0, 10.0);
	glColor3f(0.8, 0.2, 0.0);
	glutSolidSphere(2.0, 24, 24);
	glPopMatrix();

	//pilot's eye 1
	glPushMatrix();
	glTranslatef(-0.6, 8.0, 12.0);
	glColor3f(0.0, 0.0, 0.0);
	glutSolidSphere(0.5, 24, 24);
	glPopMatrix();

	//pilot's eye 2
	glPushMatrix();
	glTranslatef(0.6, 8.0, 12.0);
	glColor3f(0.0, 0.0, 0.0);
	glutSolidSphere(0.5, 24, 24);
	glPopMatrix();

	//pilot's room
	glPushMatrix();
	glTranslatef(0.0, 6.0, 10.0);
	glColor4f(1.0, 1.0, 1.0, 0.5);
	glutSolidSphere(4.0, 24, 24);
	glPopMatrix();

	//Body
	glPushMatrix();
	glScalef(1.0, 1.0, 3.0);
	glColor3f(0.556863, 0.137255, 0.137255);
	glutSolidSphere(7.0, 24, 24);
	glPopMatrix();

	//name of spaceship
	glPushMatrix();
	glColor3f(0.1, 0.1, 0.1);
	glTranslatef(-4.58, 0, -17);
	glRotatef(-13,0,1,0);
	glRotatef(-90,0,1,0);
	keimeno(name, 0.02f);
	glColor3f(0.1, 0.1, 0.1);
	glPopMatrix();

	//wings
	glPushMatrix();
	glScalef(5.0, 1.0, 1.0);
	glColor3f(0.3, 0.3, 0.3);
	glutSolidSphere(5.0, 24, 24);
	glPopMatrix();

	//tail support
	glPushMatrix();
	glRotatef(40.0, 1.0, 0.0, 0.0);
	glTranslatef(0.0, -2.5, -15.0);
	glScalef(1.0, 1.0, 1.5);
	glColor3f(0.5, 0.5, 0.5);
	glutSolidSphere(4.5, 24, 24);
	glPopMatrix();

	//tail
	glPushMatrix();
	glTranslatef(0.0, 8.0, -15.0);
	glScalef(4.0, 1.0, 1.0);
	glColor3f(0.3, 0.3, 0.3);
	glutSolidSphere(4.0, 24, 24);
	glPopMatrix();
	
	//propeller support 1
	glPushMatrix();
	glTranslatef(-17.0, 0.0, 1.5);
	glColor3f(0.556863, 0.137255, 0.137255);
	glutSolidSphere(3.5, 24, 24);
	glPopMatrix();

	//propeller 1
	glPushMatrix();
	glTranslatef(-17.0, 0.0, 6.5);
	glRotatef(angle*(manspeed + 0.5), 0.0, 0.0, 1.0);
	glScalef(4.0, 1.0, 1.0);
	glColor3f(0.5, 0.5, 0.5);
	glutSolidSphere(1.5, 24, 24);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-17.0, 0.0, 6.5);
	glRotatef(angle*(manspeed + 0.5) + 90.0, 0.0, 0.0, 1.0);
	glScalef(4.0, 1.0, 1.0);
	glColor3f(0.5, 0.5, 0.5);
	glutSolidSphere(1.5, 24, 24);
	glPopMatrix();

	//propeller support 2
	glPushMatrix();
	glTranslatef(17.0, 0.0, 1.5);
	glColor3f(0.556863, 0.137255, 0.137255);
	glutSolidSphere(3.5, 24, 24);
	glPopMatrix();

	//propeller 2
	glPushMatrix();
	glTranslatef(17.0, 0.0, 6.5);
	glRotatef(angle*(manspeed+0.5), 0.0, 0.0, 1.0);
	glScalef(4.0, 1.0, 1.0);
	glColor3f(0.5, 0.5, 0.5);
	glutSolidSphere(1.5, 24, 24);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(17.0, 0.0, 6.5);
	glRotatef(angle*(manspeed + 0.5) + 90.0, 0.0, 0.0, 1.0);
	glScalef(4.0, 1.0, 1.0);
	glColor3f(0.5, 0.5, 0.5);
	glutSolidSphere(1.5, 24, 24);
	glPopMatrix();

	//main spaceship evaporation
	glPushMatrix();
	glTranslatef(0,1,-20);

	//support
	glPushMatrix();
	glRotatef(8,1,0,0);
	glColor3f(0.3,0.3,0.3);
	glutSolidTorus(0.95,0.955,30,24);
	glPopMatrix();

	//inner flame
	glPushMatrix();
	glRotatef(180, 1, 0, 0);
	glColor3f(1,0,0);
	if(evaporation)
		glutSolidCone(0.5,bigInnerFlame,30,24);
	glPopMatrix();

	//outer flame
	glPushMatrix();
	glScalef(1,1,7);
	glColor4f(1,0.4,0,0.3);
	glutSolidSphere(0.9,30,24);
	glPopMatrix();

	glPopMatrix();

	//left spaceship evaporation
	glPushMatrix();
	glTranslatef(1.0,-1.5,-20);

	//support
	glPushMatrix();
	glRotatef(-13,1,0,0);
	glColor3f(0.3,0.3,0.3);
	glutSolidTorus(0.752,0.7521,30,24);
	glPopMatrix();

	//inner flame
	glPushMatrix();
	glRotatef(180, 1, 0, 0);
	glColor3f(1,0,0);
	if(!evaporation)
		glutSolidCone(0.35,smallInnerFlames,30,24);
	glPopMatrix();

	//outer flame
	glPushMatrix();
	glScalef(1,1,6);
	glColor4f(1,0.4,0,0.3);
	glutSolidSphere(0.75,30,24);
	glPopMatrix();

	glPopMatrix();

	//right spaceship evaporation
	glPushMatrix();
	glTranslatef(-1.0,-1.5,-20);

	//support
	glPushMatrix();
	glRotatef(-13,1,0,0);
	glColor3f(0.3,0.3,0.3);
	glutSolidTorus(0.752,0.7521,30,24);
	glPopMatrix();

	//inner flame
	glPushMatrix();
	glRotatef(180, 1, 0, 0);
	glColor3f(1,0,0);
	if(!evaporation)
		glutSolidCone(0.35,smallInnerFlames,30,24);
	glPopMatrix();

	//outer flame
	glPushMatrix();
	glScalef(1,1,6);
	glColor4f(1,0.4,0,0.3);
	glutSolidSphere(0.75,30,24);
	glPopMatrix();

	glPopMatrix();

	glPopMatrix();
}

void maneuvers(){
	if (manside == 2){
		if (manside == rmanside && mani>0){
			manside = 1;
		}
		else{
			if (!(rotangle >= 45)){
				rotangle += (0.1*(manspeed));
				mani++;
				if (mani == 0){
					if (maniup < 0){ manside = 4; }
					else if (maniup > 0){ manside = 3; }
				}
			}
		}
	}
	else if (manside == 1 ){
		if (manside == rmanside && mani < 0){
			manside = 2;
		}
		else{
			if (!(rotangle <= -45)){
				rotangle -= (0.1*(manspeed));
				mani--;
				if (mani == 0){
					if (maniup < 0){ manside = 4; }
					else if (maniup > 0){ manside = 3; }
				}
			}
		}
	}
	else if (manside == 3){
		if (manside == rmanside && maniup < 0){
			manside = 4;
		}
		else{
			if (!(rotangleup >= 60)){
				rotangleup += (0.1*(manspeed));
				maniup--;
				if (maniup == 0){
					if (mani < 0){ manside = 2; }
					else if (mani > 0){ manside = 1; }
				}
			}
		}
	}
	else if (manside == 4){
		if (manside == rmanside && maniup > 0){
			manside = 3;
		}
		else{
			if (!(rotangleup <= -60)){
				rotangleup -= (0.1*(manspeed));
				maniup++;
				if (maniup == 0){
					if (mani<0){ manside = 2; }
					else if (mani>0){ manside = 1; }
				}
			}
		}
	}
	if (mani == 0 && maniup == 0){
		rotangle = 0;
		rotangleup = 0;
		manside = 0;
		rmanside = -1;
	}
}

void lostMessage()
{
	glPushMatrix();
	glColor3f(0.8, 0.1, 0.1);
	glTranslatef(-17.0, 0.0, -40.0);
	keimeno("You Lose!", 0.05f);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.8, 0.1, 0.1);
	glScalef(0.5, 0.5, 1.0);
	glTranslatef(-32.0, -32.0, -40.0);
	keimeno("Press r to restart", 0.05f);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.8, 0.1, 0.1);
	glScalef(0.5, 0.5, 1.0);
	glTranslatef(-32.0, -40.0, -40.0);
	keimeno("Press esc to quit", 0.05f);
	glPopMatrix();
}

void helpPage()
{
	glPushMatrix();
	glColor3f(0.8, 0.1, 0.1);
	glTranslatef(-20.0, 50.0, -120.0);
	keimeno("Help Page", 0.06f);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.8, 0.1, 0.1);
	glTranslatef(-110.0, 30.0, -120.0);
	keimeno("Press h to show/hide help page", 0.05f);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.8, 0.1, 0.1);
	glTranslatef(-110.0, 18.0, -120.0);
	keimeno("Press w to move up", 0.05f);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.8, 0.1, 0.1);
	glTranslatef(-110.0, 6.0, -120.0);
	keimeno("Press s to move down", 0.05f);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.8, 0.1, 0.1);
	glTranslatef(-110.0, -6.0, -120.0);
	keimeno("Press a to move left", 0.05f);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.8, 0.1, 0.1);
	glTranslatef(-110.0, -18.0, -120.0);
	keimeno("Press d to move right", 0.05f);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.8, 0.1, 0.1);
	glTranslatef(-110.0, -30.0, -120.0);
	keimeno("Press f to fire", 0.05f);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.8, 0.1, 0.1);
	glTranslatef(-110.0, -42.0, -120.0);
	keimeno("Press +/- to speed up/down", 0.05f);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.8, 0.1, 0.1);
	glTranslatef(-110.0, -54.0, -120.0);
	keimeno("Press 0 to set spaceship's name", 0.05f);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.8, 0.1, 0.1);
	glTranslatef(10.0, 30.0, -120.0);
	keimeno("Press i to zoom in", 0.05f);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.8, 0.1, 0.1);
	glTranslatef(10.0, 18.0, -120.0);
	keimeno("Press o to zoom out", 0.05f);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.8, 0.1, 0.1);
	glTranslatef(10.0, 6.0, -120.0);
	keimeno("Press 2/8 to rotate on x axis", 0.05f);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.8, 0.1, 0.1);
	glTranslatef(10.0, -6.0, -120.0);
	keimeno("Press 4/6 to rotate on y axis", 0.05f);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.8, 0.1, 0.1);
	glTranslatef(10.0, -18.0, -120.0);
	keimeno("Press v to set first person view", 0.05f);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.8, 0.1, 0.1);
	glTranslatef(10.0, -30.0, -120.0);
	keimeno("Press r to restart game", 0.05f);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.8, 0.1, 0.1);
	glTranslatef(10.0, -42.0, -120.0);
	keimeno("Press p to pause/resume game", 0.05f);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.8, 0.1, 0.1);
	glTranslatef(10.0, -54.0, -120.0);
	keimeno("Press esc to quit game", 0.05f);
	glPopMatrix();
}

void scoreMessage()
{
	char s[10];

	glPushMatrix();
	glColor3f(0.8, 0.1, 0.1);
	glTranslatef(-37.0, 20.0, -40.0);
	glScalef(0.5, 0.5, 1.0);
	keimeno("Score:", 0.05f);
	glPopMatrix();
	glPushMatrix();
	glColor3f(0.8, 0.1, 0.1);
	glTranslatef(-25.0, 20.0, -40.0);
	glScalef(0.5, 0.5, 1.0);
	sprintf(s, "%d", score);
	keimeno(s, 0.05f);
	glPopMatrix();
}

void sun(){
	glPushMatrix();
	glTranslatef(150,  120, -250);
	glColor3f(sunred, sungreen, sunblue);
	glutSolidSphere(15.0, 30.0, 24.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(150, 120, -250);
	glColor4f(rsunred, rsungreen, rsunblue,rsuntrans);
	glutSolidSphere(rsunRadius, 30.0, 24.0);
	glPopMatrix();
}

void star(){
	glPushMatrix();
	glColor3f(0.90, 0.91, 0.98);
	glutSolidSphere(0.2, 30.0, 24.0);
	glPopMatrix();
}

void initStars(){
	int i=0;
	for(i=0; i<numstars; i++){
		mystars[i][2] = 101.0;
	}

	mystars[0][0] = 0;		//x
	mystars[0][1] = 90;		//y
	mystars[1][0] = 35;		//x
	mystars[1][1] = 25;		//y
	mystars[2][0] = 67;		//x
	mystars[2][1] = 28;		//y
	mystars[3][0] = 26;		//x
	mystars[3][1] = 21;		//y
	mystars[4][0] = 42;		//x
	mystars[4][1] = 42;		//y
	mystars[5][0] = 31;		//x
	mystars[5][1] = 18;		//y
	mystars[6][0] = 23;		//x
	mystars[6][1] = 12;		//y
	mystars[7][0] = 37;		//x
	mystars[7][1] = 22;		//y
	mystars[8][0] = 40;		//x
	mystars[8][1] = 39;		//y
	mystars[9][0] = 77;		//x
	mystars[9][1] = 3;		//y
	mystars[10][0] = 83;	//x
	mystars[10][1] = 2;		//y
	mystars[11][0] = 91;	//x
	mystars[11][1] = 15;	//y
	mystars[12][0] = 50;	//x
	mystars[12][1] = 20;	//y
	mystars[13][0] = 65;	//x
	mystars[13][1] = 35;	//y
	mystars[14][0] = 47;	//x
	mystars[14][1] = 9;		//y
	mystars[15][0] = 30;	//x
	mystars[15][1] = 50;	//y
	mystars[16][0] = 62;	//x
	mystars[16][1] = 42;	//y
	mystars[17][0] = 71;	//x
	mystars[17][1] = 18;	//y
	mystars[18][0] = 39;	//x
	mystars[18][1] = 12;	//y
	mystars[19][0] = 57;	//x
	mystars[19][1] = 22;	//y
	mystars[20][0] = 0;		//x
	mystars[20][1] = 36;	//y
	mystars[21][0] = 11;	//x
	mystars[21][1] = 57;	//y
	mystars[22][0] = 30;	//x
	mystars[22][1] = 20;	//y
	mystars[23][0] = 33;	//x
	mystars[23][1] = 24;	//y
	mystars[24][0] = 28;	//x
	mystars[24][1] = 19;	//y
	mystars[25][0] = 40;	//x
	mystars[25][1] = 40;	//y
	mystars[26][0] = 20;	//x
	mystars[26][1] = 50;	//y
	mystars[27][0] = 70;	//x
	mystars[27][1] = 20;	//y
	mystars[28][0] = 85;	//x
	mystars[28][1] = 25;	//y
	mystars[29][0] = 37;	//x
	mystars[29][1] = 46;	//y
}

void generateStar(int i)
{
	int positivex, positivey, dimx, dimy, dimz;

	if(mystars[i][2] >= 100){

		positivex = rand()%2;
		positivey = rand()%2;

		dimx = mystars[i][0];
		dimy = mystars[i][1];
		dimz = rand()%401;

		if (positivex == 1 && positivey == 1){
			glTranslatef(dimx, dimy, (-1)*dimz);
			mystars[i][0] = dimx;
			mystars[i][1] = dimy;
			mystars[i][2] = (-1)*dimz;
		}else if (positivex == 1 && positivey == 0){
			glTranslatef(dimx, (-1)*dimy, (-1)*dimz);
			mystars[i][0] = dimx;
			mystars[i][1] = (-1)*dimy;
			mystars[i][2] = (-1)*dimz;
		}else if (positivex == 0 && positivey == 1){
			glTranslatef((-1)*dimx, dimy, (-1)*dimz);
			mystars[i][0] = (-1)*dimx;
			mystars[i][1] = dimy;
			mystars[i][2] = (-1)*dimz;
		}else{
			glTranslatef((-1)*dimx, (-1)*dimy, (-1)*dimz);
			mystars[i][0] = (-1)*dimx;
			mystars[i][1] = (-1)*dimy;
			mystars[i][2] = (-1)*dimz;
		}
	}else{
		glTranslatef(mystars[i][0], mystars[i][1], mystars[i][2]);
	}
}

void generateAsteroid()
{
	int positivex, positivey, dimx, dimy, dimz;

	if (asteroid[2] >= 400){
		gotit = false;
		positivex = rand() % 2;
		positivey = rand() % 2;
		dimx = rand() % 50;
		dimy = rand() % 50;
		dimz = 400;
		aster_rot = 0.0;
		int randR = rand() % 3;
		aster_scale = (double)randR/200 + 0.010;
		if(randR == 0){
			asterRadius = 6.0;
		}else if(randR == 1){
			asterRadius = 9.0;
		}else if(randR == 2){
			asterRadius = 12.0;
		}
		aster_color = rand() % 5;

		if (positivex == 1 && positivey == 1){
			glTranslatef(dimx, dimy, (-1)*dimz);
			asteroid[0] = dimx;
			asteroid[1] = dimy;
			asteroid[2] = (-1)*dimz;
		}else if (positivex == 1 && positivey == 0){
			glTranslatef(dimx, (-1)*dimy, (-1)*dimz);
			asteroid[0] = dimx;
			asteroid[1] = (-1)*dimy;
			asteroid[2] = (-1)*dimz;
		}else if (positivex == 0 && positivey == 1){
			glTranslatef((-1)*dimx, dimy, (-1)*dimz);
			asteroid[0] = (-1)*dimx;
			asteroid[1] = dimy;
			asteroid[2] = (-1)*dimz;
		}else{
			glTranslatef((-1)*dimx, (-1)*dimy, (-1)*dimz);
			asteroid[0] = (-1)*dimx;
			asteroid[1] = (-1)*dimy;
			asteroid[2] = (-1)*dimz;
		}
	}else{
		glTranslatef(asteroid[0], asteroid[1], asteroid[2]);
	}
}

void collision()
{
	if(asteroid[1]<1 + asterRadius +ty && asteroid[1]>-1 - asterRadius +ty){
		if(asteroid[0]<1 + asterRadius -tx && asteroid[0]>-1 - asterRadius -tx){
			if (asteroid[2]<21 + asterRadius && asteroid[2]>-21 - asterRadius){
				//collision with body
				pause = true;
				lost = true;
				engine->removeSoundSource("media/The DoorsRiders on the storm.mp3");
				engine->play2D("media/explosion.wav",false);
			}
		}else if((asteroid[0]<19 + asterRadius -tx && asteroid[0]>-13 - asterRadius -tx)){
			if (asteroid[2]>-3 - asterRadius && asteroid[2]< 3 + asterRadius){
				//collision with wings
				pause = true;
				lost = true;
				engine->removeSoundSource("media/The DoorsRiders on the storm.mp3");
				engine->play2D("media/explosion.wav",false);
			}
		}
	}else if(asteroid[1]<6 + asterRadius +ty && asteroid[1]>-6 - asterRadius +ty){
		if(asteroid[0]<6 + asterRadius -tx && asteroid[0]>-6 - asterRadius -tx){
			if (asteroid[2]<11 + asterRadius && asteroid[2]>-11 - asterRadius){
				//collision with main body
				pause = true;
				lost = true;
				engine->removeSoundSource("media/The DoorsRiders on the storm.mp3");
				engine->play2D("media/explosion.wav",false);
			}
		}
	}else if(asteroid[1]<10 + asterRadius +ty && asteroid[1]>=ty){
		if (asteroid[2]<10 + asterRadius && asteroid[2]>-10 - asterRadius){
			if(asteroid[0]<6 + asterRadius -tx && asteroid[0]>-6 - asterRadius -tx){
				//collision with main body from the top
				pause = true;
				lost = true;
				engine->removeSoundSource("media/The DoorsRiders on the storm.mp3");
				engine->play2D("media/explosion.wav",false);
			}
		}else if (asteroid[2]<21 + asterRadius && asteroid[2]> 9 + asterRadius){
			if(asteroid[0]<11 + asterRadius -tx && asteroid[0]>-11 - asterRadius -tx){
				//collision with tail
				pause = true;
				lost = true;
				engine->removeSoundSource("media/The DoorsRiders on the storm.mp3");
				engine->play2D("media/explosion.wav",false);
			}
		}
	}
}

void Render()
{
	/* initialize random seed: */
	srand (time(NULL));

	//CLEARS FRAME BUFFER ie COLOR BUFFER& DEPTH BUFFER (1.0)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Clean up the colour of the window
													   // and the depth buffer
	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity();

	if (lost && !help){
		lostMessage();
	}

	if(help == true){
		helpPage();
	}else{
		scoreMessage();
		if (iAmPilot){
			glRotatef(rotangleup, 1, 0, 0);
			glRotatef(rotangle, 0, 0, 1);
		}
		glTranslatef(zoomx, -zoomy,zoom);
		glRotatef(rotx, 1, 0, 0);
		glRotatef(roty, 0, 1, 0);

		GLfloat light_position[] = { 140, 120, -219.0, 1.0 };
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);

		int i=0;

		for(i=0; i<numstars; i++){
			glPushMatrix();
			generateStar(i);
			star();
			glPopMatrix();
		}

		glPushMatrix();
		generateAsteroid();
		if (!gotit){
			DisplayModel(md);
		}
		glPopMatrix();

		if (fire){
			bullet();
		}
  
		spaceship();
		sun();
  }
	glutSwapBuffers();             // All drawing commands applied to the 
                                 // hidden buffer, so now, bring forward
                                 // the hidden buffer and hide the visible one
}

//-----------------------------------------------------------

void Resize(int w, int h)
{ 
	// define the visible area of the window ( in pixels )
	if (h==0) h=1;
	glViewport(0,0,w,h); 

	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();
 
	gluPerspective(60.0, (float)w/(float)h, 1.0, 500.0);
}

void Idle()
{
	printf("tx = %lf, zoomx = %lf, ty = %lf, zoomy = %lf\n", tx, zoomx, ty, zoomy);
	if(pause == false){
		evaporation = !evaporation;

		int i = 0;
		for(i=0; i<numstars; i++){
			if(mystars[i][2] < 100){
				mystars[i][2] = mystars[i][2] + 0.05*manspeed;
			}
		}

		if (asteroid[2] < 400){
			asteroid[2] = asteroid[2] + 0.2*manspeed;
			aster_rot = aster_rot + 0.5;
		}
		angle += 0.1;
		if (rsuntrans >= 0.7 || rsuntrans <= 0.2){
			rsunt = (!rsunt); 
		}
		if (rsunt){ 
			rsuntrans += 0.0005;
			rsunRadius += 0.015;
		}
		else{ 
			rsuntrans -= 0.0005;
			rsunRadius -= 0.015;
		}

		maneuvers();

		collision();

		if (asteroid[2]<30 && asteroid[2]>-30){
			if (!lost){ point = true; }
		}
		
		if (point && asteroid[2] > 30){
			score--;
			if (score < 0){
				score = 0;
			}
			point = false;
		}
		if (fire == true){
			bulletz = bulletz - 5;
			if (bulletz <= -400){
				bulletz = 0;
				fire = false;
			}
			if (bulletx<asteroid[0] + 10 && bulletx>asteroid[0] - 10 && bullety<asteroid[1] + 9 
				&& bullety>asteroid[1] - 9 && bulletz < asteroid[2] + 10 && bulletz > asteroid[2] -10){
				bulletz = 0;
				fire = false;
				asteroid[2] = 100;
				gotit = true;
				score = score + (((int)(18.0 - asterRadius)) / 3);
			}
		}
	}

	glutPostRedisplay();
}

void Keyboard(unsigned char key,int x,int y)
{
	switch(key)
	{
	case 27 : 
		engine->drop(); // delete engine
		exit(0);
		break;
	case '0' :
		if(!lost && !help){
			character = 0;
			changeName = !changeName;
			if(!pauseBeforeHelp){
				pause = !pause;
			}

			if(changeName){
				int i;
				for(i=0; i<9; i++){
					name[i] = 32;
				}
			}
		}
		break;
	case 'v' :
		if(!iAmPilot){
			zoom = 12.0;
			zoomx = tx;
			zoomy = ty+8.0;
			roty = 0;
			rotx = 0;
		}else{
			rotx = 0;
			roty = 0;
			zoom = -100;
			zoomx = 0.0;
			zoomy = 0.0;
		}
		iAmPilot = !iAmPilot;
		break;
	case 'f':
		if(!lost && !help && !changeName){
			fire = true;
			bulletx = -tx;
			bullety = ty;
			engine->play2D("media/shoot.wav");
		}else if(changeName){
			if(character <= 8){
				name[character] = key;
				character++;
			}else{
				engine->play2D("media/Windows Hardware Insert.wav");
			}
		}
		break;
	case 'r':
		if(!changeName){
			mani = 0;
			maniup = 0;
			rotangle = 0;
			rotangleup = 0;
			iAmPilot = false;
			asteroid[2]=400;
			tx = 0;
			ty = -20;
			rotx = 0;
			roty = 0;
			zoom = -100;
			zoomx = 0.0;
			zoomy = 0.0;
			help = false;
			pause = false;
			lost = false;
			score = 0;
			point = false;
			manspeed = 1.5;
			manside = 0;
			rmanside = -1;
			mani = 0;
			maniup = 0;
			evaporation = true;
			bigInnerFlame=4.2;
			smallInnerFlames=2.9;
			engine->removeSoundSource("media/The DoorsRiders on the storm.mp3");
			engine->play2D("media/The DoorsRiders on the storm.mp3",true);
		}else if(changeName){
			if(character <= 8){
				name[character] = key;
				character++;
			}else{
				engine->play2D("media/Windows Hardware Insert.wav");
			}
		}
		break;
	case 'i':
		if (!help && !changeName && !iAmPilot){
			if (zoom <= -80){
				zoom = zoom + 5;
			}
		}else if(changeName){
			if(character <= 8){
				name[character] = key;
				character++;
			}else{
				engine->play2D("media/Windows Hardware Insert.wav");
			}
		}
		break;
	case 'o':
		if (!help && !changeName && !iAmPilot){
			if (zoom >= -110){
				zoom = zoom - 5;
			}
		}else if(changeName){
			if(character <= 8){
				name[character] = key;
				character++;
			}else{
				engine->play2D("media/Windows Hardware Insert.wav");
			}
		}
		break;
	case 'd' :
		if(pause == false && !changeName){
			if (!(tx <= -85)){
				tx -= manspeed;
				manside = 2;
				if(iAmPilot){
					zoomx -= manspeed;
				}
			}
		}else if(changeName){
			if(character <= 8){
				name[character] = key;
				character++;
			}else{
				engine->play2D("media/Windows Hardware Insert.wav");
			}
		}
		break;
	case 'a': 
		if(pause == false && !changeName){
			if (!(tx >= 85)){
				tx += manspeed;
				manside = 1;

				if(iAmPilot){
					zoomx += manspeed;
				}
			}
		}else if(changeName){
			if(character <= 8){
				name[character] = key;
				character++;
			}else{
				engine->play2D("media/Windows Hardware Insert.wav");
			}
		}
		break;
	case 'w':
		if(pause == false && !changeName){
			if (!(ty >= 50)){
				ty += manspeed;
				manside = 4;

				if(iAmPilot){
					zoomy += manspeed;
				}
			}
		}else if(changeName){
			if(character <= 8){
				name[character] = key;
				character++;
			}else{
				engine->play2D("media/Windows Hardware Insert.wav");
			}
		}
		break;
	case 's':
		if(pause == false && !changeName){
			if (!(ty <= -50)){
				ty -= manspeed;
				manside = 3;

				if(iAmPilot){
					zoomy -=manspeed;
				}
			}
		}else if(changeName){
			if(character <= 8){
				name[character] = key;
				character++;
			}else{
				engine->play2D("media/Windows Hardware Insert.wav");
			}
		}
		break;
	case '2':
		if (!lost && !help && !iAmPilot)
			rotx -= 2.0f;
		break;
	case '8':
		if (!lost && !help && !iAmPilot)
			rotx += 2.0f;
		break;
	case '4':
		if (!lost && !help && !iAmPilot)
			roty += 2.0f;
		break;
	case '6':
		if (!lost && !help && !iAmPilot)
			roty -= 2.0f;
		break;
	case '+':
		if(pause == false){
			if (manside == 0){
				if (manspeed <= 3){
					manspeed += 0.2;
					bigInnerFlame+=0.2;
					smallInnerFlames+=0.15;
				}
			}
		}
		break;
	case '-':
		if(pause == false){
			if (manside == 0){
				if (manspeed >= 1.5){
					manspeed -= 0.2;
					bigInnerFlame-=0.2;
					smallInnerFlames-=0.15;
				}
			}
		}
		break;
	case 'p':
		if (!lost && !help && !changeName){
			pause = !pause;
			pauseBeforeHelp = !pauseBeforeHelp;
		}else if(changeName){
			if(character <= 8){
				name[character] = key;
				character++;
			}else{
				engine->play2D("media/Windows Hardware Insert.wav");
			}
		}
		break;
	case 'h':
		if(help == true && !changeName){
			help = false;
			if(!lost && !pauseBeforeHelp){
				pause = false;
			}
		}else if(changeName){
			if(character <= 8){
				name[character] = key;
				character++;
			}else{
				engine->play2D("media/Windows Hardware Insert.wav");
			}
		}else{
			help = true;
			pause = true;
		}
		break;
	default : 
		if(changeName && (key==32 || (key>=65 && key<=90) || (key>=97 && key<=122))){
			if(character <= 8){
				name[character] = key;
				character++;
			}else{
				engine->play2D("media/Windows Hardware Insert.wav");
			}
		}
		break;
	}
	glutPostRedisplay();

}

void rKeyboard(unsigned char key, int x, int y){
	switch (key)
	{
	case 'd':
		rmanside = 2;
		break;
	case 'a':
		rmanside = 1;
		break;
	case 'w':
		rmanside = 4;
		break;
	case's':
		rmanside = 3;
		break;
	}
}

void Mouse(int button,int state,int x,int y)
{
	 if(state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
	 {
		 animate = !animate;
         glutPostRedisplay();
	 }
}


void Setup() 
{
	if (!engine){
		exit(0);
	}
	engine->play2D("media/The DoorsRiders on the storm.mp3",true);

	initStars();
	
	ReadFile(&md);

	//Parameter handling
	glShadeModel (GL_SMOOTH);
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClearDepth(1);
    
	// polygon rendering mode
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
  
	glEnable(GL_BLEND);
	// incoming //  // stored //
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   
	//Set up light source
	glEnable(GL_LIGHTING);

	GLfloat ambientLight[] = { 0.3, 0.3, 0.3, 1.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	GLfloat diffuseLight[] = { 0.8, 0.8, 0.8, 1.0 };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glEnable(GL_LIGHT0);

	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
 
	asteroid[2] = 400;
	// Black background
	glClearColor(0.0f,0.0f,0.0f,1.0f);

}

void MenuSelect(int choice)
{
	switch (choice) {
		case RED : 
			sunred = 1.0; 
			sungreen = 0.0; 
			sunblue = 0.0; 
			rsunred = 1.0;
			rsungreen = 0.5;
			rsunblue = 0.0;
			break;
		case WHITE : 
			sunred = 1.0;
			sungreen = 1.0;
			sunblue = 1.0;
			rsunred = 1.0;
			rsungreen = 1.0;
			rsunblue = 0.0; break;
		case YELLOW:
			sunred = 1.0;
			sungreen = 1.0;
			sunblue = 0.0; 
			rsunred = 1.0;
			rsungreen = 0.0;
			rsunblue = 0.0;  break;
	}

}

void ReadFile(model *md)
{
	int i = 0;
	(md->vertices) = 0;
	(md->vernorms) = 0;
	(md->faces) = 0;
	char line[150];
	char *tmp;
	char a;

	ifstream obj_file("asteroid.obj");
	if (obj_file.fail())
		exit(1);

	while (!obj_file.eof()){			//edw epla pernw ton ari8mo ton v ,vn kai f sto arxio
		obj_file.getline(line, 150);

		tmp = strtok(line, " ");

		if (tmp != NULL){
			if (strcmp(tmp, "v") == 0)
			{
				(md->vertices)++; 
			}
			else if (strcmp(tmp, "vn") == 0)
			{
				(md->vernorms)++;
			}
			else if (strcmp(tmp, "f") == 0)
			{ 
				(md->faces)++;
			}
		}
	}

	obj_file.close();

	md->obj_vertices = new vertice[md->vertices];
	md->obj_faces = new face[md->faces];
	md->obj_vernorms = new vernorm[md->vernorms];

	obj_file.open("asteroid.obj");
	if (obj_file.fail())
		exit(1);
	do{
		obj_file.getline(line, 150);
		tmp = strtok(line, " ");
	} while (tmp == NULL || strcmp(tmp, "v") != 0);
	
	tmp=strtok(NULL, " ");
	md->obj_vertices[0].x = atof(tmp);
	tmp = strtok(NULL, " ");
	md->obj_vertices[0].y = atof(tmp);
	tmp = strtok(NULL, " ");
	md->obj_vertices[0].z = atof(tmp);

	for (i = 1; i < md->vertices; i++){
		obj_file >> tmp;
		obj_file >> md->obj_vertices[i].x;
		obj_file >> md->obj_vertices[i].y;
		obj_file >> md->obj_vertices[i].z;
	}
	
	do{
		obj_file.getline(line, 150);
		tmp = strtok(line, " ");
	} while (tmp == NULL || strcmp(tmp, "vn") != 0);

	tmp = strtok(NULL, " ");
	md->obj_vernorms[0].x = atof(tmp);
	tmp = strtok(NULL, " ");
	md->obj_vernorms[0].y = atof(tmp);
	tmp = strtok(NULL, " ");
	md->obj_vernorms[0].z = atof(tmp);

	for (i = 1; i < md->vernorms; i++){
		obj_file >> tmp;
		obj_file >> md->obj_vernorms[i].x;
		obj_file >> md->obj_vernorms[i].y;
		obj_file >> md->obj_vernorms[i].z;
	}

	do{
		obj_file.getline(line, 150);
		tmp = strtok(line, " ");
	} while (tmp == NULL || strcmp(tmp, "f") != 0);

	tmp = strtok(NULL, "/");
	md->obj_faces[0].a = atoi(tmp);
	tmp = strtok(NULL, " ");
	md->obj_faces[0].na = atoi(tmp);

	tmp = strtok(NULL, "/");
	md->obj_faces[0].b = atoi(tmp);
	tmp = strtok(NULL, " ");
	md->obj_faces[0].nb = atoi(tmp);

	tmp = strtok(NULL, "/");
	md->obj_faces[0].c = atoi(tmp);
	tmp = strtok(NULL, " ");
	md->obj_faces[0].nc = atoi(tmp);

	for (i = 1; i < md->faces; i++){
		obj_file >> tmp;
		obj_file >> md->obj_faces[i].a;
		obj_file >> a;
		obj_file >> a;
		obj_file >> md->obj_faces[i].na;
		obj_file >> md->obj_faces[i].b;
		obj_file >> a;
		obj_file >> a;
		obj_file >> md->obj_faces[i].nb;
		obj_file >> md->obj_faces[i].c;
		obj_file >> a;
		obj_file >> a;
		obj_file >> md->obj_faces[i].nc;
	}

	obj_file.close();
	}

void DisplayModel(model md)
{	
	glPushMatrix();
	glScalef(aster_scale, aster_scale, aster_scale);
	glRotatef(aster_rot, 1,1, 1);

	glBegin(GL_TRIANGLES);
	int i;
	for (i = 0; i < md.faces; i++){
		if(aster_color == 0){
			glColor3f(0.329412, 0.329412, 0.329412);
		}else if(aster_color == 1){
			glColor3f(0.6, 0.35, 0.1);
		}else if(aster_color == 2){
			glColor3f(0.6, 0.5, 0.5);
		}else if(aster_color == 3){
			glColor3f(0.2, 0.2, 0.2);
		}else{
			glColor3f(0.2, 0.3, 0.4);
		}

		glVertex3f(md.obj_vertices[md.obj_faces[i].a - 1].x, md.obj_vertices[md.obj_faces[i].a - 1].y, md.obj_vertices[md.obj_faces[i].a - 1].z);
		glVertex3f(md.obj_vertices[md.obj_faces[i].b - 1].x, md.obj_vertices[md.obj_faces[i].b - 1].y, md.obj_vertices[md.obj_faces[i].b - 1].z);
		glVertex3f(md.obj_vertices[md.obj_faces[i].c - 1].x, md.obj_vertices[md.obj_faces[i].c - 1].y, md.obj_vertices[md.obj_faces[i].c - 1].z);

		glNormal3f(md.obj_vernorms[md.obj_faces[i].na - 1].x, md.obj_vernorms[md.obj_faces[i].na - 1].y, md.obj_vernorms[md.obj_faces[i].na - 1].z);
		glNormal3f(md.obj_vernorms[md.obj_faces[i].nb - 1].x, md.obj_vernorms[md.obj_faces[i].nb - 1].y, md.obj_vernorms[md.obj_faces[i].nb - 1].z);
		glNormal3f(md.obj_vernorms[md.obj_faces[i].nc - 1].x, md.obj_vernorms[md.obj_faces[i].nc - 1].y, md.obj_vernorms[md.obj_faces[i].nc - 1].z);
	
	}
	glEnd();
	glPopMatrix();
	
}
