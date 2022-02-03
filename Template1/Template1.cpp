// Template1.cpp : Defines the entry point for the console application.


#include "stdafx.h"
#include <Windows.h>
#include "include\GL\GL.H"
#include "include\GL\GLU.H"
#include "include\GL\glut.h"
#include <math.h>
#include <cstdio>
#include <SOIL2.h>
#include <time.h>
#include <iostream>

// angle of rotation for the camera direction
float anglex = 0.0f, angley = 0.0f;

// actual vector representing the camera's direction
float lx = 0.0f, lz = -1.0f, ly = 0.0f;

// XZ position of the camera
float x = 0.0f, z = 5.0f, y = 1.0f;

// the key states. These variables will be zero
//when no key is being presses
float deltaAnglex = 0.0f;
float deltaAngley = 0.0f;
float deltaMove = 0;
float deltaMovex = 0;
float speed = 0.0f;
int xOrigin = -1;
int yOrigin = -1;


float rAngle = 0.0;

#define PI 3.14159265359f
#define WORLD_G 9.80665f
#define SPHERE_MASS 0.1f
#define SPRING_K 1.5f

float global_x = 0, global_y = 0.0, global_z = -6;
float dx = 1.8f;
int angle = 0;
float velocity = 0.f;
clock_t currTime = clock();



void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;

	float ratio = w * 1.0 / h;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(50.0f, ratio, 0.1f, 1000.0f);
	

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}




void computePos(float deltaMove) {

	x += deltaMove * lx * 0.1f;
	z += deltaMove * lz * 0.1f;
	y += deltaMove * ly * 0.1f;

}



void PrintSphere(double size, GLuint textureID)
{
	
	GLUquadricObj* quadratic;
	quadratic = gluNewQuadric();
	gluQuadricNormals(quadratic, GL_SMOOTH);
	gluQuadricTexture(quadratic, GL_TRUE);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexCoord4d(0, 0, 0, 1);
	gluSphere(quadratic, size, 50, 50);
}

void Torus2d(float inner, float outer, unsigned int pts, GLuint textureID)
{
	GLUquadricObj* quadratic;
	quadratic = gluNewQuadric();
	gluQuadricNormals(quadratic, GL_SMOOTH);
	gluQuadricTexture(quadratic, GL_TRUE);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexCoord4d(0, 0, 0, 1);
	glBegin(GL_QUAD_STRIP);
	for (unsigned int i = 0; i <= pts; ++i)
	{
		
		float angle = (i / (float)pts) * 3.14159f * 2.0f;
		glTexCoord2f(0, 1);
		glVertex2f(inner * cos(angle), inner * sin(angle));
		glTexCoord2f(1, 0);
		glVertex2f(outer * cos(angle), outer * sin(angle));
	}
	glEnd();
}


GLuint loadBMP(const char* imagepath)
{
	GLuint texture;

	int width, height;

	unsigned char* data;

	FILE* file;

	errno_t err = fopen_s(&file, imagepath, "rb");

	if (file == NULL) return 0;
	width = 124;
	height = 124;
	data = (unsigned char*)malloc(width * height * 3);
	//int size = fseek(file,);
	fread(data, width * height * 3, 1, file);
	fclose(file);

	for (int i = 0; i < width * height; ++i)
	{
		int index = i * 3;
		unsigned char B, R;
		B = data[index];
		R = data[index + 2];

		data[index] = R;
		data[index + 2] = B;

	}


	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);


	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
	free(data);

	return texture;
}

void seeall() {
	gluLookAt(x, y, z,
		x + lx, y + ly, z + lz,
		0.0f, 1.0f, 0.0f);
}

void update(int value/*, int value1, int value2 */) {
	rAngle += (1.0f + speed);
	

	glutPostRedisplay(); // Inform GLUT that the display has changed

	glutTimerFunc(10, update, 0);//Call update after each 25 millisecond

}

	void MyDisplay(void) {
	// The new scene

	if (deltaMove)
		computePos(deltaMove);

	GLfloat fPozycja_swiatla[] = { 0.0f,0.0f,20.0f,1.0f };
	GLfloat fSlabe_swiatlo[] = { 0.25f,0.25f,0.25f,1.0f };
	GLfloat fMocne_swiatlo[] = { 1.0f,1.0f,1.0f,1.0f };
	

	glLoadIdentity();
	glTranslated(0.0f, 0.0f, -400.0f);
	glPushMatrix();
	glPushMatrix();
	glPushMatrix();
	glPushMatrix();
	glPushMatrix();
	glPushMatrix();
	glPushMatrix();
	glPushMatrix();
	glPushMatrix();
	glPushMatrix();
	

	

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
	glLightfv(GL_LIGHT0, GL_AMBIENT, fSlabe_swiatlo);//œwiat³ootoczenia
	glLightfv(GL_LIGHT0, GL_DIFFUSE, fMocne_swiatlo);//œwiat³orozproszenia
	glLightfv(GL_LIGHT0, GL_SPECULAR, fMocne_swiatlo);//œwiat³oodbite
	glLightfv(GL_LIGHT0, GL_POSITION, fPozycja_swiatla);//pozycjaœwiat³a


	//SUN
	/*float red[] =  { 0.5F, 0, 0, 1};
	float reda[] = { 0.1F, 0, 0, 1};
	glLightfv(GL_LIGHT1, GL_AMBIENT, reda);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, red);
	glLightfv(GL_LIGHT1, GL_SPECULAR, red);*/
	seeall();
	glRotatef(rAngle, 0.0f, 0.0f, 1.0f);
	PrintSphere(15.0, loadBMP("8k_sun.bmp"));
	glPopMatrix();

	//MERCURY 0.240846 yr
	seeall();
	glRotatef(rAngle*4, 0.0f, 0.0f, 1.0f);
	glTranslated(30.0f, 0.0f, 0.0f);
	glRotatef(rAngle, 0.0f, 0.0f, 1.0f);
	PrintSphere(2.0, loadBMP("8k_mercury.bmp"));
	glPopMatrix();

	//VENUS 0.615198 yr
	seeall();
	glRotatef(rAngle*2, 0.0f, 0.0f, 1.0f);
	glTranslated(60.0f, 0.0f, 0.0f);
	glRotatef(rAngle, 0.0f, 0.0f, 1.0f);
	PrintSphere(2.0, loadBMP("8k_venus_surface.bmp"));
	glPopMatrix();

	//EARTH
	seeall();
	glRotatef(rAngle, 0.0f, 0.0f, 1.0f);
	glTranslated(90.0f, 0.0f, 0.0f);
	glRotatef(rAngle, 0.0f, 0.0f, 1.0f);
	PrintSphere(3.0, loadBMP("8k_earth_daymap.bmp"));
	//MOON
	glRotatef(rAngle*4, 0.0f, 0.0f, 1.0f);
	glTranslated(4.5f, 0.0f, 0.0f);
	glRotatef(rAngle, 0.0f, 0.0f, 1.0f);
	PrintSphere(1.0, loadBMP("8k_moon.bmp"));
	glPopMatrix();

	//MARS 686.971 d
	seeall();
	glRotatef(rAngle/1.5, 0.0f, 0.0f, 1.0f);
	glTranslated(120.0f, 0.0f, 0.0f);
	glRotatef(rAngle, 0.0f, 0.0f, 1.0f);
	PrintSphere(3.0, loadBMP("8k_mars.bmp"));
	glPopMatrix();

	//JUPITER 4,332.59 d
	seeall();
	glRotatef(rAngle/2, 0.0f, 0.0f, 1.0f);
	glTranslated(150.0f, 0.0f, 0.0f);
	glRotatef(rAngle, 0.0f, 0.0f, 1.0f);
	PrintSphere(7.0, loadBMP("8k_jupiter.bmp"));
	glPopMatrix();
	
	//SATURN 10,759.22 d
	seeall();
	glRotatef(rAngle/2.5, 0.0f, 0.0f, 1.0f);
	glTranslated(180.0f, 0.0f, 0.0f);
	glRotatef(rAngle, 0.0f, 0.0f, 1.0f);
	PrintSphere(7.0, loadBMP("8k_saturn.bmp"));
	//SATURN RINGS
	Torus2d(8.0, 12.0, 40, loadBMP("8k_saturn_ring_alpha.bmp"));
	glPopMatrix();
	
	//URANUS 30,688.5 d[3]
	seeall();
	glRotatef(rAngle/3, 0.0f, 0.0f, 1.0f);
	glTranslated(210.0f, 0.0f, 0.0f);
	glRotatef(rAngle, 0.0f, 0.0f, 1.0f);
	PrintSphere(5.0, loadBMP("2k_uranus.bmp"));
	glPopMatrix();

	//NEPTUNE 60,182 days
	seeall();
	glRotatef(rAngle/3.5, 0.0f, 0.0f, 1.0f);
	glTranslated(240.0f, 0.0f, 0.0f);
	glRotatef(rAngle, 0.0f, 0.0f, 1.0f);
	PrintSphere(5.0, loadBMP("2k_neptune.bmp"));
	glPopMatrix();

	/*
	//PLUTO
	seeall();
	glTranslated(270.0f, 0.0f, 0.0f);
	glRotatef(rAngle, 0.0f, 0.0f, 1.0f);
	PrintSphere(1.0, loadBMP("4k_eris_fictional.bmp"));
	glPopMatrix();
	*/

	
	glutSwapBuffers();
	glFlush();
}

void processNormalKeys(unsigned char key, int xx, int yy) {

	if (key == 27)
		exit(0);
}

void pressKey(int key, int xx, int yy) {

	switch (key) {
	case GLUT_KEY_LEFT: speed += -0.5f; break;
	case GLUT_KEY_RIGHT: speed += 0.5f; break;
	case GLUT_KEY_UP: deltaMove = 20.0f; break;
	case GLUT_KEY_DOWN: deltaMove = -20.0f; break;
	}
}

void releaseKey(int key, int x, int y) {

	switch (key) {
	case GLUT_KEY_LEFT:
	case GLUT_KEY_RIGHT: deltaMovex = 0;break;
	case GLUT_KEY_UP:
	case GLUT_KEY_DOWN: deltaMove = 0;break;
	}
}

void mouseMove(int x, int y) {

	// this will only be true when the left button is down
	if (xOrigin >= 0) {

		// update deltaAngle
		deltaAnglex = (x - xOrigin) * 0.001f;
		deltaAngley = (y - yOrigin) * 0.001f;

		// update camera's direction
		lx = sin(angle + deltaAnglex);
		lz = -cos(angle + deltaAnglex);
		ly = sin(angley + deltaAngley);
	}
}

void mouseButton(int button, int state, int x, int y) {

	// only start motion if the left button is pressed
	if (button == GLUT_LEFT_BUTTON) {

		// when the button is released
		if (state == GLUT_UP) {
			angle += deltaAnglex;
			xOrigin = -10;
			angley += deltaAngley;
			yOrigin = -10;
		}
		else {// state = GLUT_DOWN
			xOrigin = x;
			yOrigin = y;
		}
	}
}


void MyInit(void) {
	glClearColor(0.0, 0.0, 0.0, 0.0);//select clearing (background) color
									 /* initialize viewing values */
	glClearDepth(1.0f);



	//glViewport(0, 0, 1000, 1000);//window origin and size

	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	


	glutReshapeFunc(changeSize);
	/*
	glMatrixMode(GL_PROJECTION);//
	glLoadIdentity();//=1
	glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();//=1
	*/
}

/*void SpecialInput(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		global_y += 1;
		break;
	case GLUT_KEY_DOWN:
		global_y -= 1;
		break;
	case GLUT_KEY_LEFT:
		global_x -= 1;
		break;
	case GLUT_KEY_RIGHT:
		global_x += 1;
		break;
	case GLUT_KEY_PAGE_UP:
		global_z += 0.1;
		break;
	case GLUT_KEY_PAGE_DOWN:
		global_z -= 0.1;
		break;
	}
	glutPostRedisplay();
}
*/


int main(int argc, char** argv) { //<- for normal API

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);//single buffer and RGBA
	glutInitWindowPosition(120, 100);
	glutInitWindowSize(1700, 900);
	glutCreateWindow("ITS ALIVE");//create widnow, hello title bar
	glutDisplayFunc(MyDisplay);//
	glutTimerFunc(10, update, 0);
	
	//glutSpecialFunc(SpecialInput);
	MyInit();
	glutIdleFunc(MyDisplay);

	glutIgnoreKeyRepeat(1);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(pressKey);
	glutSpecialUpFunc(releaseKey);

	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);

	glutMainLoop();//enter main loop and process events
	return 0;
}
