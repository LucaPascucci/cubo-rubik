#include <windows.h>
#include <gl/glut.h>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cmath>

#include "imageloader.h"

// Nasconde la console
#pragma comment(linker,"/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

using namespace std;

int larghezza,altezza,windowsID;
int window_x,window_y;

float angolo_rotazione = 0;
const float DIMENSIONE_FACCIA = 1.0;

GLuint white_textureId;
GLuint red_textureId;
GLuint blue_textureId;
GLuint green_textureId;
GLuint yellow_textureId;
GLuint orange_textureId;
GLuint _displayListId_smallcube;

GLfloat arrowXrot = 0.0;
GLfloat arrowYrot = 0.0;
GLfloat arrowZrot = 0.0;

//Converte l'immagine in una texture e ritorna l'id della texture
GLuint loadTexture(Image* image) {
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGB,
		image->width, image->height,
		0,
		GL_RGB,
		GL_UNSIGNED_BYTE,
		image->pixels);
	return textureId;
}

void centraFinestraDesktop()
{
	window_x = (glutGet (GLUT_SCREEN_WIDTH) - larghezza)/2;
	window_y = (glutGet (GLUT_SCREEN_HEIGHT) - altezza)/2;
}

void keyboard(unsigned char key,int x,int y)
{
	switch(key)
	{
	case 27: //esc
		glutDestroyWindow ( windowsID );
		exit(0);
		break;
	}
}

void specialKeyboard(int key, int x, int y)
{
	if(key == GLUT_KEY_UP)
	{
		arrowXrot-=3;
		if(arrowXrot < -360.0)
			arrowXrot=0;
	}

	if(key == GLUT_KEY_DOWN)
	{
		arrowXrot += 3.0;
		if(arrowXrot > 360.0)
			arrowXrot=0;
	}

	/*if(key == GLUT_KEY_LEFT)
	{
		arrowZrot+=3.0;
		if(arrowZrot > 360.0)
			arrowZrot=0;
	}

	if(key == GLUT_KEY_RIGHT)
	{
		arrowZrot-=3;
		if(arrowZrot < -360.0)
			arrowZrot=0;
	}*/

	if(key == GLUT_KEY_LEFT)
	{
		arrowYrot-=3;
		if(arrowYrot < -360.0)
			arrowYrot=0;
	}

	if(key == GLUT_KEY_RIGHT)
	{
		arrowYrot+=3;
		if(arrowYrot>360.0)
			arrowYrot=0;
	}

	/* Aggiorna l'immagine */
	glutPostRedisplay();
}

void oscilla()
{
	GLfloat offset = 0.1;
	GLfloat maxAngleSize = 30.0;
	static GLfloat angle = 90.0;
	static GLfloat angleOld;
	GLfloat zRot = sin(angle)*3;
	GLfloat xRot = cos(angle)*3;

	angleOld = angle + 10.0;

	glRotatef(zRot, 0.0, 0.0, 1.0);
	glRotatef(xRot, 0.0, 1.0, 0.0);

	/* imposta un leggero movimento oscillatorio di default */
	if(angleOld != angle && angle > -maxAngleSize)
	{
		angle-=offset;
	}
	else if(angle <= maxAngleSize)
	{
		angle+=offset;
		angleOld=angle;
	}
	else if(angleOld == angle && angle < maxAngleSize)
	{
		angle+=offset; 
		angleOld+=offset;
	}
	else{
		angleOld=angle+10.0;
	}
}


void resize(int w,int h)
{
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0,(double)w/(double)h,1.0,200.0);
	gluLookAt(0.0f,5.5f, 15.0f,
		0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f);
}

void drawGround(void)
{ 
	glPushMatrix();
	glBegin(GL_QUADS);
	glColor3ub(0, 0, 0);
	glVertex3f(600.0f, -50.0f, -400.0f);
	glVertex3f(-600.0f, -50.0f, -400.0f);
	glColor3ub(200, 200, 200);
	glVertex3f(-600.0f, -50.0f, -20.0f);
	glVertex3f(600.0f, -50.0f, -20.0f);
	glEnd();
	glPopMatrix();
}

void cuboSingolo()
{

	Image* image = loadBMP("white.bmp");
	white_textureId = loadTexture(image);
	delete image;

	Image* image1 = loadBMP("red.bmp");
	red_textureId = loadTexture(image1);
	delete image1;

	Image* image2 = loadBMP("blue.bmp");
	blue_textureId = loadTexture(image2);
	delete image2;

	Image* image3 = loadBMP("orange.bmp");
	orange_textureId = loadTexture(image3);
	delete image3;

	Image* image4 = loadBMP("green.bmp");
	green_textureId = loadTexture(image4);
	delete image4;

	Image* image5 = loadBMP("yellow.bmp");
	yellow_textureId = loadTexture(image5);
	delete image5;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, orange_textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBegin(GL_QUADS);
	//Faccia Superiore

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-DIMENSIONE_FACCIA / 2, DIMENSIONE_FACCIA / 2, -DIMENSIONE_FACCIA / 2);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-DIMENSIONE_FACCIA / 2, DIMENSIONE_FACCIA / 2, DIMENSIONE_FACCIA / 2);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(DIMENSIONE_FACCIA / 2, DIMENSIONE_FACCIA / 2, DIMENSIONE_FACCIA / 2);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(DIMENSIONE_FACCIA / 2, DIMENSIONE_FACCIA / 2, -DIMENSIONE_FACCIA / 2);

	glEnd();
	glDisable(GL_TEXTURE_2D);

	//cambio texture
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, red_textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBegin(GL_QUADS);
	//Faccia Inferiore

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-DIMENSIONE_FACCIA / 2, -DIMENSIONE_FACCIA / 2, -DIMENSIONE_FACCIA / 2);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(DIMENSIONE_FACCIA / 2, -DIMENSIONE_FACCIA / 2, -DIMENSIONE_FACCIA / 2);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(DIMENSIONE_FACCIA / 2, -DIMENSIONE_FACCIA / 2, DIMENSIONE_FACCIA / 2);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-DIMENSIONE_FACCIA / 2, -DIMENSIONE_FACCIA / 2, DIMENSIONE_FACCIA / 2);

	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, green_textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBegin(GL_QUADS);
	//Faccia Sinistra

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-DIMENSIONE_FACCIA / 2, -DIMENSIONE_FACCIA / 2, -DIMENSIONE_FACCIA / 2);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-DIMENSIONE_FACCIA / 2, -DIMENSIONE_FACCIA / 2, DIMENSIONE_FACCIA / 2);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-DIMENSIONE_FACCIA / 2, DIMENSIONE_FACCIA / 2, DIMENSIONE_FACCIA / 2);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-DIMENSIONE_FACCIA / 2, DIMENSIONE_FACCIA / 2, -DIMENSIONE_FACCIA / 2);

	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, blue_textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBegin(GL_QUADS);
	//Faccia Destra

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(DIMENSIONE_FACCIA / 2, -DIMENSIONE_FACCIA / 2, -DIMENSIONE_FACCIA / 2);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(DIMENSIONE_FACCIA / 2, DIMENSIONE_FACCIA / 2, -DIMENSIONE_FACCIA / 2);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(DIMENSIONE_FACCIA / 2, DIMENSIONE_FACCIA / 2, DIMENSIONE_FACCIA / 2);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(DIMENSIONE_FACCIA / 2, -DIMENSIONE_FACCIA / 2, DIMENSIONE_FACCIA / 2);

	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, white_textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBegin(GL_QUADS);
	//Faccia Anteriore

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-DIMENSIONE_FACCIA / 2, -DIMENSIONE_FACCIA / 2, DIMENSIONE_FACCIA / 2);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(DIMENSIONE_FACCIA / 2, -DIMENSIONE_FACCIA / 2, DIMENSIONE_FACCIA / 2);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(DIMENSIONE_FACCIA / 2, DIMENSIONE_FACCIA / 2, DIMENSIONE_FACCIA / 2);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-DIMENSIONE_FACCIA / 2, DIMENSIONE_FACCIA / 2, DIMENSIONE_FACCIA / 2);

	glEnd();
	glDisable(GL_TEXTURE_2D);


	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, yellow_textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBegin(GL_QUADS);
	//Faccia Posteriore

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-DIMENSIONE_FACCIA / 2, -DIMENSIONE_FACCIA / 2, -DIMENSIONE_FACCIA / 2);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-DIMENSIONE_FACCIA / 2, DIMENSIONE_FACCIA / 2, -DIMENSIONE_FACCIA / 2);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(DIMENSIONE_FACCIA / 2, DIMENSIONE_FACCIA / 2, -DIMENSIONE_FACCIA / 2);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(DIMENSIONE_FACCIA / 2, -DIMENSIONE_FACCIA / 2, -DIMENSIONE_FACCIA / 2);

	glEnd();
	glDisable(GL_TEXTURE_2D);

}

void init()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glEnable(GL_CULL_FACE);
	glShadeModel(GL_SMOOTH);
	/* colore di sfondo nero */
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//Set up a display list for drawing a cube
	_displayListId_smallcube = glGenLists(1); //Make room for the display list
	glNewList(_displayListId_smallcube, GL_COMPILE); //Begin the display list
	cuboSingolo(); //Add commands for drawing a black area to the display list
	glEndList(); //End the display list

}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	drawGround();

	//glTranslatef(0,-5.0,-10.0);
	glTranslatef(0, 0.0, 0.0);
	//glRotatef(-angolo_rotazione, 2.0f, 2.0f, 0.0f);
	//glRotatef(45.0, 1.0, 0.0, 0.0);
	//glRotatef(45.0, 0.0, 1.0, 0.0);

	glRotatef(arrowXrot, 1.0, 0.0, 0.0);
	glRotatef(arrowYrot, 0.0, 1.0, 0.0);
	glRotatef(arrowZrot, 0.0, 0.0, 0.0);

	//oscilla();
	//cuboSingolo();

	for(int x = 0.0;x < DIMENSIONE_FACCIA*3-0.1; x += DIMENSIONE_FACCIA)
	{
		for(int y = 0;y < DIMENSIONE_FACCIA*3-0.1; y += DIMENSIONE_FACCIA)
		{
			for(int z = 0; z < DIMENSIONE_FACCIA*3-0.1; z += DIMENSIONE_FACCIA)
			{
				glPushMatrix();
				glTranslatef(x,y,z);
				glCallList(_displayListId_smallcube);
				glPopMatrix();
			}
		}
	}

	glutSwapBuffers();

}

void timer(int value) {
	angolo_rotazione += 2.0f;
	if (angolo_rotazione > 360) {
		angolo_rotazione -= 360;
	}
	glutPostRedisplay();
	glutTimerFunc(10, timer, 0);
}

void main(int argc,char** argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
	larghezza = 800;
	altezza = 800;
	centraFinestraDesktop();
	glutInitWindowSize(larghezza,altezza);
	glutInitWindowPosition (window_x, window_y);
	windowsID = glutCreateWindow("Cubo Di Rubik");

	init();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(resize);
	glutTimerFunc(17, timer, 0);
	glutSpecialFunc(specialKeyboard);

	glutMainLoop();
}