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
#include <sstream>

#include "imageloader.h"

#define spostamentoFinestra 400
#define spessoreAssi 3

// Nasconde la console
//#pragma comment(linker,"/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

using namespace std;

typedef enum _Colori{
	WHITE = 0,
	YELLOW = 1,
	BLUE = 2,
	GREEN = 3,
ORANGE = 4,
	RED = 5	
}Colore;

typedef struct{
	float x, y, z;
} Point;

typedef struct{
	int numeroCubo;
	Colore colorifacce[6];
} Cubo;

typedef struct{
	int numeroVisuale; //non dovrebbe servire
	char riga_colonna_sezione; //c = colonna, r = righa, s = sezione
	int valore; //si intende quale riga o colonna vuole muovere.
	bool direzione;
} Mossa;

const float DIMENSIONE_FACCIA = 0.5;
Cubo cuboRubik[3][3][3];
Colore coloriInModifica[6];

Mossa mossaInCorso;
bool pulsantePremuto;
vector<Mossa> mosseEffettuate;
int gradiRotazioneMossa = 90;
float angolo_rotazione = 0;
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
GLUI_RadioGroup *radio_group;

GLuint white_textureId;
GLuint red_textureId;
GLuint blue_textureId;
GLuint green_textureId;
GLuint yellow_textureId;
GLuint orange_textureId;

GLuint texture_faccia_anteriore;	//texture della faccia anteriore
GLuint texture_faccia_posteriore;	//texture della faccia posteriore
GLuint texture_faccia_destra;	//texture della faccia destra
GLuint texture_faccia_sinistra;	//texture della faccia sinistra
GLuint texture_faccia_superiore;	//texture della faccia superiore
GLuint texture_faccia_inferiore;	//texture della faccia inferiore

// fraction of the length to use as height of the characters:
const float LENFRAC = 0.10f;

// fraction of length to use as start location of the characters:
const float BASEFRAC = 1.10f;

// Il carattere 'X':
static float xx[ ] = {
	0.f, 1.f, 0.f, 1.f
};

static float xy[ ] = {
	-.5f, .5f, .5f, -.5f
};

static int xorder[ ] = {
	1, 2, -3, 4
};

// Il carattere 'Y':
static float yx[ ] = {
	0.f, 0.f, -.5f, .5f
};

static float yy[ ] = {
	0.f, .6f, 1.f, 1.f
};

static int yorder[ ] = {
	1, 2, 3, -2, 4
};

// Il carattere 'Z':
static float zx[ ] = {
	1.f, 0.f, 1.f, 0.f, .25f, .75f
};

static float zy[ ] = {
	.5f, .5f, -.5f, -.5f, 0.f, 0.f
};

static int zorder[ ] = {
	1, 2, 3, 4, -5, 6
};

int coloreAssi = 0;

// Le definizioni dei colori
// L'ordine deve rispecchiare l'ordine dei radiobutton
const GLfloat Colors[6][3] = 
{
	{ 0.0, 0.0, 0.0 },	//nero
	{ 1.0, 1.0, 1.0 },	//bianco
	{ 1.0, 0.0, 0.0 },	//rosso
	{ 0.0, 1.0, 0.0 },	//verde
	{ 0.0, 0.0, 1.0 },	//blu
	{ 1.0, 1.0, 0.0 },	//giallo
};

//Carica il file, lo converte in texture e ritorna l'id della texture in base all'id del colore immesso
GLuint caricaTexture(Colore colore) {

	Image* image = NULL;
	GLuint textureId;
	switch (colore){
	case WHITE:
		image = loadBMP("white.bmp");
		cout << "caricato bianco" << endl;
		break;
	case YELLOW:
		image = loadBMP("yellow.bmp");
		cout << "caricato giallo" << endl;
		break;
	case BLUE:
		image = loadBMP("blue.bmp");
		cout << "caricato blu" << endl;
		break;
	case GREEN:
		image = loadBMP("green.bmp");
		cout << "caricato verde" << endl;
		break;

	case ORANGE:
		image = loadBMP("orange.bmp");
		cout << "caricato arancione" << endl;
		break;
	case RED:
		image = loadBMP("red.bmp");
		cout << "caricato rosso" << endl;
		break;
	}
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
	//delete image;
	return textureId;
}

//restituisce l'id della texture in base al colore cercato
GLuint selezionaTextureCaricata(Colore colore){
	GLuint textureId;

	switch (colore){
	case WHITE:
		textureId = white_textureId;
		break;
	case YELLOW:
		textureId = yellow_textureId;
		break;
	case BLUE:
		textureId = blue_textureId;
		break;
	case GREEN:
		textureId = green_textureId;
		break;

	case ORANGE:
		textureId = orange_textureId;
		break;
	case RED:
		textureId = red_textureId;
		break;
	}

	return textureId;
}

string int2str(int x) 
{
	stringstream ss;
	ss << x;
	return ss.str( );
}

void disegnaTesto(float x, float y, string text)
{
	glRasterPos2f(x, y);
	int len = text.length();
	for (int i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
	}
}

void disegnaAssi(float lunghezza)
{
	glColor3fv(Colors[coloreAssi]);
	glBegin(GL_LINE_STRIP);
	glVertex3f(lunghezza, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, lunghezza, 0);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, lunghezza);
	glEnd();

	float fact = LENFRAC * lunghezza;
	float base = BASEFRAC * lunghezza;

	glBegin( GL_LINE_STRIP );
	for( int i = 0; i < 4; i++ )
	{
		int j = xorder[i];
		if( j < 0 )
		{

			glEnd( );
			glBegin( GL_LINE_STRIP );
			j = -j;
		}
		j--;
		glVertex3f( base + fact*xx[j], fact*xy[j], 0.0 );
	}
	glEnd( );

	glBegin( GL_LINE_STRIP );
	for( int i = 0; i < 5; i++ )
	{
		int j = yorder[i];
		if( j < 0 )
		{

			glEnd( );
			glBegin( GL_LINE_STRIP );
			j = -j;
		}
		j--;
		glVertex3f( fact*yx[j], base + fact*yy[j], 0.0 );
	}
	glEnd( );

	glBegin( GL_LINE_STRIP );
	for( int i = 0; i < 6; i++ )
	{
		int j = zorder[i];
		if( j < 0 )
		{

			glEnd( );
			glBegin( GL_LINE_STRIP );
			j = -j;
		}
		j--;
		glVertex3f( 0.0, fact*zy[j], base + fact*zx[j] );
	}
	glEnd( );
	glColor3f(1.0, 1.0, 1.0);
}

void cuboSingolo(Colore colori[6])
{
	texture_faccia_anteriore = selezionaTextureCaricata(colori[0]);	//texture faccia anteriore
	texture_faccia_posteriore = selezionaTextureCaricata(colori[1]);	//texture faccia posteriore
	texture_faccia_destra = selezionaTextureCaricata(colori[2]);	//texture faccia destra
	texture_faccia_sinistra = selezionaTextureCaricata(colori[3]);	//texture faccia sinistra
	texture_faccia_superiore = selezionaTextureCaricata(colori[4]);	//texture faccia superiore
	texture_faccia_inferiore = selezionaTextureCaricata(colori[5]);	//texture faccia inferiore

	//Faccia Anteriore
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_faccia_anteriore);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-DIMENSIONE_FACCIA, -DIMENSIONE_FACCIA, DIMENSIONE_FACCIA);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(DIMENSIONE_FACCIA, -DIMENSIONE_FACCIA, DIMENSIONE_FACCIA);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(DIMENSIONE_FACCIA, DIMENSIONE_FACCIA, DIMENSIONE_FACCIA);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-DIMENSIONE_FACCIA, DIMENSIONE_FACCIA, DIMENSIONE_FACCIA);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	//Faccia Posteriore
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_faccia_posteriore);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-DIMENSIONE_FACCIA, -DIMENSIONE_FACCIA, -DIMENSIONE_FACCIA);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-DIMENSIONE_FACCIA, DIMENSIONE_FACCIA, -DIMENSIONE_FACCIA);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(DIMENSIONE_FACCIA, DIMENSIONE_FACCIA, -DIMENSIONE_FACCIA);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(DIMENSIONE_FACCIA, -DIMENSIONE_FACCIA, -DIMENSIONE_FACCIA);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	//Faccia Destra
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_faccia_destra);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(DIMENSIONE_FACCIA, -DIMENSIONE_FACCIA, -DIMENSIONE_FACCIA);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(DIMENSIONE_FACCIA, DIMENSIONE_FACCIA, -DIMENSIONE_FACCIA);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(DIMENSIONE_FACCIA, DIMENSIONE_FACCIA, DIMENSIONE_FACCIA);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(DIMENSIONE_FACCIA, -DIMENSIONE_FACCIA, DIMENSIONE_FACCIA);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	//Faccia Sinistra
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_faccia_sinistra);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-DIMENSIONE_FACCIA, -DIMENSIONE_FACCIA, -DIMENSIONE_FACCIA);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-DIMENSIONE_FACCIA, -DIMENSIONE_FACCIA, DIMENSIONE_FACCIA);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-DIMENSIONE_FACCIA, DIMENSIONE_FACCIA, DIMENSIONE_FACCIA);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-DIMENSIONE_FACCIA, DIMENSIONE_FACCIA, -DIMENSIONE_FACCIA);
	glEnd();
	glDisable(GL_TEXTURE_2D);	

	//Faccia Superiore
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_faccia_superiore);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-DIMENSIONE_FACCIA, DIMENSIONE_FACCIA, -DIMENSIONE_FACCIA);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-DIMENSIONE_FACCIA, DIMENSIONE_FACCIA, DIMENSIONE_FACCIA);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(DIMENSIONE_FACCIA, DIMENSIONE_FACCIA, DIMENSIONE_FACCIA);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(DIMENSIONE_FACCIA, DIMENSIONE_FACCIA, -DIMENSIONE_FACCIA);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	//Faccia Inferiore
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_faccia_inferiore);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-DIMENSIONE_FACCIA, -DIMENSIONE_FACCIA, -DIMENSIONE_FACCIA);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(DIMENSIONE_FACCIA, -DIMENSIONE_FACCIA, -DIMENSIONE_FACCIA);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(DIMENSIONE_FACCIA, -DIMENSIONE_FACCIA, DIMENSIONE_FACCIA);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-DIMENSIONE_FACCIA, -DIMENSIONE_FACCIA, DIMENSIONE_FACCIA);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void cambiaColoreCuboColonna(Colore coloriCubo[6],bool direzione){
	if(direzione){
		//ruota in avanti
		coloriInModifica[4] = coloriCubo[1];	//il colore sopra viene sostituito dal colore dietro
		coloriInModifica[0] = coloriCubo[4];	//il colore frontale viene sostituito dal colore sopra
		coloriInModifica[5] = coloriCubo[0];	//il colore sotto viene sostituito dal colore frontale
		coloriInModifica[1] = coloriCubo[5];	//il colore dietro viene sostituito dal colore sotto
	}else{
		//ruota in dietro
		coloriInModifica[4] = coloriCubo[0];	//il colore sopra viene sostituito dal colore frontale
		coloriInModifica[0] = coloriCubo[5];	//il colore frontale viene sostituito dal colore sotto
		coloriInModifica[5] = coloriCubo[1];	//il colore sotto viene sostituito dal colore dietro
		coloriInModifica[1] = coloriCubo[4];	//il colore dietro viene sostituito dal colore sopra
	}

	//colori invariati
	coloriInModifica[2] = coloriCubo[2];
	coloriInModifica[3] = coloriCubo[3];
}

void cambiaColoreCuboRiga(Colore coloriCubo[6],bool direzione){
	if (direzione){
		//ruota verso destra
		coloriInModifica[2] = coloriCubo[0];	//il colore destra viene sostituito dal colore frontale
		coloriInModifica[0] = coloriCubo[3];	//il colore frontale viene sostituito dal colore sinistro
		coloriInModifica[3] = coloriCubo[1];	//il colore sinistra viene sostituito dal colore posteriore
		coloriInModifica[1] = coloriCubo[2];	//il colore dietro viene sostituito dal colore destro
	}else{
		//ruota verso sinistra
		coloriInModifica[2] = coloriCubo[1];	//il colore destra viene sostituito dal colore posteriore
		coloriInModifica[0] = coloriCubo[2];	//il colore frontale viene sostituito dal colore destro
		coloriInModifica[3] = coloriCubo[0];	//il colore sinistra viene sostituito dal colore frontale
		coloriInModifica[1] = coloriCubo[3];	//il colore dietro viene sostituito dal colore sinistro
	}
	//colori invariati
	coloriInModifica[4] = coloriCubo[4];
	coloriInModifica[5] = coloriCubo[5];
}

void cambiaColoreCuboSezione(Colore coloriCubo[6],bool direzione){
	if(direzione){
		//ruota verso destra
		coloriInModifica[2] = coloriCubo[4];	//il colore destra viene sostituito dal colore sopra
		coloriInModifica[4] = coloriCubo[3];	//il colore sopra viene sostituito dal colore sinistro
		coloriInModifica[3] = coloriCubo[5];	//il colore sinistra viene sostituito dal colore sotto
		coloriInModifica[5] = coloriCubo[2];	//il colore sotto viene sostituito dal colore destro
	}else{
		//ruota verso sinistra
		coloriInModifica[2] = coloriCubo[5];	//il colore destra viene sostituito dal colore sotto
		coloriInModifica[4] = coloriCubo[2];	//il colore sopra viene sostituito dal colore destro
		coloriInModifica[3] = coloriCubo[4];	//il colore sinistra viene sostituito dal colore sopra
		coloriInModifica[5] = coloriCubo[3];	//il colore sotto viene sostituito dal colore sinistro
	}
	//colori invariati
	coloriInModifica[1] = coloriCubo[1];
	coloriInModifica[0] = coloriCubo[0];
}

void inizializzaCubo(){
	int numeroCubo = 1;

	for (int x = 0; x < 3;x++){
		for (int y = 0; y < 3;y++){
			for (int z = 0; z < 3;z++){
				Cubo nuovoCubo;
				nuovoCubo.colorifacce[0] = WHITE;
				nuovoCubo.colorifacce[1] = YELLOW;
				nuovoCubo.colorifacce[2] = BLUE;
				nuovoCubo.colorifacce[3] = GREEN;
				nuovoCubo.colorifacce[4] = ORANGE;
				nuovoCubo.colorifacce[5] = RED;

				nuovoCubo.numeroCubo = numeroCubo;

				cuboRubik[x][y][z] = nuovoCubo;
				cout <<"Numero cubo: " << numeroCubo <<" x: "<< x << " y: "<< y << " z: " << z << " PosReale x: " << x-1  << " PosReale y: "<< y-1 << " PosReale z: "<< z-1 << endl;
				numeroCubo++;
			}
		}
	}

}

void ruotaColonnaRubik(int x, bool direzione){
	if (direzione){

		Cubo temp = cuboRubik[x][0][0];

		cambiaColoreCuboColonna(cuboRubik[x][0][2].colorifacce,direzione);
		cuboRubik[x][0][2].colorifacce[0] = coloriInModifica[0];
		cuboRubik[x][0][2].colorifacce[1] = coloriInModifica[1];
		cuboRubik[x][0][2].colorifacce[2] = coloriInModifica[2];
		cuboRubik[x][0][2].colorifacce[3] = coloriInModifica[3];
		cuboRubik[x][0][2].colorifacce[4] = coloriInModifica[4];
		cuboRubik[x][0][2].colorifacce[5] = coloriInModifica[5];

		cuboRubik[x][0][0] = cuboRubik[x][0][2];

		cambiaColoreCuboColonna(cuboRubik[x][2][2].colorifacce,direzione);
		cuboRubik[x][2][2].colorifacce[0] = coloriInModifica[0];
		cuboRubik[x][2][2].colorifacce[1] = coloriInModifica[1];
		cuboRubik[x][2][2].colorifacce[2] = coloriInModifica[2];
		cuboRubik[x][2][2].colorifacce[3] = coloriInModifica[3];
		cuboRubik[x][2][2].colorifacce[4] = coloriInModifica[4];
		cuboRubik[x][2][2].colorifacce[5] = coloriInModifica[5];

		cuboRubik[x][0][2] = cuboRubik[x][2][2];

		cambiaColoreCuboColonna(cuboRubik[x][2][0].colorifacce,direzione);
		cuboRubik[x][2][0].colorifacce[0] = coloriInModifica[0];
		cuboRubik[x][2][0].colorifacce[1] = coloriInModifica[1];
		cuboRubik[x][2][0].colorifacce[2] = coloriInModifica[2];
		cuboRubik[x][2][0].colorifacce[3] = coloriInModifica[3];
		cuboRubik[x][2][0].colorifacce[4] = coloriInModifica[4];
		cuboRubik[x][2][0].colorifacce[5] = coloriInModifica[5];

		cuboRubik[x][2][2] = cuboRubik[x][2][0];

		cambiaColoreCuboColonna(temp.colorifacce,direzione);
		temp.colorifacce[0] = coloriInModifica[0];
		temp.colorifacce[1] = coloriInModifica[1];
		temp.colorifacce[2] = coloriInModifica[2];
		temp.colorifacce[3] = coloriInModifica[3];
		temp.colorifacce[4] = coloriInModifica[4];
		temp.colorifacce[5] = coloriInModifica[5];

		cuboRubik[x][2][0] = temp;

		temp = cuboRubik[x][0][1];

		cambiaColoreCuboColonna(cuboRubik[x][1][2].colorifacce,direzione);
		cuboRubik[x][1][2].colorifacce[0] = coloriInModifica[0];
		cuboRubik[x][1][2].colorifacce[1] = coloriInModifica[1];
		cuboRubik[x][1][2].colorifacce[2] = coloriInModifica[2];
		cuboRubik[x][1][2].colorifacce[3] = coloriInModifica[3];
		cuboRubik[x][1][2].colorifacce[4] = coloriInModifica[4];
		cuboRubik[x][1][2].colorifacce[5] = coloriInModifica[5];

		cuboRubik[x][0][1] = cuboRubik[x][1][2];

		cambiaColoreCuboColonna(cuboRubik[x][2][1].colorifacce,direzione);
		cuboRubik[x][2][1].colorifacce[0] = coloriInModifica[0];
		cuboRubik[x][2][1].colorifacce[1] = coloriInModifica[1];
		cuboRubik[x][2][1].colorifacce[2] = coloriInModifica[2];
		cuboRubik[x][2][1].colorifacce[3] = coloriInModifica[3];
		cuboRubik[x][2][1].colorifacce[4] = coloriInModifica[4];
		cuboRubik[x][2][1].colorifacce[5] = coloriInModifica[5];

		cuboRubik[x][1][2] = cuboRubik[x][2][1];

		cambiaColoreCuboColonna(cuboRubik[x][1][0].colorifacce,direzione);
		cuboRubik[x][1][0].colorifacce[0] = coloriInModifica[0];
		cuboRubik[x][1][0].colorifacce[1] = coloriInModifica[1];
		cuboRubik[x][1][0].colorifacce[2] = coloriInModifica[2];
		cuboRubik[x][1][0].colorifacce[3] = coloriInModifica[3];
		cuboRubik[x][1][0].colorifacce[4] = coloriInModifica[4];
		cuboRubik[x][1][0].colorifacce[5] = coloriInModifica[5];

		cuboRubik[x][2][1] = cuboRubik[x][1][0];

		cambiaColoreCuboColonna(temp.colorifacce,direzione);
		temp.colorifacce[0] = coloriInModifica[0];
		temp.colorifacce[1] = coloriInModifica[1];
		temp.colorifacce[2] = coloriInModifica[2];
		temp.colorifacce[3] = coloriInModifica[3];
		temp.colorifacce[4] = coloriInModifica[4];
		temp.colorifacce[5] = coloriInModifica[5];

		cuboRubik[x][1][0] = temp;

	}else{

		Cubo temp = cuboRubik[x][0][0];

		cambiaColoreCuboColonna(cuboRubik[x][2][0].colorifacce,direzione);
		cuboRubik[x][2][0].colorifacce[0] = coloriInModifica[0];
		cuboRubik[x][2][0].colorifacce[1] = coloriInModifica[1];
		cuboRubik[x][2][0].colorifacce[2] = coloriInModifica[2];
		cuboRubik[x][2][0].colorifacce[3] = coloriInModifica[3];
		cuboRubik[x][2][0].colorifacce[4] = coloriInModifica[4];
		cuboRubik[x][2][0].colorifacce[5] = coloriInModifica[5];

		cout<<"Il cubo: "<< cuboRubik[x][2][0].numeroCubo <<" va al posto del cubo: " << cuboRubik[x][0][0].numeroCubo<<endl;
		cuboRubik[x][0][0] = cuboRubik[x][2][0];

		cambiaColoreCuboColonna(cuboRubik[x][2][2].colorifacce,direzione);
		cuboRubik[x][2][2].colorifacce[0] = coloriInModifica[0];
		cuboRubik[x][2][2].colorifacce[1] = coloriInModifica[1];
		cuboRubik[x][2][2].colorifacce[2] = coloriInModifica[2];
		cuboRubik[x][2][2].colorifacce[3] = coloriInModifica[3];
		cuboRubik[x][2][2].colorifacce[4] = coloriInModifica[4];
		cuboRubik[x][2][2].colorifacce[5] = coloriInModifica[5];

		cout<<"Il cubo: "<< cuboRubik[x][2][2].numeroCubo <<" va al posto del cubo: " << cuboRubik[x][2][0].numeroCubo<<endl;
		cuboRubik[x][2][0] = cuboRubik[x][2][2];

		cambiaColoreCuboColonna(cuboRubik[x][0][2].colorifacce,direzione);
		cuboRubik[x][0][2].colorifacce[0] = coloriInModifica[0];
		cuboRubik[x][0][2].colorifacce[1] = coloriInModifica[1];
		cuboRubik[x][0][2].colorifacce[2] = coloriInModifica[2];
		cuboRubik[x][0][2].colorifacce[3] = coloriInModifica[3];
		cuboRubik[x][0][2].colorifacce[4] = coloriInModifica[4];
		cuboRubik[x][0][2].colorifacce[5] = coloriInModifica[5];

		cout<<"Il cubo: "<< cuboRubik[x][0][2].numeroCubo <<" va al posto del cubo: " << cuboRubik[x][2][2].numeroCubo<<endl;
		cuboRubik[x][2][2] = cuboRubik[x][0][2];

		cambiaColoreCuboColonna(temp.colorifacce,direzione);
		temp.colorifacce[0] = coloriInModifica[0];
		temp.colorifacce[1] = coloriInModifica[1];
		temp.colorifacce[2] = coloriInModifica[2];
		temp.colorifacce[3] = coloriInModifica[3];
		temp.colorifacce[4] = coloriInModifica[4];
		temp.colorifacce[5] = coloriInModifica[5];

		cout<<"Il cubo: "<< temp.numeroCubo <<" va al posto del cubo: " << cuboRubik[x][0][2].numeroCubo<<endl;
		cuboRubik[x][0][2] = temp;

		temp = cuboRubik[x][0][1];

		cambiaColoreCuboColonna(cuboRubik[x][1][0].colorifacce,direzione);
		cuboRubik[x][1][0].colorifacce[0] = coloriInModifica[0];
		cuboRubik[x][1][0].colorifacce[1] = coloriInModifica[1];
		cuboRubik[x][1][0].colorifacce[2] = coloriInModifica[2];
		cuboRubik[x][1][0].colorifacce[3] = coloriInModifica[3];
		cuboRubik[x][1][0].colorifacce[4] = coloriInModifica[4];
		cuboRubik[x][1][0].colorifacce[5] = coloriInModifica[5];

		cout<<"Il cubo: "<< cuboRubik[x][1][0].numeroCubo <<" va al posto del cubo: " << cuboRubik[x][0][1].numeroCubo<<endl;
		cuboRubik[x][0][1] = cuboRubik[x][1][0];

		cambiaColoreCuboColonna(cuboRubik[x][2][1].colorifacce,direzione);
		cuboRubik[x][2][1].colorifacce[0] = coloriInModifica[0];
		cuboRubik[x][2][1].colorifacce[1] = coloriInModifica[1];
		cuboRubik[x][2][1].colorifacce[2] = coloriInModifica[2];
		cuboRubik[x][2][1].colorifacce[3] = coloriInModifica[3];
		cuboRubik[x][2][1].colorifacce[4] = coloriInModifica[4];
		cuboRubik[x][2][1].colorifacce[5] = coloriInModifica[5];

		cout<<"Il cubo: "<< cuboRubik[x][2][1].numeroCubo <<" va al posto del cubo: " << cuboRubik[x][1][0].numeroCubo<<endl;
		cuboRubik[x][1][0] = cuboRubik[x][2][1];

		cambiaColoreCuboColonna(cuboRubik[x][1][2].colorifacce,direzione);
		cuboRubik[x][1][2].colorifacce[0] = coloriInModifica[0];
		cuboRubik[x][1][2].colorifacce[1] = coloriInModifica[1];
		cuboRubik[x][1][2].colorifacce[2] = coloriInModifica[2];
		cuboRubik[x][1][2].colorifacce[3] = coloriInModifica[3];
		cuboRubik[x][1][2].colorifacce[4] = coloriInModifica[4];
		cuboRubik[x][1][2].colorifacce[5] = coloriInModifica[5];

		cout<<"Il cubo: "<< cuboRubik[x][1][2].numeroCubo <<" va al posto del cubo: " << cuboRubik[x][2][1].numeroCubo<<endl;
		cuboRubik[x][2][1] = cuboRubik[x][1][2];

		cambiaColoreCuboColonna(temp.colorifacce,direzione);
		temp.colorifacce[0] = coloriInModifica[0];
		temp.colorifacce[1] = coloriInModifica[1];
		temp.colorifacce[2] = coloriInModifica[2];
		temp.colorifacce[3] = coloriInModifica[3];
		temp.colorifacce[4] = coloriInModifica[4];
		temp.colorifacce[5] = coloriInModifica[5];

		cout<<"Il cubo: "<< temp.numeroCubo <<" va al posto del cubo: " << cuboRubik[x][1][2].numeroCubo<<endl;
		cuboRubik[x][1][2] = temp;
	}

	cambiaColoreCuboColonna(cuboRubik[x][1][1].colorifacce,direzione);	//modifica i colori del cubo centrale che non si vede
	cuboRubik[x][1][1].colorifacce[0] = coloriInModifica[0];
	cuboRubik[x][1][1].colorifacce[1] = coloriInModifica[1];
	cuboRubik[x][1][1].colorifacce[2] = coloriInModifica[2];
	cuboRubik[x][1][1].colorifacce[3] = coloriInModifica[3];
	cuboRubik[x][1][1].colorifacce[4] = coloriInModifica[4];
	cuboRubik[x][1][1].colorifacce[5] = coloriInModifica[5];
}

void ruotaRigaRubik(int y,bool direzione){
	if (direzione){
		Cubo temp = cuboRubik[0][y][0];

		cambiaColoreCuboRiga(cuboRubik[2][y][0].colorifacce,direzione);
		cuboRubik[2][y][0].colorifacce[0] = coloriInModifica[0];
		cuboRubik[2][y][0].colorifacce[1] = coloriInModifica[1];
		cuboRubik[2][y][0].colorifacce[2] = coloriInModifica[2];
		cuboRubik[2][y][0].colorifacce[3] = coloriInModifica[3];
		cuboRubik[2][y][0].colorifacce[4] = coloriInModifica[4];
		cuboRubik[2][y][0].colorifacce[5] = coloriInModifica[5];

		cuboRubik[0][y][0] = cuboRubik[2][y][0];

		cambiaColoreCuboRiga(cuboRubik[2][y][2].colorifacce,direzione);
		cuboRubik[2][y][2].colorifacce[0] = coloriInModifica[0];
		cuboRubik[2][y][2].colorifacce[1] = coloriInModifica[1];
		cuboRubik[2][y][2].colorifacce[2] = coloriInModifica[2];
		cuboRubik[2][y][2].colorifacce[3] = coloriInModifica[3];
		cuboRubik[2][y][2].colorifacce[4] = coloriInModifica[4];
		cuboRubik[2][y][2].colorifacce[5] = coloriInModifica[5];

		cuboRubik[2][y][0] = cuboRubik[2][y][2];

		cambiaColoreCuboRiga(cuboRubik[0][y][2].colorifacce,direzione);
		cuboRubik[0][y][2].colorifacce[0] = coloriInModifica[0];
		cuboRubik[0][y][2].colorifacce[1] = coloriInModifica[1];
		cuboRubik[0][y][2].colorifacce[2] = coloriInModifica[2];
		cuboRubik[0][y][2].colorifacce[3] = coloriInModifica[3];
		cuboRubik[0][y][2].colorifacce[4] = coloriInModifica[4];
		cuboRubik[0][y][2].colorifacce[5] = coloriInModifica[5];

		cuboRubik[2][y][2] = cuboRubik[0][y][2];

		cambiaColoreCuboRiga(temp.colorifacce,direzione);
		temp.colorifacce[0] = coloriInModifica[0];
		temp.colorifacce[1] = coloriInModifica[1];
		temp.colorifacce[2] = coloriInModifica[2];
		temp.colorifacce[3] = coloriInModifica[3];
		temp.colorifacce[4] = coloriInModifica[4];
		temp.colorifacce[5] = coloriInModifica[5];

		cuboRubik[0][y][2] = temp;

		temp = cuboRubik[0][y][1];

		cambiaColoreCuboRiga(cuboRubik[1][y][0].colorifacce,direzione);
		cuboRubik[1][y][0].colorifacce[0] = coloriInModifica[0];
		cuboRubik[1][y][0].colorifacce[1] = coloriInModifica[1];
		cuboRubik[1][y][0].colorifacce[2] = coloriInModifica[2];
		cuboRubik[1][y][0].colorifacce[3] = coloriInModifica[3];
		cuboRubik[1][y][0].colorifacce[4] = coloriInModifica[4];
		cuboRubik[1][y][0].colorifacce[5] = coloriInModifica[5];

		cuboRubik[0][y][1] = cuboRubik[1][y][0];

		cambiaColoreCuboRiga(cuboRubik[2][y][1].colorifacce,direzione);
		cuboRubik[2][y][1].colorifacce[0] = coloriInModifica[0];
		cuboRubik[2][y][1].colorifacce[1] = coloriInModifica[1];
		cuboRubik[2][y][1].colorifacce[2] = coloriInModifica[2];
		cuboRubik[2][y][1].colorifacce[3] = coloriInModifica[3];
		cuboRubik[2][y][1].colorifacce[4] = coloriInModifica[4];
		cuboRubik[2][y][1].colorifacce[5] = coloriInModifica[5];

		cuboRubik[1][y][0] = cuboRubik[2][y][1];

		cambiaColoreCuboRiga(cuboRubik[1][y][2].colorifacce,direzione);
		cuboRubik[1][y][2].colorifacce[0] = coloriInModifica[0];
		cuboRubik[1][y][2].colorifacce[1] = coloriInModifica[1];
		cuboRubik[1][y][2].colorifacce[2] = coloriInModifica[2];
		cuboRubik[1][y][2].colorifacce[3] = coloriInModifica[3];
		cuboRubik[1][y][2].colorifacce[4] = coloriInModifica[4];
		cuboRubik[1][y][2].colorifacce[5] = coloriInModifica[5];

		cuboRubik[2][y][1] = cuboRubik[1][y][2];

		cambiaColoreCuboRiga(temp.colorifacce,direzione);
		temp.colorifacce[0] = coloriInModifica[0];
		temp.colorifacce[1] = coloriInModifica[1];
		temp.colorifacce[2] = coloriInModifica[2];
		temp.colorifacce[3] = coloriInModifica[3];
		temp.colorifacce[4] = coloriInModifica[4];
		temp.colorifacce[5] = coloriInModifica[5];

		cuboRubik[1][y][2] = temp;

	}else{

		Cubo temp = cuboRubik[0][y][0];

		cambiaColoreCuboRiga(cuboRubik[0][y][2].colorifacce,direzione);
		cuboRubik[0][y][2].colorifacce[0] = coloriInModifica[0];
		cuboRubik[0][y][2].colorifacce[1] = coloriInModifica[1];
		cuboRubik[0][y][2].colorifacce[2] = coloriInModifica[2];
		cuboRubik[0][y][2].colorifacce[3] = coloriInModifica[3];
		cuboRubik[0][y][2].colorifacce[4] = coloriInModifica[4];
		cuboRubik[0][y][2].colorifacce[5] = coloriInModifica[5];

		cuboRubik[0][y][0] = cuboRubik[0][y][2];

		cambiaColoreCuboRiga(cuboRubik[2][y][2].colorifacce,direzione);
		cuboRubik[2][y][2].colorifacce[0] = coloriInModifica[0];
		cuboRubik[2][y][2].colorifacce[1] = coloriInModifica[1];
		cuboRubik[2][y][2].colorifacce[2] = coloriInModifica[2];
		cuboRubik[2][y][2].colorifacce[3] = coloriInModifica[3];
		cuboRubik[2][y][2].colorifacce[4] = coloriInModifica[4];
		cuboRubik[2][y][2].colorifacce[5] = coloriInModifica[5];

		cuboRubik[0][y][2] = cuboRubik[2][y][2];

		cambiaColoreCuboRiga(cuboRubik[2][y][0].colorifacce,direzione);
		cuboRubik[2][y][0].colorifacce[0] = coloriInModifica[0];
		cuboRubik[2][y][0].colorifacce[1] = coloriInModifica[1];
		cuboRubik[2][y][0].colorifacce[2] = coloriInModifica[2];
		cuboRubik[2][y][0].colorifacce[3] = coloriInModifica[3];
		cuboRubik[2][y][0].colorifacce[4] = coloriInModifica[4];
		cuboRubik[2][y][0].colorifacce[5] = coloriInModifica[5];

		cuboRubik[2][y][2] = cuboRubik[2][y][0];

		cambiaColoreCuboRiga(temp.colorifacce,direzione);
		temp.colorifacce[0] = coloriInModifica[0];
		temp.colorifacce[1] = coloriInModifica[1];
		temp.colorifacce[2] = coloriInModifica[2];
		temp.colorifacce[3] = coloriInModifica[3];
		temp.colorifacce[4] = coloriInModifica[4];
		temp.colorifacce[5] = coloriInModifica[5];

		cuboRubik[2][y][0] = temp;

		temp = cuboRubik[0][y][1];

		cambiaColoreCuboRiga(cuboRubik[1][y][2].colorifacce,direzione);
		cuboRubik[1][y][2].colorifacce[0] = coloriInModifica[0];
		cuboRubik[1][y][2].colorifacce[1] = coloriInModifica[1];
		cuboRubik[1][y][2].colorifacce[2] = coloriInModifica[2];
		cuboRubik[1][y][2].colorifacce[3] = coloriInModifica[3];
		cuboRubik[1][y][2].colorifacce[4] = coloriInModifica[4];
		cuboRubik[1][y][2].colorifacce[5] = coloriInModifica[5];

		cuboRubik[0][y][1] = cuboRubik[1][y][2];

		cambiaColoreCuboRiga(cuboRubik[2][y][1].colorifacce,direzione);
		cuboRubik[2][y][1].colorifacce[0] = coloriInModifica[0];
		cuboRubik[2][y][1].colorifacce[1] = coloriInModifica[1];
		cuboRubik[2][y][1].colorifacce[2] = coloriInModifica[2];
		cuboRubik[2][y][1].colorifacce[3] = coloriInModifica[3];
		cuboRubik[2][y][1].colorifacce[4] = coloriInModifica[4];
		cuboRubik[2][y][1].colorifacce[5] = coloriInModifica[5];

		cuboRubik[1][y][2] = cuboRubik[2][y][1];

		cambiaColoreCuboRiga(cuboRubik[1][y][0].colorifacce,direzione);
		cuboRubik[1][y][0].colorifacce[0] = coloriInModifica[0];
		cuboRubik[1][y][0].colorifacce[1] = coloriInModifica[1];
		cuboRubik[1][y][0].colorifacce[2] = coloriInModifica[2];
		cuboRubik[1][y][0].colorifacce[3] = coloriInModifica[3];
		cuboRubik[1][y][0].colorifacce[4] = coloriInModifica[4];
		cuboRubik[1][y][0].colorifacce[5] = coloriInModifica[5];

		cuboRubik[2][y][1] = cuboRubik[1][y][0];

		cambiaColoreCuboRiga(temp.colorifacce,direzione);
		temp.colorifacce[0] = coloriInModifica[0];
		temp.colorifacce[1] = coloriInModifica[1];
		temp.colorifacce[2] = coloriInModifica[2];
		temp.colorifacce[3] = coloriInModifica[3];
		temp.colorifacce[4] = coloriInModifica[4];
		temp.colorifacce[5] = coloriInModifica[5];

		cuboRubik[1][y][0] = temp;
	}

	cambiaColoreCuboRiga(cuboRubik[1][y][1].colorifacce,direzione);	//modifica i colori del cubo centrale che non si vede
	cuboRubik[1][y][1].colorifacce[0] = coloriInModifica[0];
	cuboRubik[1][y][1].colorifacce[1] = coloriInModifica[1];
	cuboRubik[1][y][1].colorifacce[2] = coloriInModifica[2];
	cuboRubik[1][y][1].colorifacce[3] = coloriInModifica[3];
	cuboRubik[1][y][1].colorifacce[4] = coloriInModifica[4];
	cuboRubik[1][y][1].colorifacce[5] = coloriInModifica[5];

}

void ruotaSezioneRubik(int z,bool direzione){
	if (direzione){
		Cubo temp = cuboRubik[0][0][z];

		cambiaColoreCuboSezione(cuboRubik[2][0][z].colorifacce,direzione);
		cuboRubik[2][0][z].colorifacce[0] = coloriInModifica[0];
		cuboRubik[2][0][z].colorifacce[1] = coloriInModifica[1];
		cuboRubik[2][0][z].colorifacce[2] = coloriInModifica[2];
		cuboRubik[2][0][z].colorifacce[3] = coloriInModifica[3];
		cuboRubik[2][0][z].colorifacce[4] = coloriInModifica[4];
		cuboRubik[2][0][z].colorifacce[5] = coloriInModifica[5];

		cuboRubik[0][0][z] = cuboRubik[2][0][z];


		cambiaColoreCuboSezione(cuboRubik[2][2][z].colorifacce,direzione);
		cuboRubik[2][2][z].colorifacce[0] = coloriInModifica[0];
		cuboRubik[2][2][z].colorifacce[1] = coloriInModifica[1];
		cuboRubik[2][2][z].colorifacce[2] = coloriInModifica[2];
		cuboRubik[2][2][z].colorifacce[3] = coloriInModifica[3];
		cuboRubik[2][2][z].colorifacce[4] = coloriInModifica[4];
		cuboRubik[2][2][z].colorifacce[5] = coloriInModifica[5];

		cuboRubik[2][0][z] = cuboRubik[2][2][z];

		cambiaColoreCuboSezione(cuboRubik[0][2][z].colorifacce,direzione);
		cuboRubik[0][2][z].colorifacce[0] = coloriInModifica[0];
		cuboRubik[0][2][z].colorifacce[1] = coloriInModifica[1];
		cuboRubik[0][2][z].colorifacce[2] = coloriInModifica[2];
		cuboRubik[0][2][z].colorifacce[3] = coloriInModifica[3];
		cuboRubik[0][2][z].colorifacce[4] = coloriInModifica[4];
		cuboRubik[0][2][z].colorifacce[5] = coloriInModifica[5];

		cuboRubik[2][2][z] = cuboRubik[0][2][z];

		cambiaColoreCuboSezione(temp.colorifacce,direzione);
		temp.colorifacce[0] = coloriInModifica[0];
		temp.colorifacce[1] = coloriInModifica[1];
		temp.colorifacce[2] = coloriInModifica[2];
		temp.colorifacce[3] = coloriInModifica[3];
		temp.colorifacce[4] = coloriInModifica[4];
		temp.colorifacce[5] = coloriInModifica[5];

		cuboRubik[0][2][z] = temp;

		temp = cuboRubik[0][1][z];

		cambiaColoreCuboSezione(cuboRubik[1][0][z].colorifacce,direzione);
		cuboRubik[1][0][z].colorifacce[0] = coloriInModifica[0];
		cuboRubik[1][0][z].colorifacce[1] = coloriInModifica[1];
		cuboRubik[1][0][z].colorifacce[2] = coloriInModifica[2];
		cuboRubik[1][0][z].colorifacce[3] = coloriInModifica[3];
		cuboRubik[1][0][z].colorifacce[4] = coloriInModifica[4];
		cuboRubik[1][0][z].colorifacce[5] = coloriInModifica[5];

		cuboRubik[0][1][z] = cuboRubik[1][0][z];

		cambiaColoreCuboSezione(cuboRubik[2][1][z].colorifacce,direzione);
		cuboRubik[2][1][z].colorifacce[0] = coloriInModifica[0];
		cuboRubik[2][1][z].colorifacce[1] = coloriInModifica[1];
		cuboRubik[2][1][z].colorifacce[2] = coloriInModifica[2];
		cuboRubik[2][1][z].colorifacce[3] = coloriInModifica[3];
		cuboRubik[2][1][z].colorifacce[4] = coloriInModifica[4];
		cuboRubik[2][1][z].colorifacce[5] = coloriInModifica[5];

		cuboRubik[1][0][z] = cuboRubik[2][1][z];

		cambiaColoreCuboSezione(cuboRubik[1][2][z].colorifacce,direzione);
		cuboRubik[1][2][z].colorifacce[0] = coloriInModifica[0];
		cuboRubik[1][2][z].colorifacce[1] = coloriInModifica[1];
		cuboRubik[1][2][z].colorifacce[2] = coloriInModifica[2];
		cuboRubik[1][2][z].colorifacce[3] = coloriInModifica[3];
		cuboRubik[1][2][z].colorifacce[4] = coloriInModifica[4];
		cuboRubik[1][2][z].colorifacce[5] = coloriInModifica[5];

		cuboRubik[2][1][z] = cuboRubik[1][2][z];

		cambiaColoreCuboSezione(temp.colorifacce,direzione);
		temp.colorifacce[0] = coloriInModifica[0];
		temp.colorifacce[1] = coloriInModifica[1];
		temp.colorifacce[2] = coloriInModifica[2];
		temp.colorifacce[3] = coloriInModifica[3];
		temp.colorifacce[4] = coloriInModifica[4];
		temp.colorifacce[5] = coloriInModifica[5];

		cuboRubik[1][2][z] = temp;
	} else {
		Cubo temp = cuboRubik[0][0][z];

		cambiaColoreCuboSezione(cuboRubik[0][2][z].colorifacce,direzione);
		cuboRubik[0][2][z].colorifacce[0] = coloriInModifica[0];
		cuboRubik[0][2][z].colorifacce[1] = coloriInModifica[1];
		cuboRubik[0][2][z].colorifacce[2] = coloriInModifica[2];
		cuboRubik[0][2][z].colorifacce[3] = coloriInModifica[3];
		cuboRubik[0][2][z].colorifacce[4] = coloriInModifica[4];
		cuboRubik[0][2][z].colorifacce[5] = coloriInModifica[5];

		cuboRubik[0][0][z] = cuboRubik[0][2][z];

		cambiaColoreCuboSezione(cuboRubik[2][2][z].colorifacce,direzione);
		cuboRubik[2][2][z].colorifacce[0] = coloriInModifica[0];
		cuboRubik[2][2][z].colorifacce[1] = coloriInModifica[1];
		cuboRubik[2][2][z].colorifacce[2] = coloriInModifica[2];
		cuboRubik[2][2][z].colorifacce[3] = coloriInModifica[3];
		cuboRubik[2][2][z].colorifacce[4] = coloriInModifica[4];
		cuboRubik[2][2][z].colorifacce[5] = coloriInModifica[5];

		cuboRubik[0][2][z] = cuboRubik[2][2][z];

		cambiaColoreCuboSezione(cuboRubik[2][0][z].colorifacce,direzione);
		cuboRubik[2][0][z].colorifacce[0] = coloriInModifica[0];
		cuboRubik[2][0][z].colorifacce[1] = coloriInModifica[1];
		cuboRubik[2][0][z].colorifacce[2] = coloriInModifica[2];
		cuboRubik[2][0][z].colorifacce[3] = coloriInModifica[3];
		cuboRubik[2][0][z].colorifacce[4] = coloriInModifica[4];
		cuboRubik[2][0][z].colorifacce[5] = coloriInModifica[5];

		cuboRubik[2][2][z] = cuboRubik[2][0][z];

		cambiaColoreCuboSezione(temp.colorifacce,direzione);
		temp.colorifacce[0] = coloriInModifica[0];
		temp.colorifacce[1] = coloriInModifica[1];
		temp.colorifacce[2] = coloriInModifica[2];
		temp.colorifacce[3] = coloriInModifica[3];
		temp.colorifacce[4] = coloriInModifica[4];
		temp.colorifacce[5] = coloriInModifica[5];

		cuboRubik[2][0][z] = temp;

		temp = cuboRubik[0][1][z];

		cambiaColoreCuboSezione(cuboRubik[1][2][z].colorifacce,direzione);
		cuboRubik[1][2][z].colorifacce[0] = coloriInModifica[0];
		cuboRubik[1][2][z].colorifacce[1] = coloriInModifica[1];
		cuboRubik[1][2][z].colorifacce[2] = coloriInModifica[2];
		cuboRubik[1][2][z].colorifacce[3] = coloriInModifica[3];
		cuboRubik[1][2][z].colorifacce[4] = coloriInModifica[4];
		cuboRubik[1][2][z].colorifacce[5] = coloriInModifica[5];

		cuboRubik[0][1][z] = cuboRubik[1][2][z];

		cambiaColoreCuboSezione(cuboRubik[2][1][z].colorifacce,direzione);
		cuboRubik[2][1][z].colorifacce[0] = coloriInModifica[0];
		cuboRubik[2][1][z].colorifacce[1] = coloriInModifica[1];
		cuboRubik[2][1][z].colorifacce[2] = coloriInModifica[2];
		cuboRubik[2][1][z].colorifacce[3] = coloriInModifica[3];
		cuboRubik[2][1][z].colorifacce[4] = coloriInModifica[4];
		cuboRubik[2][1][z].colorifacce[5] = coloriInModifica[5];

		cuboRubik[1][2][z] = cuboRubik[2][1][z];

		cambiaColoreCuboSezione(cuboRubik[1][0][z].colorifacce,direzione);
		cuboRubik[1][0][z].colorifacce[0] = coloriInModifica[0];
		cuboRubik[1][0][z].colorifacce[1] = coloriInModifica[1];
		cuboRubik[1][0][z].colorifacce[2] = coloriInModifica[2];
		cuboRubik[1][0][z].colorifacce[3] = coloriInModifica[3];
		cuboRubik[1][0][z].colorifacce[4] = coloriInModifica[4];
		cuboRubik[1][0][z].colorifacce[5] = coloriInModifica[5];

		cuboRubik[2][1][z] = cuboRubik[1][0][z];

		cambiaColoreCuboSezione(temp.colorifacce,direzione);
		temp.colorifacce[0] = coloriInModifica[0];
		temp.colorifacce[1] = coloriInModifica[1];
		temp.colorifacce[2] = coloriInModifica[2];
		temp.colorifacce[3] = coloriInModifica[3];
		temp.colorifacce[4] = coloriInModifica[4];
		temp.colorifacce[5] = coloriInModifica[5];

		cuboRubik[1][0][z] = temp;
	}

	cambiaColoreCuboSezione(cuboRubik[1][1][z].colorifacce,direzione);	//modifica i colori del cubo centrale che non si vede
	cuboRubik[1][1][z].colorifacce[0] = coloriInModifica[0];
	cuboRubik[1][1][z].colorifacce[1] = coloriInModifica[1];
	cuboRubik[1][1][z].colorifacce[2] = coloriInModifica[2];
	cuboRubik[1][1][z].colorifacce[3] = coloriInModifica[3];
	cuboRubik[1][1][z].colorifacce[4] = coloriInModifica[4];
	cuboRubik[1][1][z].colorifacce[5] = coloriInModifica[5];
}

void disegnaSuolo()
{ 
	glPushMatrix();
	glBegin(GL_QUADS);
	glColor3ub(0, 0, 0);
	glVertex3f(600.0f, -50.0f, -400.0f);
	glVertex3f(-600.0f, -50.0f, -400.0f);
	glColor3ub(200, 200, 200);
	glVertex3f(-600.0f, -50.0f, 30.0f);
	glVertex3f(600.0f, -50.0f, 30.0f);
	glEnd();
	glPopMatrix();
}

void gestioneBottoni(int opzione){
	switch (opzione)
	{
	case 0: //premuto quit
		glutDestroyWindow(windowsID);
		exit(0);
		break;

	case -1:	//Prima riga a sinistra
		if (!pulsantePremuto){
			pulsantePremuto = !pulsantePremuto;
			mossaInCorso.riga_colonna_sezione = 'r';
			mossaInCorso.valore = 2.0;
			mossaInCorso.direzione = false;
		}
		break;

	case -2:	//Seconda riga a sinistra
		if (!pulsantePremuto){
			pulsantePremuto = !pulsantePremuto;
			mossaInCorso.riga_colonna_sezione = 'r';
			mossaInCorso.valore = 1.0;
			mossaInCorso.direzione = false;
		}
		break;

	case -3:	//Terza riga a sinistra
		if (!pulsantePremuto){
			pulsantePremuto = !pulsantePremuto;
			mossaInCorso.riga_colonna_sezione = 'r';
			mossaInCorso.valore = 0.0;
			mossaInCorso.direzione = false;
		}
		break;

	case 1:		//Prima riga a destra
		if (!pulsantePremuto){
			pulsantePremuto = !pulsantePremuto;
			mossaInCorso.riga_colonna_sezione = 'r';
			mossaInCorso.valore = 2.0;
			mossaInCorso.direzione = true;
			mossaInCorso.direzione = 'r';
		}
		break;

	case 2:		//Seconda riga a destra
		if (!pulsantePremuto){
			pulsantePremuto = !pulsantePremuto;
			mossaInCorso.riga_colonna_sezione = 'r';
			mossaInCorso.valore = 1.0;
			mossaInCorso.direzione = true;
		}
		break;

	case 3:		//Terza riga a destra 
		if (!pulsantePremuto){
			pulsantePremuto = !pulsantePremuto;
			mossaInCorso.riga_colonna_sezione = 'r';
			mossaInCorso.valore = 0.0;
			mossaInCorso.direzione = true;
		}
		break;

	case -4:	//Prima colonna sù
		if (!pulsantePremuto){
			pulsantePremuto = !pulsantePremuto;
			mossaInCorso.riga_colonna_sezione = 'c',
			mossaInCorso.valore = 0.0;
			mossaInCorso.direzione = false;
		}
		break;

	case -5:	//Seconda colonna sù
		if (!pulsantePremuto){
			pulsantePremuto = !pulsantePremuto;
			mossaInCorso.riga_colonna_sezione = 'c';
			mossaInCorso.valore = 1.0;
			mossaInCorso.direzione = false;
		}
		break;

	case -6:	//Terza colonna sù
		if (!pulsantePremuto){
			pulsantePremuto = !pulsantePremuto;
			mossaInCorso.riga_colonna_sezione = 'c';
			mossaInCorso.valore = 2.0;
			mossaInCorso.direzione = false;
		}
		break;

	case 4:		//Prima colonna giù
		if (!pulsantePremuto){
			pulsantePremuto = !pulsantePremuto;
			mossaInCorso.riga_colonna_sezione = 'c';
			mossaInCorso.valore = 0.0;
			mossaInCorso.direzione = true;
		}
		break;

	case 5:		//Seconda colonna giù
		if (!pulsantePremuto){
			pulsantePremuto = !pulsantePremuto;
			mossaInCorso.riga_colonna_sezione = 'c';
			mossaInCorso.valore = 1.0;
			mossaInCorso.direzione = true;
			
		}
		break;

	case 6:		//Terza colonna giù
		if (!pulsantePremuto){
			pulsantePremuto = !pulsantePremuto;
			mossaInCorso.riga_colonna_sezione = 'c';
			mossaInCorso.valore = 2.0;
			mossaInCorso.direzione = true;
		}
		break;

	case 7:		//Shuffle
		if (!pulsantePremuto){
			pulsantePremuto = !pulsantePremuto;
			//creare un metodo per lo shuffle
		}
		break;

	case 8:		//Reset
		if (!pulsantePremuto){
			pulsantePremuto = !pulsantePremuto;
			inizializzaCubo();
			mosseEffettuate.clear();
			angolo_asse_x = 0;
			angolo_asse_y = 0;
			angolo_asse_z = 0;
			frecciaPremuta = true;
			pulsantePremuto = !pulsantePremuto;
			glutPostRedisplay();	
		}
		break;
	}
}

bool attivazioneMossa(Mossa mossaCorrente, Point posizioneCuboCorrente){
	bool attivaMovimento = false;
	switch (mossaCorrente.riga_colonna_sezione)
	{
	case 'r':
		puntoRiferimentoRotazione.x = 0.0;
		puntoRiferimentoRotazione.z = 0.0;
		if (mossaCorrente.direzione){
			puntoRiferimentoRotazione.y = 1.0;
		}else {
			puntoRiferimentoRotazione.y = -1.0;
		}
		if (posizioneCuboCorrente.y == mossaCorrente.valore){
			attivaMovimento = true;
		}
		break;
	case 'c':
		puntoRiferimentoRotazione.y = 0.0;
		puntoRiferimentoRotazione.z = 0.0;
		if (mossaCorrente.direzione){
			puntoRiferimentoRotazione.x = 1.0;
		}else {
			puntoRiferimentoRotazione.x = -1.0;
		}
		if (posizioneCuboCorrente.x == mossaCorrente.valore){
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
		if (posizioneCuboCorrente.z == mossaCorrente.valore){
			attivaMovimento = true;
		}
		break;
	}
	return attivaMovimento;

}

void memorizzaMossa(){

	switch (mossaInCorso.riga_colonna_sezione)
	{
	case 'r':
		ruotaRigaRubik(mossaInCorso.valore,mossaInCorso.direzione);
		break;
	case 'c':
		ruotaColonnaRubik(mossaInCorso.valore,mossaInCorso.direzione);
		break;

	case 's':
		ruotaSezioneRubik(mossaInCorso.valore,mossaInCorso.direzione);
		break;
	}
	mosseEffettuate.push_back(mossaInCorso);
}

void keyboard(unsigned char key,int x,int y)
{
	switch(key)
	{
	case 27: //esc
		glutDestroyWindow ( windowsID );
		exit(0);
		break;

	case 'q': // sezione posteriore a sinistra
		if (!pulsantePremuto){
			pulsantePremuto = !pulsantePremuto;
			mossaInCorso.riga_colonna_sezione = 's';
			mossaInCorso.valore = 0.0;
			mossaInCorso.direzione = false;
		}
		break;

	case 'a': //sezione centrale a sinistra
		if (!pulsantePremuto){
			pulsantePremuto = !pulsantePremuto;
			mossaInCorso.riga_colonna_sezione = 's';
			mossaInCorso.valore = 1.0;
			mossaInCorso.direzione = false;
		}
		break;

	case 'z': //sezione frontale a sinistra
		if (!pulsantePremuto){
			pulsantePremuto = !pulsantePremuto;
			mossaInCorso.riga_colonna_sezione = 's';
			mossaInCorso.valore = 2.0;
			mossaInCorso.direzione = false;
		}
		break;

	case 'w': //sezione posteriore a destra
		if (!pulsantePremuto){
			pulsantePremuto = !pulsantePremuto;
			mossaInCorso.riga_colonna_sezione = 's';
			mossaInCorso.valore = 0.0;
			mossaInCorso.direzione = true;
		}
		break;

	case 's': //sezione centrale a destra
		if (!pulsantePremuto){
			pulsantePremuto = !pulsantePremuto;
			mossaInCorso.riga_colonna_sezione = 's';
			mossaInCorso.valore = 1.0;
			mossaInCorso.direzione = true;
		}
		break;

	case 'x': //sezione frontale a destra
		if (!pulsantePremuto){
			pulsantePremuto = !pulsantePremuto;
			mossaInCorso.riga_colonna_sezione = 's';
			mossaInCorso.valore = 2.0;
			mossaInCorso.direzione = true;
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
			if (angolo_asse_y == -90 || angolo_asse_y == 270)
			{
				angolo_asse_z += 90;
			} else if (angolo_asse_y == 90 || angolo_asse_y == -270) {
				angolo_asse_z -= 90;
			} else {
				angolo_asse_x -= 90;
			}
		}

		if(key == GLUT_KEY_DOWN)
		{

			if (angolo_asse_y == -90 || angolo_asse_y == 270)
			{
				angolo_asse_z -= 90;
			} else if (angolo_asse_y == 90 || angolo_asse_y == -270) {
				angolo_asse_z += 90;
			} else {
				angolo_asse_x += 90;
			}
		}

		if(key == GLUT_KEY_LEFT)
		{
			if (angolo_asse_x == -90 || angolo_asse_x == 270)
			{
				if (angolo_asse_y == -180 || angolo_asse_y == 180)
				{
					angolo_asse_z -= 90;
				} else {
					angolo_asse_z += 90;
				}
			} else if (angolo_asse_x == 90 || angolo_asse_x == -270) {
				if (angolo_asse_y == -180 || angolo_asse_y == 180) {
					angolo_asse_z += 90;
				} else {
					angolo_asse_z -= 90;
				}
			} else if (angolo_asse_x == 180 || angolo_asse_x == -180) {
				angolo_asse_y -= 90;
			} else {
				angolo_asse_y += 90;
			}
		}

		if(key == GLUT_KEY_RIGHT)
		{
			if (angolo_asse_x == -90 || angolo_asse_x == 270)
			{
				if (angolo_asse_y == -180 || angolo_asse_y == 180)
				{
					angolo_asse_z += 90;
				} else {
					angolo_asse_z -= 90;
				}
			} else if (angolo_asse_x == 90 || angolo_asse_x == -270) {
				if (angolo_asse_y == -180 || angolo_asse_y == 180) {
					angolo_asse_z -= 90;
				} else {
					angolo_asse_z += 90;
				}
			} else if (angolo_asse_x == 180 || angolo_asse_x == -180) {
				angolo_asse_y += 90;
			} else {
				angolo_asse_y -= 90;
			}
		}

		glutPostRedisplay();
	}
}

void reshape(GLsizei width, GLsizei height) // GLsizei for non-negative integer
{ 
	// Compute aspect ratio of the new window
	if (height == 0) {
		height = 1;
	}// To prevent divide by 0
	GLfloat aspect = (GLfloat)width / (GLfloat)height;
	// Set the aspect ratio of the clipping volume to match the viewport
	glMatrixMode(GL_PROJECTION); // To operate on the Projection matrix
	glLoadIdentity(); // Reset
	// Enable perspective projection with fovy, aspect, zNear and zFar
	gluPerspective(60.0f, aspect, 1.0f, 1000.0f);

	//metodo per non fare modificare la grandezza della finestra
	if(width != larghezza || height != altezza)
	{
		glutReshapeWindow(larghezza, altezza);
	}
}

void init()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glEnable(GL_CULL_FACE);
	glShadeModel(GL_SMOOTH);
	/* colore di sfondo nero */
	glClearColor(0.0, 0.0, 0.0, 1.0);

	//carico le texture delle immagini bitmap
	white_textureId = caricaTexture(WHITE);
	red_textureId = caricaTexture(RED);
	blue_textureId = caricaTexture(BLUE);
	green_textureId = caricaTexture(GREEN);
	yellow_textureId = caricaTexture(YELLOW);
	orange_textureId = caricaTexture(ORANGE);

}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(-4.0f, 5.0f, 8.0f, 
		0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f);

	disegnaSuolo();

	disegnaTesto(-4, 4.9, "Numero di mosse effettuate: " + int2str(mosseEffettuate.size()));

	//per far ruotare il cubo nella sua interezza
	glRotatef(valorePrecX, rotate_x.x, rotate_x.y, rotate_x.z);
	glRotatef(valorePrecY, rotate_y.x, rotate_y.y, rotate_y.z); 
	glRotatef(valorePrecZ, rotate_z.x, rotate_z.y, rotate_z.z);

	glLineWidth(spessoreAssi);
	disegnaAssi(2.5);

	for (int x = 0; x < 3;x++){
		for (int y = 0; y < 3;y++){
			for (int z = 0; z < 3;z++){
				glPushMatrix();
				Point posizioneCuboCorrente = {x,y,z};	
				if (pulsantePremuto){
					if (attivazioneMossa(mossaInCorso,posizioneCuboCorrente)){
						glRotatef(angolo_rotazione, puntoRiferimentoRotazione.x, puntoRiferimentoRotazione.y, puntoRiferimentoRotazione.z);
					}
				}
				glTranslatef(posizioneCuboCorrente.x-1,posizioneCuboCorrente.y-1,posizioneCuboCorrente.z-1);
				cuboSingolo(cuboRubik[x][y][z].colorifacce);
				glPopMatrix();
			}
		}
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
			cout << "Il valore di x e' : " << angolo_asse_x << endl;
			cout << "Il valore di y e' : " << angolo_asse_y << endl;
			cout << "Il valore di z e' : " << angolo_asse_z << endl << endl;
			frecciaPremuta = false;
			if (angolo_asse_x == 360 || angolo_asse_x == -360)
			{
				valorePrecX = 0;
				angolo_asse_x = 0;
			}
			if (angolo_asse_y == 360 || angolo_asse_y == -360)
			{
				valorePrecY = 0;
				angolo_asse_y = 0;
			}
			if (angolo_asse_z == 360 || angolo_asse_z == -360)
			{
				valorePrecZ = 0;
				angolo_asse_z = 0;
			}
		}
	}
	if (pulsantePremuto){

		if (angolo_rotazione < gradiRotazioneMossa) {
			angolo_rotazione += 2.0; //controllare velocità rotazione
		}else{
			angolo_rotazione = 0;
			pulsantePremuto = false;
			memorizzaMossa();
		}
	}

	glutSetWindow(windowsID);
	glutPostRedisplay();
	glutTimerFunc(17, timer, 0);

}

void creaPannelloGlui()
{
	//nascondere quei valori con delle variabili
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

	panel = glui -> add_panel("Colori assi", true);
	radio_group = glui -> add_radiogroup_to_panel(panel, &coloreAssi);
	glui -> add_radiobutton_to_group(radio_group, "Nero");
	glui -> add_radiobutton_to_group(radio_group, "Bianco");
	glui -> add_radiobutton_to_group(radio_group, "Rosso");
	glui -> add_radiobutton_to_group(radio_group, "Verde");
	glui -> add_radiobutton_to_group(radio_group, "Blu");
	glui -> add_radiobutton_to_group(radio_group, "Giallo");

	glui -> set_main_gfx_window(windowsID);
}

void centraFinestraDesktop()
{
	window_x = (glutGet (GLUT_SCREEN_WIDTH) - larghezza)/2 + spostamentoFinestra;
	window_y = (glutGet (GLUT_SCREEN_HEIGHT) - altezza)/2;
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
	inizializzaCubo();
	init();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutTimerFunc(17, timer, 0);
	glutSpecialFunc(specialKeyboard);

	creaPannelloGlui();

	glutMainLoop();
}