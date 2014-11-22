#include <windows.h>
#include <gl/glut.h>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

#include "imageloader.h"

// Nasconde la console
#pragma comment(linker,"/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

using namespace std;

typedef struct{
	float x, y,z;
} Point;

typedef struct{
	Point posizione;
	char colore;
} Cubo;

typedef struct{
	int numeroVisuale;
	bool riga_colonna; //se true muove riga altrimenti colonna
	int valore_riga_colonna; //si intende quale riga o colonna vuole muovere.
} Mossa;

vector<Cubo> cuboRubik;
Cubo cuboMultiuso;
Point puntoMultiuso;
Mossa mossaCorrente;
bool mossaAttiva; //true se l'utente ha richiesto una mossa
vector<Mossa> mosseEffettuate;
int scattoRotazione = 90;

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

void inizializzaCubo(){

	puntoMultiuso.x = -1.0;
	puntoMultiuso.y = -1.0;
	puntoMultiuso.z = -1.0;
	cuboMultiuso.posizione = puntoMultiuso;
	cuboMultiuso.colore = 'G';
	cuboRubik.push_back(cuboMultiuso);

	puntoMultiuso.z = 0.0;
	cuboMultiuso.posizione = puntoMultiuso;
	cuboRubik.push_back(cuboMultiuso);

	puntoMultiuso.z = 1.0;
	cuboMultiuso.posizione = puntoMultiuso;
	cuboRubik.push_back(cuboMultiuso);

	puntoMultiuso.y = 0.0;
	puntoMultiuso.z = -1.0;
	cuboMultiuso.posizione = puntoMultiuso;
	cuboRubik.push_back(cuboMultiuso);

	puntoMultiuso.z = 0.0;
	cuboMultiuso.posizione = puntoMultiuso;
	cuboRubik.push_back(cuboMultiuso);

	puntoMultiuso.z = 1.0;
	cuboMultiuso.posizione = puntoMultiuso;
	cuboRubik.push_back(cuboMultiuso);

	puntoMultiuso.y = 1.0;
	puntoMultiuso.z = -1.0;
	cuboMultiuso.posizione = puntoMultiuso;
	cuboRubik.push_back(cuboMultiuso);

	puntoMultiuso.z = 0.0;
	cuboMultiuso.posizione = puntoMultiuso;
	cuboRubik.push_back(cuboMultiuso);

	puntoMultiuso.z = 1.0;
	cuboMultiuso.posizione = puntoMultiuso;
	cuboRubik.push_back(cuboMultiuso);

	//linea centrale
	puntoMultiuso.x = 0.0;
	puntoMultiuso.y = -1.0;
	puntoMultiuso.z = -1.0;
	cuboMultiuso.posizione = puntoMultiuso;
	cuboMultiuso.colore = 'G';
	cuboRubik.push_back(cuboMultiuso);

	puntoMultiuso.z = 0.0;
	cuboMultiuso.posizione = puntoMultiuso;
	cuboRubik.push_back(cuboMultiuso);

	puntoMultiuso.z = 1.0;
	cuboMultiuso.posizione = puntoMultiuso;
	cuboRubik.push_back(cuboMultiuso);

	puntoMultiuso.y = 0.0;
	puntoMultiuso.z = -1.0;
	cuboMultiuso.posizione = puntoMultiuso;
	cuboRubik.push_back(cuboMultiuso);

	puntoMultiuso.z = 0.0;
	cuboMultiuso.posizione = puntoMultiuso;
	cuboRubik.push_back(cuboMultiuso);

	puntoMultiuso.z = 1.0;
	cuboMultiuso.posizione = puntoMultiuso;
	cuboRubik.push_back(cuboMultiuso);

	puntoMultiuso.y = 1.0;
	puntoMultiuso.z = -1.0;
	cuboMultiuso.posizione = puntoMultiuso;
	cuboRubik.push_back(cuboMultiuso);

	puntoMultiuso.z = 0.0;
	cuboMultiuso.posizione = puntoMultiuso;
	cuboRubik.push_back(cuboMultiuso);

	puntoMultiuso.z = 1.0;
	cuboMultiuso.posizione = puntoMultiuso;
	cuboRubik.push_back(cuboMultiuso);


	//linea destra
	puntoMultiuso.x = 1.0;
	puntoMultiuso.y = -1.0;
	puntoMultiuso.z = -1.0;
	cuboMultiuso.posizione = puntoMultiuso;
	cuboMultiuso.colore = 'G';
	cuboRubik.push_back(cuboMultiuso);

	puntoMultiuso.z = 0.0;
	cuboMultiuso.posizione = puntoMultiuso;
	cuboRubik.push_back(cuboMultiuso);

	puntoMultiuso.z = 1.0;
	cuboMultiuso.posizione = puntoMultiuso;
	cuboRubik.push_back(cuboMultiuso);

	puntoMultiuso.y = 0.0;
	puntoMultiuso.z = -1.0;
	cuboMultiuso.posizione = puntoMultiuso;
	cuboRubik.push_back(cuboMultiuso);

	puntoMultiuso.z = 0.0;
	cuboMultiuso.posizione = puntoMultiuso;
	cuboRubik.push_back(cuboMultiuso);

	puntoMultiuso.z = 1.0;
	cuboMultiuso.posizione = puntoMultiuso;
	cuboRubik.push_back(cuboMultiuso);

	puntoMultiuso.y = 1.0;
	puntoMultiuso.z = -1.0;
	cuboMultiuso.posizione = puntoMultiuso;
	cuboRubik.push_back(cuboMultiuso);

	puntoMultiuso.z = 0.0;
	cuboMultiuso.posizione = puntoMultiuso;
	cuboRubik.push_back(cuboMultiuso);

	puntoMultiuso.z = 1.0;
	cuboMultiuso.posizione = puntoMultiuso;
	cuboRubik.push_back(cuboMultiuso);

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


void reshape(GLsizei width, GLsizei height) // GLsizei for non-negative integer
{ 
	// Compute aspect ratio of the new window
	if (height == 0) height = 1; // To prevent divide by 0
	GLfloat aspect = (GLfloat)width / (GLfloat)height;
	// Set the viewport to cover the new window
	glViewport(0, 0, width, height);
	// Set the aspect ratio of the clipping volume to match the viewport
	glMatrixMode(GL_PROJECTION); // To operate on the Projection matrix
	glLoadIdentity(); // Reset
	// Enable perspective projection with fovy, aspect, zNear and zFar
	gluPerspective(60.0f, aspect, 1.0f, 1000.0f);
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
	
	/*gluLookAt(2.0f,-5.0f, -10.0f, 
		0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f);*/

	/*gluLookAt(0.0f,12.0f, 4.0f, 
		0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f);*/

	//faccia gialla avanti
	/*gluLookAt(4.0f, 5.0f, -8.0f, 
		0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f);*/

	//faccia verde davanti
	/*gluLookAt(-8.0f, 5.0f, -2.0f, 
		0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f);*/

	//(faccia blu davanti)
	/*gluLookAt(10.0f,5.0f, 4.0f,
		0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f);*/

	//Questa puo andare bene come posizione iniziale (faccia bianca davanti)
	gluLookAt(-2.0f,5.0f, 10.0f, 
		0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f);

	//quella presente nella vecchia resize
	/*gluLookAt(0.0f,5.5f, 15.0f,
		0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f);*/

	drawGround();

	//glTranslatef(0,-5.0,-10.0);
	glTranslatef(0, 0.0, 0.0);
	//glRotatef(-angolo_rotazione, 2.0f, 2.0f, 0.0f);
	//glRotatef(45.0, 1.0, 0.0, 0.0);
	//glRotatef(45.0, 0.0, 1.0, 0.0);

	glRotatef(arrowXrot, 1.0, 0.0, 0.0);
	glRotatef(arrowYrot, 0.0, 1.0, 0.0);
	glRotatef(arrowZrot, 0.0, 0.0, 0.0);

	glutWireCube(4.0);

	//oscilla();
	//cuboSingolo();

	/*for(int x = -1.0;x <= DIMENSIONE_FACCIA*2-1; x += DIMENSIONE_FACCIA)
	{
		for(int y = -1.0;y <= DIMENSIONE_FACCIA*2-1; y += DIMENSIONE_FACCIA)
		{
			for(int z = -1.0; z <= DIMENSIONE_FACCIA*2-1; z += DIMENSIONE_FACCIA)
			{
				glPushMatrix();
				glTranslatef(x,y,z);
				glCallList(_displayListId_smallcube);
				glPopMatrix();
			}
		}
	}*/
	
	
	for (int i = 0; i < cuboRubik.size(); i++){
		
		glPushMatrix();
		if (cuboRubik[i].posizione.x == 0){
			glRotatef(angolo_rotazione, 1.0f, 0.0f, 0.0f);
		}
		glTranslatef(cuboRubik[i].posizione.x,cuboRubik[i].posizione.y,cuboRubik[i].posizione.z);
		glCallList(_displayListId_smallcube);
		glPopMatrix();

	}

	glutSwapBuffers();

}

void timer(int value) {
	if (angolo_rotazione != 90) {
		angolo_rotazione += 1.0;
	}
	glutPostRedisplay();
	glutTimerFunc(10, timer, 0);
}

void createGluiPanel()
{
//	GLUI* glui = GLUI_Master.create_glui("Opzioni", GLUI_SUBWINDOW_TOP, (glutGet(GLUT_SCREEN_WIDTH)-larghezza-400)/2, (glutGet(GLUT_SCREEN_HEIGHT)-altezza)/2);
//	pannello_colore_sfondo = glui -> add_panel("Gestione colore sfondo", GLUI_PANEL_EMBOSSED);
//	rs = glui -> add_edittext_to_panel(pannello_colore_sfondo, "rosso:", GLUI_EDITTEXT_FLOAT, &rosso_sf);
//	gs = glui -> add_edittext_to_panel(pannello_colore_sfondo, "verde:", GLUI_EDITTEXT_FLOAT, &verde_sf);
//	bs = glui -> add_edittext_to_panel(pannello_colore_sfondo, "blu:", GLUI_EDITTEXT_FLOAT, &blu_sf);
//
//	rs -> set_int_limits(0, 255);
//	gs -> set_int_limits(0, 255);
//	bs -> set_int_limits(0, 255);
//
//	pannello_velocità_palla = glui -> add_panel("Gestione velocita palla", GLUI_PANEL_EMBOSSED);
//	incrementa_palla = glui -> add_button_to_panel(pannello_velocità_palla, "+", 1, manageButton);
//	decrementa_palla = glui -> add_button_to_panel(pannello_velocità_palla, "-", 2, manageButton);
//
//	pannello_velocità_racchette = glui -> add_panel("Gestione velocita racchette", GLUI_PANEL_EMBOSSED);
//	incrementa_racchetta = glui -> add_button_to_panel(pannello_velocità_racchette, "+", 3, manageButton);
//	decrementa_racchetta = glui -> add_button_to_panel(pannello_velocità_racchette, "-", 4, manageButton);
//
//	glui -> set_main_gfx_window(winID);
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
	glutReshapeFunc(reshape);
	glutTimerFunc(17, timer, 0);
	glutSpecialFunc(specialKeyboard);

	//creaPannelloGlui();
	
	glutMainLoop();
}