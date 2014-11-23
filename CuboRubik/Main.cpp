#include <windows.h>
#include <gl/glut.h> 
#include <gl/gl.h> 
#include <gl/glu.h>
#include <gl/glui.h>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

#include "imageloader.h"

// Nasconde la console
//#pragma comment(linker,"/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

using namespace std;

typedef struct{
	float x, y, z;
} Point;

typedef struct{
	Point posizione;
	char colore;
} Cubo;

typedef struct{
	int numeroVisuale;
	char riga_colonna; //c = colonna, r = righa, s = sezione
	int valore_riga_colonna_sezione; //si intende quale riga o colonna vuole muovere.
	bool direzione;
} Mossa;

vector<Cubo> cuboRubik;
Cubo cuboMultiuso;
Point puntoMultiuso;

Mossa mossaInCorso;
bool pussantePremuto; //true se l'utente ha premuto un pussante
vector<Mossa> mosseEffettuate;
int scattoRotazione = 90;
Point puntoRiferimentoRotazione;

//variabili necessarie per il controllo della rotazione del cubo nella sua interezza
int angolo_asse_y = 0;
int angolo_asse_x = 0;
int angolo_asse_z = 0;
Point rotate_x = {1.0, 0.0, 0.0};
Point rotate_y = {0.0, 1.0, 0.0};
Point rotate_z = {0.0, 0.0, 1.0};
bool frecciaPremuta = false;
int valorePrecX = 0;
int valorePrecY = 0;
int valorePrecZ = 0;

int larghezza,altezza,windowsID;
int window_x,window_y;

GLUI* glui;
GLUI_Panel *panel;

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

	//punto centrale che rimane nascosto
	/*puntoMultiuso.z = 0.0;
	cuboMultiuso.posizione = puntoMultiuso;
	cuboRubik.push_back(cuboMultiuso);*/

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
	Mossa mossaAttiva;
	switch(key)
	{
	case 27: //esc
		glutDestroyWindow ( windowsID );
		exit(0);
		break;

	case 'q': // sezione posteriore a sinistra
		if (!pussantePremuto){
			pussantePremuto = !pussantePremuto;
			mossaAttiva.riga_colonna = 's';
			mossaAttiva.valore_riga_colonna_sezione = -1.0;
			mossaAttiva.direzione = false;
			mosseEffettuate.push_back(mossaAttiva);
			mossaInCorso = mossaAttiva;
		}
		break;

	case 'a': //sezione centrale a sinistra
		if (!pussantePremuto){
			pussantePremuto = !pussantePremuto;
			mossaAttiva.riga_colonna = 's';
			mossaAttiva.valore_riga_colonna_sezione = 0.0;
			mossaAttiva.direzione = false;
			mosseEffettuate.push_back(mossaAttiva);
			mossaInCorso = mossaAttiva;
		}
		break;

	case 'z': //sezione frontale a sinistra
		if (!pussantePremuto){
			pussantePremuto = !pussantePremuto;
			mossaAttiva.riga_colonna = 's';
			mossaAttiva.valore_riga_colonna_sezione = 1.0;
			mossaAttiva.direzione = false;
			mosseEffettuate.push_back(mossaAttiva);
			mossaInCorso = mossaAttiva;
		}
		break;

	case 'w': //sezione posteriore a destra
		if (!pussantePremuto){
			pussantePremuto = !pussantePremuto;
			mossaAttiva.riga_colonna = 's';
			mossaAttiva.valore_riga_colonna_sezione = -1.0;
			mossaAttiva.direzione = true;
			mosseEffettuate.push_back(mossaAttiva);
			mossaInCorso = mossaAttiva;
		}
		break;

	case 's': //sezione centrale a destra
		if (!pussantePremuto){
			pussantePremuto = !pussantePremuto;
			mossaAttiva.riga_colonna = 's';
			mossaAttiva.valore_riga_colonna_sezione = 0.0;
			mossaAttiva.direzione = true;
			mosseEffettuate.push_back(mossaAttiva);
			mossaInCorso = mossaAttiva;
		}
		break;

	case 'x': //sezione frontale a destra
		if (!pussantePremuto){
			pussantePremuto = !pussantePremuto;
			mossaAttiva.riga_colonna = 's';
			mossaAttiva.valore_riga_colonna_sezione = 1.0;
			mossaAttiva.direzione = true;
			mosseEffettuate.push_back(mossaAttiva);
			mossaInCorso = mossaAttiva;
		}
		break;
	}
}

void specialKeyboard(int key, int x, int y)
{
	if (!frecciaPremuta)
	{
		frecciaPremuta = true;
		if(key == GLUT_KEY_UP)
		{
			if (angolo_asse_y == 90 || angolo_asse_y == -90 || angolo_asse_y == 270 || angolo_asse_y == -270)
			{
				angolo_asse_z -= 90;
				if (angolo_asse_z < -360) {
					angolo_asse_z = 0;
				}
			} else {
				angolo_asse_x -= 90;
				if (angolo_asse_x < -360) {
					angolo_asse_x = 0;
				}
			}
			cout << "Il valore di x e' : " << angolo_asse_x << endl;
			cout << "Il valore di y e' : " << angolo_asse_y << endl;
			cout << "Il valore di z e' : " << angolo_asse_z << endl;
		}

		if(key == GLUT_KEY_DOWN)
		{
			if (angolo_asse_y == 90 || angolo_asse_y == -90 || angolo_asse_y == 270 || angolo_asse_y == -270)
			{
				angolo_asse_z += 90;
				if (angolo_asse_z < -360) {
					angolo_asse_z = 0;
				}
			} else {
				angolo_asse_x += 90;
				if (angolo_asse_x > 360) {
					angolo_asse_x = 0;
				}
			}
			cout << "Il valore di x e' : " << angolo_asse_x << endl;
			cout << "Il valore di y e' : " << angolo_asse_y << endl;
			cout << "Il valore di z e' : " << angolo_asse_z << endl;
		}

		if(key == GLUT_KEY_LEFT)
		{
			if (angolo_asse_x == 90 || angolo_asse_x == -90 || angolo_asse_x == 270 || angolo_asse_x == -270)
			{
				angolo_asse_z -= 90;
				if (angolo_asse_z < -360) {
					angolo_asse_z = 0;
				}
			} else {
				angolo_asse_y += 90;
				if (angolo_asse_y > 360) {
					angolo_asse_y = 0;
				}
			}
			cout << "Il valore di x e' : " << angolo_asse_x << endl;
			cout << "Il valore di y e' : " << angolo_asse_y << endl;
			cout << "Il valore di z e' : " << angolo_asse_z << endl;
		}

		if(key == GLUT_KEY_RIGHT)
		{
			if (angolo_asse_x == 90 || angolo_asse_x == -90 || angolo_asse_x == 270 || angolo_asse_x == -270) {
				angolo_asse_z += 90;
				if (angolo_asse_z > 360) {
					angolo_asse_z = 0;
				}
			} else {
				angolo_asse_y -= 90;
				if (angolo_asse_y < -360) {
					angolo_asse_y = 0;
				}
			}
			cout << "Il valore di x e' : " << angolo_asse_x << endl;
			cout << "Il valore di y e' : " << angolo_asse_y << endl;
			cout << "Il valore di z e' : " << angolo_asse_z << endl;
		}

		glutPostRedisplay();
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

void gestioneBottoni(int opzione){
	Mossa mossaAttiva;
	if (!pussantePremuto){
		pussantePremuto = !pussantePremuto;
		switch (opzione)
		{
		case 0: //premuto quit
			glutDestroyWindow(windowsID);
			exit(0);
			break;

		case -1:	//Prima riga a sinistra
			mossaAttiva.riga_colonna = 'r';
			mossaAttiva.valore_riga_colonna_sezione = 1.0;
			mossaAttiva.direzione = false;
			mosseEffettuate.push_back(mossaAttiva);
			mossaInCorso = mossaAttiva;
			break;

		case -2:	//Seconda riga a sinistra
			mossaAttiva.riga_colonna = 'r';
			mossaAttiva.valore_riga_colonna_sezione = 0.0;
			mossaAttiva.direzione = false;
			mosseEffettuate.push_back(mossaAttiva);
			mossaInCorso = mossaAttiva;
			break;

		case -3:	//Terza riga a sinistra
			mossaAttiva.riga_colonna = 'r';
			mossaAttiva.valore_riga_colonna_sezione = -1.0;
			mossaAttiva.direzione = false;
			mosseEffettuate.push_back(mossaAttiva);
			mossaInCorso = mossaAttiva;
			break;

		case 1:		//Prima riga a destra
			mossaAttiva.riga_colonna = 'r';
			mossaAttiva.valore_riga_colonna_sezione = 1.0;
			mossaAttiva.direzione = 'r';
			mosseEffettuate.push_back(mossaAttiva);
			mossaInCorso = mossaAttiva;
			break;

		case 2:		//Seconda riga a destra
			mossaAttiva.riga_colonna = 'r';
			mossaAttiva.valore_riga_colonna_sezione = 0.0;
			mossaAttiva.direzione = true;
			mosseEffettuate.push_back(mossaAttiva);
			mossaInCorso = mossaAttiva;
			break;

		case 3:		//Terza riga a destra 
			mossaAttiva.riga_colonna = 'r';
			mossaAttiva.valore_riga_colonna_sezione = -1.0;
			mossaAttiva.direzione = true;
			mosseEffettuate.push_back(mossaAttiva);
			mossaInCorso = mossaAttiva;
			break;

		case -4:	//Prima colonna sù
			mossaAttiva.riga_colonna = 'c',
				mossaAttiva.valore_riga_colonna_sezione = -1.0;
			mossaAttiva.direzione = false;
			mosseEffettuate.push_back(mossaAttiva);
			mossaInCorso = mossaAttiva;
			break;

		case -5:	//Seconda colonna sù
			mossaAttiva.riga_colonna = 'c';
			mossaAttiva.valore_riga_colonna_sezione = 0.0;
			mossaAttiva.direzione = false;
			mosseEffettuate.push_back(mossaAttiva);
			mossaInCorso = mossaAttiva;
			break;

		case -6:	//Terza colonna sù
			mossaAttiva.riga_colonna = 'c';
			mossaAttiva.valore_riga_colonna_sezione = 1.0;
			mossaAttiva.direzione = false;
			mosseEffettuate.push_back(mossaAttiva);
			mossaInCorso = mossaAttiva;
			break;

		case 4:		//Prima colonna giù
			mossaAttiva.riga_colonna = 'c';
			mossaAttiva.valore_riga_colonna_sezione = -1.0;
			mossaAttiva.direzione = true;
			mosseEffettuate.push_back(mossaAttiva);
			mossaInCorso = mossaAttiva;
			break;

		case 5:		//Seconda colonna giù
			mossaAttiva.riga_colonna = 'c';
			mossaAttiva.valore_riga_colonna_sezione = 0.0;
			mossaAttiva.direzione = true;
			mosseEffettuate.push_back(mossaAttiva);
			mossaInCorso = mossaAttiva;
			break;

		case 6:		//Terza colonna giù
			mossaAttiva.riga_colonna = 'c';
			mossaAttiva.valore_riga_colonna_sezione = 1.0;
			mossaAttiva.direzione = true;
			mosseEffettuate.push_back(mossaAttiva);
			mossaInCorso = mossaAttiva;
			break;

		case 7:		//Shuffle
			break;

		case 8:		//Reset
			break;
		}
	}
}

bool attivazioneMossa(Mossa mossaCorrente,Cubo cuboCorrente){
	bool attivaMovimento = false;
	switch (mossaCorrente.riga_colonna)
	{
	case 'r':
		puntoRiferimentoRotazione.x = 0.0;
		puntoRiferimentoRotazione.z = 0.0;
		if (mossaCorrente.direzione){
			puntoRiferimentoRotazione.y = 1.0;
		}else {
			puntoRiferimentoRotazione.y = -1.0;
		}
		if (cuboCorrente.posizione.y == mossaCorrente.valore_riga_colonna_sezione){
			attivaMovimento = true;
		}
		break;
	case 'c':
		puntoRiferimentoRotazione.y = 0.0;
		puntoRiferimentoRotazione.z = 0.0;
		if (mossaCorrente.direzione){
			puntoRiferimentoRotazione.x = -1.0;
		}else {
			puntoRiferimentoRotazione.x = 1.0;
		}
		if (cuboCorrente.posizione.x == mossaCorrente.valore_riga_colonna_sezione){
			attivaMovimento = true;
		}
		break;

	case 's':
		puntoRiferimentoRotazione.y = 0.0;
		puntoRiferimentoRotazione.x = 0.0;
		if (mossaCorrente.direzione){
			puntoRiferimentoRotazione.z = -1.0;
		}else {
			puntoRiferimentoRotazione.z = 1.0;
		}
		if (cuboCorrente.posizione.z == mossaCorrente.valore_riga_colonna_sezione){
			attivaMovimento = true;
		}
		break;
	}
	return attivaMovimento;

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

	gluLookAt(-4.0f, 5.0f, 8.0f, 
		0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f);

	drawGround();

	//per far ruotare il cubo nella sua interezza
	glRotatef(valorePrecX, rotate_x.x, rotate_x.y, rotate_x.z);
	glRotatef(valorePrecY, rotate_y.x, rotate_y.y, rotate_y.z); 
	glRotatef(valorePrecZ, rotate_z.x, rotate_z.y, rotate_z.z);

	glutWireCube(4.0);

	for (int i = 0; i < cuboRubik.size(); i++){

		glPushMatrix();

		if (pussantePremuto){
			if (!mosseEffettuate.empty()){
				for (int x = 0;x < mosseEffettuate.size()-1;x++){
					if (attivazioneMossa(mosseEffettuate[x],cuboRubik[i])){
						glRotatef(90.0, puntoRiferimentoRotazione.x, puntoRiferimentoRotazione.y, puntoRiferimentoRotazione.z);
					}
				}
			}
			if (attivazioneMossa(mossaInCorso,cuboRubik[i])){
				glRotatef(angolo_rotazione, puntoRiferimentoRotazione.x, puntoRiferimentoRotazione.y, puntoRiferimentoRotazione.z);
			}
		}else{
			if (!mosseEffettuate.empty()){
				for (int x = 0;x < mosseEffettuate.size();x++){
					if (attivazioneMossa(mosseEffettuate[x],cuboRubik[i])){
						glRotatef(90.0, puntoRiferimentoRotazione.x, puntoRiferimentoRotazione.y, puntoRiferimentoRotazione.z);
					}
				}
			}
		}
		glTranslatef(cuboRubik[i].posizione.x,cuboRubik[i].posizione.y,cuboRubik[i].posizione.z);
		glCallList(_displayListId_smallcube);
		glPopMatrix();

	}

	glutSwapBuffers();

}

void timer(int value) {

	SetActiveWindow(FindWindowA(0,"Cubo Di Rubik"));

	//per permettere una rotazione progressiva dell'intero cubo
	if (frecciaPremuta)
	{
		if (valorePrecX > angolo_asse_x) {
			valorePrecX -= 3;
		} else if (valorePrecX < angolo_asse_x){
			valorePrecX += 3;
		}
		if (valorePrecY > angolo_asse_y) {
			valorePrecY -= 3;
		} else if (valorePrecY < angolo_asse_y){
			valorePrecY += 3;
		}
		if (valorePrecZ > angolo_asse_z) {
			valorePrecZ -= 3;
		} else if (valorePrecZ < angolo_asse_z){
			valorePrecZ += 3;
		}
		if (valorePrecX == angolo_asse_x && valorePrecY == angolo_asse_y && valorePrecZ == angolo_asse_z){
			frecciaPremuta = false;
		}
	}

	if (angolo_rotazione < scattoRotazione && pussantePremuto) {
		angolo_rotazione += 2.0;
		//angolo_rotazione = scattoRotazione;
	}else{
		angolo_rotazione = 0;
		pussantePremuto = false;
	}

	glutSetWindow(windowsID);
	glutPostRedisplay();
	glutTimerFunc(17, timer, 0);

}

void creaPannelloGlui()
{
	glui = GLUI_Master.create_glui("Comandi Giocatore", GLUI_SUBWINDOW_TOP, (glutGet(GLUT_SCREEN_WIDTH)-larghezza-400)/2, (glutGet(GLUT_SCREEN_HEIGHT)-altezza)/2);

	panel = glui -> add_panel("Movimenti cubo", GLUI_PANEL_EMBOSSED);
	glui -> add_statictext_to_panel(panel,"");
	glui -> add_button_to_panel(panel, "<", -1, gestioneBottoni);	//Prima riga a sinistra
	glui -> add_button_to_panel(panel, "<", -2, gestioneBottoni);	//Seconda riga a sinistra
	glui -> add_button_to_panel(panel, "<", -3, gestioneBottoni);	//Terza riga a sinistra
	glui -> add_statictext_to_panel(panel,"");
	glui->add_column_to_panel( panel, false );
	glui -> add_button_to_panel(panel, "^", -4, gestioneBottoni);	//Prima colonna giù
	glui -> add_statictext_to_panel(panel,"");
	glui -> add_statictext_to_panel(panel,"");
	glui -> add_statictext_to_panel(panel,"");
	glui -> add_button_to_panel(panel, "v", 4, gestioneBottoni);	//Prima colonna sù
	glui->add_column_to_panel( panel, false );
	glui -> add_button_to_panel(panel, "^", -5, gestioneBottoni);	//Seconda colonna giù
	glui -> add_statictext_to_panel(panel,"");
	glui -> add_statictext_to_panel(panel,"");
	glui -> add_statictext_to_panel(panel,"");
	glui -> add_button_to_panel(panel, "v", 5, gestioneBottoni);	//Seconda colonna sù
	glui->add_column_to_panel( panel, false );
	glui -> add_button_to_panel(panel, "^", -6, gestioneBottoni);	//Terza colonna giù
	glui -> add_statictext_to_panel(panel,"");
	glui -> add_statictext_to_panel(panel,"");
	glui -> add_statictext_to_panel(panel,"");
	glui -> add_button_to_panel(panel, "v", 6, gestioneBottoni);	//Terza colonna sù
	glui->add_column_to_panel( panel, false );
	glui -> add_statictext_to_panel(panel,"");
	glui -> add_button_to_panel(panel, ">", 1, gestioneBottoni);	//Prima riga a destra
	glui -> add_button_to_panel(panel, ">", 2, gestioneBottoni);	//Seconda riga a destra
	glui -> add_button_to_panel(panel, ">", 3, gestioneBottoni);	//Terza riga a destra
	glui -> add_statictext_to_panel(panel,"");

	panel = glui->add_panel( "", false );
	glui -> add_button_to_panel(panel, "Shuffle", 7, gestioneBottoni);
	glui->add_column_to_panel( panel, false );
	glui->add_button_to_panel( panel, "Reset", 8, gestioneBottoni);
	glui->add_column_to_panel( panel, false );
	glui->add_button_to_panel( panel, "Quit", 0, gestioneBottoni);

	glui -> set_main_gfx_window(windowsID);
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

	inizializzaCubo();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutTimerFunc(17, timer, 0);
	glutSpecialFunc(specialKeyboard);

	creaPannelloGlui();

	glutMainLoop();
}