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
#include <time.h>
#include <tchar.h>

#include "imageloader.h"

#define spostamentoFinestra 400
#define spessoreAssi 3

// Nasconde la console
#pragma comment(linker,"/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

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
	Colore colorifacce[6];
} Cubo;

typedef struct{
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
vector<Mossa> mosseMischiate;
vector<Mossa> mosseAnnullate;
bool mossaAnnullata = false;
int contatoreMosse = 0;
int gradiRotazioneMossa = 90;
float angolo_rotazione = 0;
Point puntoRiferimentoRotazione = {0.0,0.0,0.0};
bool risolvi = false;
bool vittoria = false;
int timerApparizione = 0;
bool mischiaCubo = false;
int numeroMosseMischiate = 30;

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
GLUI_Spinner *spinner;

//contengono gli id delle texture caricate ad avvio programma nel init()
GLuint ID_Texture_Bianco;
GLuint ID_Texture_Rosso;
GLuint ID_Texture_Blu;
GLuint ID_Texture_Verde;
GLuint ID_Texture_Giallo;
GLuint ID_Texture_Arancione;

//frazione della lunghezza da usare come scaler dei caratteri degli assi
const float LENFRAC = 0.10f;

//frazione della lunghezza da usare come locazione di partenza dei caratteri degli assi
const float BASEFRAC = 1.10f;

// Il carattere 'X':
float xx[4] = {0.0f, 1.0f, 0.0f, 1.0f};		//coordinate x dei punti per il carattere x
float xy[4] = {-0.5f, 0.5f, 0.5f, -0.5f};	//coordinate y dei punti per il carattere x
int ordine_Punti_X[4] = {0, 1, -2, 3};		//ordine di prelevamento dei punti per il carattere x

// Il carattere 'Y':
float yx[4] = {0.0f, 0.0f, -0.5f, 0.5f};	//coordinate x dei punti per il carattere y
float yy[4] = {0.0f, 0.6f, 1.0f, 1.0f};		//coordinate y dei punti per il carattere y
int ordine_Punti_Y[5] = {0, 1, 2, -1, 3};	//ordine di prelevamento dei punti per il carattere y

// Il carattere 'Z':
float zx[6] = {1.0f, 0.0f, 1.0f, 0.0f, 0.10f, 0.90f};	//coordinate x dei punti per il carattere z
float zy[6] = {0.5f, 0.5f, -0.5f, -0.5f, 0.0f, 0.0f};	//coordinate y dei punti per il carattere z
int ordine_Punti_Z[6] = {0, 1, 2, 3, -4, 5};			//ordine di prelevamento dei punti per il carattere z


//colore di base degli assi
int coloreAssi = 0;

// Le definizioni dei colori degli assi
// L'ordine deve rispecchiare l'ordine dei radiobutton
const GLfloat Colors[6][3] = 
{
	{0.0, 0.0, 0.0},	//nero
	{1.0, 1.0, 1.0},	//bianco
	{1.0, 0.0, 0.0},	//rosso
	{0.0, 1.0, 0.0},	//verde
	{0.0, 0.0, 1.0},	//blu
	{1.0, 1.0, 0.0},	//giallo
};

GLuint caricaTexture(Colore colore);
GLuint selezionaTextureCaricata(Colore colore);
string int2str(int x);
char randomChar(char r, char c, char s);
bool randomBool();
int randomInt(int primo, int secondo , int terzo);
void disegnaTestoBitmap(float x, float y, string text);
void disegnaAssi(float lunghezza);
void disegnaSuolo();
void cuboSingolo(Colore colori[6]);
void cambiaColoreCuboColonna(Colore coloriCubo[6],bool direzione);
void cambiaColoreCuboRiga(Colore coloriCubo[6],bool direzione);
void cambiaColoreCuboSezione(Colore coloriCubo[6],bool direzione);
void inizializzaCubo();
void mischiaRubik();
void ruotaColonnaRubik(int x, bool direzione);
void ruotaRigaRubik(int y,bool direzione);
void ruotaSezioneRubik(int z,bool direzione);
bool controllaVittoria();
void mossaPrecedente();
void mossaSuccessiva();
void memorizzaMossa(bool effettuatoOMischiato);
bool attivazioneMossa(Mossa mossaCorrente, Point posizioneCuboCorrente);
void gestioneBottoni(int opzione);
void keyboard(unsigned char key,int x,int y);
void specialKeyboard(int key, int x, int y);
void reshape(GLsizei width, GLsizei height);
void init();
void display();
void timer(int value);
void creaPannelloGlui();
void centraFinestraDesktop();
void main(int argc,char** argv);

//Carica il file, lo converte in texture e ritorna l'id della texture in base all'id del colore immesso
GLuint caricaTexture(Colore colore) {

	Image* image = NULL;
	GLuint textureId;
	switch (colore){
	case WHITE:
		image = loadBMP("white.bmp");
		break;
	case YELLOW:
		image = loadBMP("yellow.bmp");
		break;
	case BLUE:
		image = loadBMP("blue.bmp");
		break;
	case GREEN:
		image = loadBMP("green.bmp");
		break;

	case ORANGE:
		image = loadBMP("orange.bmp");
		break;
	case RED:
		image = loadBMP("red.bmp");
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
	return textureId;
}

//restituisce l'id della texture in base al colore cercato
GLuint selezionaTextureCaricata(Colore colore){
	GLuint textureId;

	switch (colore){
	case WHITE:
		textureId = ID_Texture_Bianco;
		break;
	case YELLOW:
		textureId = ID_Texture_Giallo;
		break;
	case BLUE:
		textureId = ID_Texture_Blu;
		break;
	case GREEN:
		textureId = ID_Texture_Verde;
		break;

	case ORANGE:
		textureId = ID_Texture_Arancione;
		break;
	case RED:
		textureId = ID_Texture_Rosso;
		break;
	}

	return textureId;
}

//converte un intero in stringa
string int2str(int x) 
{
	stringstream ss;
	ss << x;
	return ss.str( );
}

//restituisce un char random tra r,c ed s
char randomChar(char r, char c, char s) {
	int random = rand()%3;

	if (random == 0) {
		return r;
	} else if (random == 1) {
		return c;
	} else {
		return s;
	}
}

//restituisce un boolean random
bool randomBool() {
	int r = rand()%2;

	if (r == 0) {
		return false;
	} else {
		return true;
	}
}

//restituisce un random int tra primo, secondo e terzo
int randomInt(int primo, int secondo , int terzo) {
	int r = rand()%3;

	if (r == 0) {
		return primo;
	} else if (r == 1) {
		return secondo;
	} else {
		return terzo;
	}
}

//per stampare a video una stringa
void disegnaTestoBitmap(float x, float y, string text)
{
	glRasterPos2f(x, y);
	int len = text.length();
	for (int i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
	}
}

//disegna gli assi cartesiani in base al colore scelto ed alla lunghezza desiderata
void disegnaAssi(float lunghezza)
{
	//per fare asse x
	glColor3fv(Colors[coloreAssi]); //preleva il colore scelto
	glBegin(GL_LINE_STRIP);
	glVertex3f(lunghezza, 0, 0);
	glVertex3f(-lunghezza, 0, 0);
	glEnd();

	//per fare asse y
	glBegin(GL_LINE_STRIP);
	glVertex3f(0, lunghezza, 0);
	glVertex3f(0, -lunghezza, 0);
	glEnd();

	//per fare asse z
	glBegin(GL_LINE_STRIP);
	glVertex3f(0, 0, lunghezza);
	glVertex3f(0, 0, -lunghezza);
	glEnd();

	float scalerLettere = LENFRAC * lunghezza;	// grandezza delle lettere
	float base = BASEFRAC * lunghezza;	// distanza dove iniziare a disegnare

	//per fare le due x
	for (int doppio = 0; doppio < 2; doppio++){
		glPushMatrix();
		if (doppio == 1){	//caso utilizzato per disegnare l'opposto
			glRotatef(180, 0.0, 1.0, 0.0);
		}
		glBegin( GL_LINE_STRIP );
		for( int i = 0; i < 4; i++ )	//ciclo della grandezza dell'ordine di prelevamento dei punti
		{
			int j = ordine_Punti_X[i]; 
			if( j < 0 ) //caso in cui deve smettere di disegnare perch?? i punti non sono collegati
			{
				glEnd(); //interrompe il disegno
				glBegin( GL_LINE_STRIP );
				j = -j;	//inverte il numero per utilizzarlo come indice del vettore
			}
			glVertex3f( base + scalerLettere*xx[j], scalerLettere*xy[j], 0.0 );
		}
		glEnd( );
		glPopMatrix();
	}

	//per fare le due y
	for (int doppio = 0; doppio < 2; doppio++){
		glPushMatrix();
		if (doppio == 1){	 //caso utilizzato per disegnare l'opposto
			glRotatef(180, 0.0, 0.0, 1.0);
		}
		glBegin( GL_LINE_STRIP );
		for( int i = 0; i < 5; i++ ) //ciclo della grandezza dell'ordine di prelevamento dei punti
		{
			int j = ordine_Punti_Y[i];
			if( j < 0 ) //caso in cui deve smettere di disegnare perch?? i punti non sono collegati
			{
				glEnd(); //interrompe il disegno
				glBegin( GL_LINE_STRIP );
				j = -j; //inverte il numero per utilizzarlo come indice del vettore
			}
			glVertex3f(scalerLettere*yx[j], base + scalerLettere*yy[j], 0.0 );
		}
		glEnd( );
		glPopMatrix();
	}

	//per fare le due z
	for (int doppio = 0; doppio < 2; doppio++){
		glPushMatrix();
		if (doppio == 1){	 //caso utilizzato per disegnare l'opposto
			glRotatef(180, 0.0, 1.0, 0.0);
		}
		glBegin( GL_LINE_STRIP );
		for( int i = 0; i < 6; i++ ) //ciclo della grandezza dell'ordine di prelevamento dei punti
		{
			int j = ordine_Punti_Z[i];
			if( j < 0 )
			{
				glEnd(); //interrompe il disegno
				glBegin( GL_LINE_STRIP );
				j = -j; //inverte il numero per utilizzarlo come indice del vettore
			}
			glVertex3f( 0.0, scalerLettere*zy[j], base + scalerLettere*zx[j] );
		}
		glEnd();
		glPopMatrix();
	}

	glColor3f(1.0, 1.0, 1.0);
}

//disegna il suolo (fisso) della della scena
void disegnaSuolo()
{ 
	glPushMatrix();
	glBegin(GL_QUADS);
	glColor3ub(0, 0, 0); //colore iniziale sfumatura (lontano)
	glVertex3f(600.0f, -50.0f, -200.0f);
	glVertex3f(-600.0f, -50.0f, -650.0f);
	glColor3ub(200, 200, 200); //colore finale sfumatura (vicino)
	glVertex3f(-600.0f, -50.0f, 30.0f);
	glVertex3f(600.0f, -50.0f, 30.0f);
	glEnd();
	glPopMatrix();
}

//disegna un singolo cubo sullo schermo viene passato il vettore di colori del cubo da disegnare
void cuboSingolo(Colore colori[6])
{
	GLuint texture_faccia_anteriore = selezionaTextureCaricata(colori[0]);		//texture faccia anteriore
	GLuint texture_faccia_posteriore = selezionaTextureCaricata(colori[1]);		//texture faccia posteriore
	GLuint texture_faccia_destra = selezionaTextureCaricata(colori[2]);			//texture faccia destra
	GLuint texture_faccia_sinistra = selezionaTextureCaricata(colori[3]);		//texture faccia sinistra
	GLuint texture_faccia_superiore = selezionaTextureCaricata(colori[4]);		//texture faccia superiore
	GLuint texture_faccia_inferiore = selezionaTextureCaricata(colori[5]);		//texture faccia inferiore

	//Faccia Anteriore
	glEnable(GL_TEXTURE_2D); //per utlizzare la texture
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

//cambia il colore del cubo considerando che si muove per colonna
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

//cambia il colore del cubo considerando che si muove per riga
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

//cambia il colore del cubo considerando che si muove per sezione
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

//crea il cubo
void inizializzaCubo(){
	
	//svuota tutte le mosse effettuate
	mosseEffettuate.clear();
	mosseAnnullate.clear();
	mosseMischiate.clear();
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

				cuboRubik[x][y][z] = nuovoCubo;
			}
		}
	}

}

//mischia il cubo utilizzando metodi random per creare le mosse che mischiano
void mischiaRubik() {
	srand(time(NULL));
	for (int i = 0; i < numeroMosseMischiate; i++)
	{
		mossaInCorso.riga_colonna_sezione = randomChar('r', 'c', 's');
		mossaInCorso.valore = randomInt(0, 1, 2);
		mossaInCorso.direzione = randomBool();
		memorizzaMossa(false);
	}
}

//ruota la colonna x con direzione bool ruotando i colori di ogni cubo e spostandoli di posizione nel cubo visualizzato
void ruotaColonnaRubik(int x, bool direzione){
	if (direzione){

		Cubo temp = cuboRubik[x][0][0];

		cambiaColoreCuboColonna(cuboRubik[x][0][2].colorifacce,direzione); //salva i colori del cubo gi?? ruotati nel senso corretto in coloriInModifica
		cuboRubik[x][0][2].colorifacce[0] = coloriInModifica[0];
		cuboRubik[x][0][2].colorifacce[1] = coloriInModifica[1];
		cuboRubik[x][0][2].colorifacce[2] = coloriInModifica[2];
		cuboRubik[x][0][2].colorifacce[3] = coloriInModifica[3];
		cuboRubik[x][0][2].colorifacce[4] = coloriInModifica[4];
		cuboRubik[x][0][2].colorifacce[5] = coloriInModifica[5];

		cuboRubik[x][0][0] = cuboRubik[x][0][2]; //sposta la posizione dentro il cubo

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

		cuboRubik[x][0][0] = cuboRubik[x][2][0];

		cambiaColoreCuboColonna(cuboRubik[x][2][2].colorifacce,direzione);
		cuboRubik[x][2][2].colorifacce[0] = coloriInModifica[0];
		cuboRubik[x][2][2].colorifacce[1] = coloriInModifica[1];
		cuboRubik[x][2][2].colorifacce[2] = coloriInModifica[2];
		cuboRubik[x][2][2].colorifacce[3] = coloriInModifica[3];
		cuboRubik[x][2][2].colorifacce[4] = coloriInModifica[4];
		cuboRubik[x][2][2].colorifacce[5] = coloriInModifica[5];

		cuboRubik[x][2][0] = cuboRubik[x][2][2];

		cambiaColoreCuboColonna(cuboRubik[x][0][2].colorifacce,direzione);
		cuboRubik[x][0][2].colorifacce[0] = coloriInModifica[0];
		cuboRubik[x][0][2].colorifacce[1] = coloriInModifica[1];
		cuboRubik[x][0][2].colorifacce[2] = coloriInModifica[2];
		cuboRubik[x][0][2].colorifacce[3] = coloriInModifica[3];
		cuboRubik[x][0][2].colorifacce[4] = coloriInModifica[4];
		cuboRubik[x][0][2].colorifacce[5] = coloriInModifica[5];

		cuboRubik[x][2][2] = cuboRubik[x][0][2];

		cambiaColoreCuboColonna(temp.colorifacce,direzione);
		temp.colorifacce[0] = coloriInModifica[0];
		temp.colorifacce[1] = coloriInModifica[1];
		temp.colorifacce[2] = coloriInModifica[2];
		temp.colorifacce[3] = coloriInModifica[3];
		temp.colorifacce[4] = coloriInModifica[4];
		temp.colorifacce[5] = coloriInModifica[5];

		cuboRubik[x][0][2] = temp;

		temp = cuboRubik[x][0][1];

		cambiaColoreCuboColonna(cuboRubik[x][1][0].colorifacce,direzione);
		cuboRubik[x][1][0].colorifacce[0] = coloriInModifica[0];
		cuboRubik[x][1][0].colorifacce[1] = coloriInModifica[1];
		cuboRubik[x][1][0].colorifacce[2] = coloriInModifica[2];
		cuboRubik[x][1][0].colorifacce[3] = coloriInModifica[3];
		cuboRubik[x][1][0].colorifacce[4] = coloriInModifica[4];
		cuboRubik[x][1][0].colorifacce[5] = coloriInModifica[5];

		cuboRubik[x][0][1] = cuboRubik[x][1][0];

		cambiaColoreCuboColonna(cuboRubik[x][2][1].colorifacce,direzione);
		cuboRubik[x][2][1].colorifacce[0] = coloriInModifica[0];
		cuboRubik[x][2][1].colorifacce[1] = coloriInModifica[1];
		cuboRubik[x][2][1].colorifacce[2] = coloriInModifica[2];
		cuboRubik[x][2][1].colorifacce[3] = coloriInModifica[3];
		cuboRubik[x][2][1].colorifacce[4] = coloriInModifica[4];
		cuboRubik[x][2][1].colorifacce[5] = coloriInModifica[5];

		cuboRubik[x][1][0] = cuboRubik[x][2][1];

		cambiaColoreCuboColonna(cuboRubik[x][1][2].colorifacce,direzione);
		cuboRubik[x][1][2].colorifacce[0] = coloriInModifica[0];
		cuboRubik[x][1][2].colorifacce[1] = coloriInModifica[1];
		cuboRubik[x][1][2].colorifacce[2] = coloriInModifica[2];
		cuboRubik[x][1][2].colorifacce[3] = coloriInModifica[3];
		cuboRubik[x][1][2].colorifacce[4] = coloriInModifica[4];
		cuboRubik[x][1][2].colorifacce[5] = coloriInModifica[5];

		cuboRubik[x][2][1] = cuboRubik[x][1][2];

		cambiaColoreCuboColonna(temp.colorifacce,direzione);
		temp.colorifacce[0] = coloriInModifica[0];
		temp.colorifacce[1] = coloriInModifica[1];
		temp.colorifacce[2] = coloriInModifica[2];
		temp.colorifacce[3] = coloriInModifica[3];
		temp.colorifacce[4] = coloriInModifica[4];
		temp.colorifacce[5] = coloriInModifica[5];

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

//ruota la riga x con direzione bool ruotando i colori di ogni cubo e spostandoli di posizione nel cubo visualizzato
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

//ruota la sezione x con direzione bool ruotando i colori di ogni cubo e spostandoli di posizione nel cubo visualizzato
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

//controlla che tutte le faccie del cubo abbiano lo stesso colore
bool controllaVittoria(){

	bool vittoriaParziale = true;

	Colore facciaSinistra = cuboRubik[0][0][0].colorifacce[3];		//colore visualizzato di un cubo della faccia sinistra
	Colore facciaDestra = cuboRubik[2][0][0].colorifacce[2];		//colore visualizzato di un cubo della faccia destra
	Colore facciaFrontale = cuboRubik[0][0][2].colorifacce[0];		//colore visualizzato di un cubo della faccia frontale
	Colore facciaPosteriore = cuboRubik[0][0][0].colorifacce[1];	//colore visualizzato di un cubo della faccia posteriore
	Colore facciaSuperiore = cuboRubik[0][2][0].colorifacce[4];		//colore visualizzato di un cubo della faccia superiore
	Colore facciaInferiore = cuboRubik[0][0][0].colorifacce[5];		//colore visualizzato di un cubo della faccia inferiore

	//controllo faccia sinistra quindi x = 0;
	for (int y = 0; y < 3 && vittoriaParziale; y++){
		for (int z = 0; z < 3 && vittoriaParziale; z++){
			if (cuboRubik[0][y][z].colorifacce[3] != facciaSinistra){
				//se entra qui dentro allora esiste un cubo con una faccia non dello stesso colore
				vittoriaParziale = false;
			}
		}
	}

	//controllo faccia destra quindi x = 2
	for (int y = 0; y < 3 && vittoriaParziale; y++){
		for (int z = 0; z < 3 && vittoriaParziale; z++){
			if (cuboRubik[2][y][z].colorifacce[2] != facciaDestra){
				vittoriaParziale = false;
			}
		}
	}

	//controllo faccia frontale quindi z = 2;
	for (int x = 0; x < 3 && vittoriaParziale; x++){
		for (int y = 0; y < 3 && vittoriaParziale; y++){
			if (cuboRubik[x][y][2].colorifacce[0] != facciaFrontale){
				vittoriaParziale = false;
			}
		}
	}

	//controllo faccia posteriore quindi z = 0;
	for (int x = 0; x < 3 && vittoriaParziale; x++){
		for (int y = 0; y < 3 && vittoriaParziale; y++){
			if (cuboRubik[x][y][0].colorifacce[1] != facciaPosteriore){
				vittoriaParziale = false;
			}
		}
	}

	//controllo faccia superiore quindi y = 2;
	for (int x = 0; x < 3 && vittoriaParziale; x++){
		for (int z = 0; z < 3 && vittoriaParziale; z++){
			if (cuboRubik[x][2][z].colorifacce[4] != facciaSuperiore){
				vittoriaParziale = false;
			}
		}
	}

	//controllo faccia inferiore quindi y = 0;
	for (int x = 0; x < 3 && vittoriaParziale; x++){
		for (int z = 0; z < 3 && vittoriaParziale; z++){
			if (cuboRubik[x][0][z].colorifacce[5] != facciaInferiore){
				vittoriaParziale = false;
			}
		}
	}

	return vittoriaParziale;
}

//annulla l'ultima mossa effettuata
void mossaPrecedente() {
		if (!mosseEffettuate.empty()) {
			mossaInCorso.riga_colonna_sezione = mosseEffettuate.back().riga_colonna_sezione;
			mossaInCorso.valore = mosseEffettuate.back().valore;
			mossaInCorso.direzione = !mosseEffettuate.back().direzione;
			mosseEffettuate.erase(mosseEffettuate.begin() + mosseEffettuate.size() - 1);
			mosseAnnullate.push_back(mossaInCorso);
			mossaAnnullata = true;
			pulsantePremuto = true;
			if (!vittoria){
				contatoreMosse++;
			}
			PlaySound(TEXT("movimento.wav"), NULL, SND_FILENAME | SND_ASYNC); 
		}
}

//ripristina l'utlima mossa annullata
void mossaSuccessiva() {
	//controlla se esistono mosse annullate
	if (!mosseAnnullate.empty()) {
		mossaInCorso.riga_colonna_sezione = mosseAnnullate.back().riga_colonna_sezione;
		mossaInCorso.valore = mosseAnnullate.back().valore;
		mossaInCorso.direzione = !mosseAnnullate.back().direzione;
		mosseAnnullate.erase(mosseAnnullate.begin() + mosseAnnullate.size() - 1);
		pulsantePremuto = true;
		PlaySound(TEXT("movimento.wav"), NULL, SND_FILENAME | SND_ASYNC); 
	}
}

//applica la mossa nella matrice del cubo e la salva nel vettore delle mosse
void memorizzaMossa(bool effettuatoOMischiato){ //se true la mossa ?? stata fatta dall'utente, altrimenti dal tasto mischia

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

	//se ?? true allora ?? una mossa effettuata
	if (effettuatoOMischiato) {
		if (!mossaAnnullata){
			mosseEffettuate.push_back(mossaInCorso);
			if (controllaVittoria()){
				vittoria = true;
			}
		}else{
			mossaAnnullata = false;
		}
	} else {
		mosseMischiate.push_back(mossaInCorso);
	}

}

//controlla se la mossa corrente va applicata alla visualizzazione del cubo
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
			mosseAnnullate.clear();
			if (!vittoria){
				contatoreMosse++;
			}
			PlaySound(TEXT("movimento.wav"), NULL, SND_FILENAME | SND_ASYNC); 
		}
		break;

	case -2:	//Seconda riga a sinistra
		if (!pulsantePremuto){
			pulsantePremuto = !pulsantePremuto;
			mossaInCorso.riga_colonna_sezione = 'r';
			mossaInCorso.valore = 1.0;
			mossaInCorso.direzione = false;
			mosseAnnullate.clear();
			if (!vittoria){
				contatoreMosse++;
			}
			PlaySound(TEXT("movimento.wav"), NULL, SND_FILENAME | SND_ASYNC); 
		}
		break;

	case -3:	//Terza riga a sinistra
		if (!pulsantePremuto){
			pulsantePremuto = !pulsantePremuto;
			mossaInCorso.riga_colonna_sezione = 'r';
			mossaInCorso.valore = 0.0;
			mossaInCorso.direzione = false;
			mosseAnnullate.clear();
			if (!vittoria){
				contatoreMosse++;
			}
			PlaySound(TEXT("movimento.wav"), NULL, SND_FILENAME | SND_ASYNC); 
		}
		break;

	case 1:		//Prima riga a destra
		if (!pulsantePremuto){
			pulsantePremuto = !pulsantePremuto;
			mossaInCorso.riga_colonna_sezione = 'r';
			mossaInCorso.valore = 2.0;
			mossaInCorso.direzione = true;
			mossaInCorso.direzione = 'r';
			mosseAnnullate.clear();
			if (!vittoria){
				contatoreMosse++;
			}
			PlaySound(TEXT("movimento.wav"), NULL, SND_FILENAME | SND_ASYNC); 
		}
		break;

	case 2:		//Seconda riga a destra
		if (!pulsantePremuto){
			pulsantePremuto = !pulsantePremuto;
			mossaInCorso.riga_colonna_sezione = 'r';
			mossaInCorso.valore = 1.0;
			mossaInCorso.direzione = true;
			mosseAnnullate.clear();
			if (!vittoria){
				contatoreMosse++;
			}
			PlaySound(TEXT("movimento.wav"), NULL, SND_FILENAME | SND_ASYNC); 
		}
		break;

	case 3:		//Terza riga a destra 
		if (!pulsantePremuto){
			pulsantePremuto = !pulsantePremuto;
			mossaInCorso.riga_colonna_sezione = 'r';
			mossaInCorso.valore = 0.0;
			mossaInCorso.direzione = true;
			mosseAnnullate.clear();
			if (!vittoria){
				contatoreMosse++;
			}
			PlaySound(TEXT("movimento.wav"), NULL, SND_FILENAME | SND_ASYNC); 
		}
		break;

	case -4:	//Prima colonna s??
		if (!pulsantePremuto){
			pulsantePremuto = !pulsantePremuto;
			mossaInCorso.riga_colonna_sezione = 'c',
			mossaInCorso.valore = 0.0;
			mossaInCorso.direzione = false;
			mosseAnnullate.clear();
			if (!vittoria){
				contatoreMosse++;
			}
			PlaySound(TEXT("movimento.wav"), NULL, SND_FILENAME | SND_ASYNC); 
		}
		break;

	case -5:	//Seconda colonna s??
		if (!pulsantePremuto){
			pulsantePremuto = !pulsantePremuto;
			mossaInCorso.riga_colonna_sezione = 'c';
			mossaInCorso.valore = 1.0;
			mossaInCorso.direzione = false;
			mosseAnnullate.clear();
			if (!vittoria){
				contatoreMosse++;
			}
			PlaySound(TEXT("movimento.wav"), NULL, SND_FILENAME | SND_ASYNC); 
		}
		break;

	case -6:	//Terza colonna s??
		if (!pulsantePremuto){
			pulsantePremuto = !pulsantePremuto;
			mossaInCorso.riga_colonna_sezione = 'c';
			mossaInCorso.valore = 2.0;
			mossaInCorso.direzione = false;
			mosseAnnullate.clear();
			if (!vittoria){
				contatoreMosse++;
			}
			PlaySound(TEXT("movimento.wav"), NULL, SND_FILENAME | SND_ASYNC); 
		}
		break;

	case 4:		//Prima colonna gi??
		if (!pulsantePremuto){
			pulsantePremuto = !pulsantePremuto;
			mossaInCorso.riga_colonna_sezione = 'c';
			mossaInCorso.valore = 0.0;
			mossaInCorso.direzione = true;
			mosseAnnullate.clear();
			if (!vittoria){
				contatoreMosse++;
			}
			PlaySound(TEXT("movimento.wav"), NULL, SND_FILENAME | SND_ASYNC); 
		}
		break;

	case 5:		//Seconda colonna gi??
		if (!pulsantePremuto){
			pulsantePremuto = !pulsantePremuto;
			mossaInCorso.riga_colonna_sezione = 'c';
			mossaInCorso.valore = 1.0;
			mossaInCorso.direzione = true;
			mosseAnnullate.clear();
			if (!vittoria){
				contatoreMosse++;
			}
			PlaySound(TEXT("movimento.wav"), NULL, SND_FILENAME | SND_ASYNC); 
		}
		break;

	case 6:		//Terza colonna gi??
		if (!pulsantePremuto){
			pulsantePremuto = !pulsantePremuto;
			mossaInCorso.riga_colonna_sezione = 'c';
			mossaInCorso.valore = 2.0;
			mossaInCorso.direzione = true;
			mosseAnnullate.clear();
			if (!vittoria){
				contatoreMosse++;
			}
			PlaySound(TEXT("movimento.wav"), NULL, SND_FILENAME | SND_ASYNC); 
		}
		break;

	case 7:		//Mischia
		if (!mischiaCubo && !vittoria){
			inizializzaCubo();
			mischiaCubo = !mischiaCubo;
			mischiaRubik();
		}
		break;

	case 8:		//Reset
		if (!pulsantePremuto){
			pulsantePremuto = !pulsantePremuto;
			inizializzaCubo();
			angolo_asse_x = 0;
			angolo_asse_y = 0;
			angolo_asse_z = 0;
			frecciaPremuta = true;
			vittoria = false;
			mischiaCubo = false;
			contatoreMosse = 0;
			pulsantePremuto = !pulsantePremuto;
			glutPostRedisplay();
		}
		break;
	case 9:
		if (!pulsantePremuto){
			mossaPrecedente();
		}
		break;
	case 10:
		if (!pulsantePremuto){
			mossaSuccessiva();
		}
		break;
	case 11:
		if (!pulsantePremuto && !vittoria){
			risolvi = true;
		}
		break;
	case 12: //caso dell'aumento dello spinner che viene gia gestito automaticamente dalla glui
		break;
	}
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
			mosseAnnullate.clear();
			if (!vittoria){
				contatoreMosse++;
			}
			PlaySound(TEXT("movimento.wav"), NULL, SND_FILENAME | SND_ASYNC); 
		}
		break;

	case 'a': //sezione centrale a sinistra
		if (!pulsantePremuto){
			pulsantePremuto = !pulsantePremuto;
			mossaInCorso.riga_colonna_sezione = 's';
			mossaInCorso.valore = 1.0;
			mossaInCorso.direzione = false;
			mosseAnnullate.clear();
			if (!vittoria){
				contatoreMosse++;
			}
			PlaySound(TEXT("movimento.wav"), NULL, SND_FILENAME | SND_ASYNC); 
		}
		break;

	case 'z': //sezione frontale a sinistra
		if (!pulsantePremuto){
			pulsantePremuto = !pulsantePremuto;
			mossaInCorso.riga_colonna_sezione = 's';
			mossaInCorso.valore = 2.0;
			mossaInCorso.direzione = false;
			mosseAnnullate.clear();
			if (!vittoria){
				contatoreMosse++;
			}
			PlaySound(TEXT("movimento.wav"), NULL, SND_FILENAME | SND_ASYNC); 
		}
		break;

	case 'w': //sezione posteriore a destra
		if (!pulsantePremuto){
			pulsantePremuto = !pulsantePremuto;
			mossaInCorso.riga_colonna_sezione = 's';
			mossaInCorso.valore = 0.0;
			mossaInCorso.direzione = true;
			mosseAnnullate.clear();
			if (!vittoria){
				contatoreMosse++;
			}
			PlaySound(TEXT("movimento.wav"), NULL, SND_FILENAME | SND_ASYNC); 
		}
		break;

	case 's': //sezione centrale a destra
		if (!pulsantePremuto){
			pulsantePremuto = !pulsantePremuto;
			mossaInCorso.riga_colonna_sezione = 's';
			mossaInCorso.valore = 1.0;
			mossaInCorso.direzione = true;
			mosseAnnullate.clear();
			if (!vittoria){
				contatoreMosse++;
			}
			PlaySound(TEXT("movimento.wav"), NULL, SND_FILENAME | SND_ASYNC); 
		}
		break;

	case 'x': //sezione frontale a destra
		if (!pulsantePremuto){
			pulsantePremuto = !pulsantePremuto;
			mossaInCorso.riga_colonna_sezione = 's';
			mossaInCorso.valore = 2.0;
			mossaInCorso.direzione = true;
			mosseAnnullate.clear();
			if (!vittoria){
				contatoreMosse++;
			}
			PlaySound(TEXT("movimento.wav"), NULL, SND_FILENAME | SND_ASYNC); 
		}
		break;
	}
}

//utiizzato per prelevare l'uso delle freccie e ruotare il cubo
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

void reshape(GLsizei width, GLsizei height) // GLsizei per interi non negativi
{ 
	//calcola l'aspect ratio della nuova finestra
	if (height == 0) {
		height = 1;
	}//per evitare di dividere per 0
	GLfloat aspect = (GLfloat)width / (GLfloat)height;
	//imposta l'aspect ratio del volume di ritaglio per abbinare la viewport
	glMatrixMode(GL_PROJECTION); //per operare con la matrice Projection
	glLoadIdentity(); // Reset
	//abilita la proiezione prospettica
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
	//colore di sfondo nero
	glClearColor(0.0, 0.0, 0.0, 1.0);

	//carico le texture delle immagini bitmap
	ID_Texture_Bianco = caricaTexture(WHITE);
	ID_Texture_Rosso = caricaTexture(RED);
	ID_Texture_Blu = caricaTexture(BLUE);
	ID_Texture_Verde = caricaTexture(GREEN);
	ID_Texture_Giallo = caricaTexture(YELLOW);
	ID_Texture_Arancione = caricaTexture(ORANGE);

}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//imposta il punto di vista
	gluLookAt(-4.0f, 5.0f, 8.0f, 
		0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f);

	disegnaSuolo();

	disegnaTestoBitmap(-4, 4.9, "Numero di mosse effettuate: " + int2str(contatoreMosse));
	if (vittoria && timerApparizione < 0){
		glColor3f(0.0,0.4,0.0);
		disegnaTestoBitmap(-0.7,3.9,"HAI VINTO!");
		glColor3f(1.0,1.0,1.0);
	}

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

	//timer che regola l'apparizione della scritta HAI VINTO! nei 20 cicli positivi (da 20 a 0 compare e nei 20 cicli negativi da -1 a -20 non compare)
	timerApparizione--;
	if (timerApparizione == -20){
		timerApparizione = 20;
	}

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

	//utilzzata per animare la rotazione di una qualsiasi mossa
	if (pulsantePremuto){

		if (angolo_rotazione < gradiRotazioneMossa) {
			angolo_rotazione += 2.0;
		}else{
			//finita la rotazione memorizziamo la mossa sia spostando di posizione i cubi sia salvando la mossa dentro il vettore
			angolo_rotazione = 0;
			pulsantePremuto = false;
			memorizzaMossa(true);
		}
	}

	//caso in cui venga attivata la risoluzione automatica del cubo
	if (risolvi && angolo_rotazione == 0){
		mosseAnnullate.clear();
		if (!mosseEffettuate.empty()){
			mossaInCorso = mosseEffettuate.back();
			mossaInCorso.direzione = !mosseEffettuate.back().direzione;
			mosseEffettuate.erase(mosseEffettuate.begin() + mosseEffettuate.size() - 1);
			pulsantePremuto = true;
			mossaAnnullata = true;
			PlaySound(TEXT("movimento.wav"), NULL, SND_FILENAME | SND_ASYNC); 
		}else if (!mosseMischiate.empty()){
			mossaInCorso = mosseMischiate.back();
			mossaInCorso.direzione = !mosseMischiate.back().direzione;
			mosseMischiate.erase(mosseMischiate.begin() + mosseMischiate.size() - 1);
			pulsantePremuto = true;
			mossaAnnullata = true;
			PlaySound(TEXT("movimento.wav"), NULL, SND_FILENAME | SND_ASYNC); 
		}else{
			risolvi = false;
			contatoreMosse = 0;
		}
	}

	glutSetWindow(windowsID);
	glutPostRedisplay();
	//ogni 17 millisecondi
	glutTimerFunc(17, timer, 0);

}

//crea il pannello con i comandi del giocatore
void creaPannelloGlui()
{
	glui = GLUI_Master.create_glui("Comandi Giocatore", GLUI_SUBWINDOW_TOP, (glutGet(GLUT_SCREEN_WIDTH)-larghezza-spostamentoFinestra)/2, (glutGet(GLUT_SCREEN_HEIGHT)-altezza)/2);

	panel = glui -> add_panel("Movimenti cubo", GLUI_PANEL_EMBOSSED);
	glui -> add_statictext_to_panel(panel,"");
	glui -> add_button_to_panel(panel, "<", -1, gestioneBottoni);	//Prima riga a sinistra
	glui -> add_button_to_panel(panel, "<", -2, gestioneBottoni);	//Seconda riga a sinistra
	glui -> add_button_to_panel(panel, "<", -3, gestioneBottoni);	//Terza riga a sinistra
	glui -> add_statictext_to_panel(panel,"");
	glui->add_column_to_panel( panel, false );
	glui -> add_button_to_panel(panel, "^", -4, gestioneBottoni);	//Prima colonna s??
	glui -> add_statictext_to_panel(panel,"sezione posteriore");
	glui -> add_statictext_to_panel(panel,"sezione centrale");
	glui -> add_statictext_to_panel(panel,"sezione frontale");
	glui -> add_button_to_panel(panel, "v", 4, gestioneBottoni);	//Prima colonna gi??
	glui -> add_statictext_to_panel(panel,"");
	glui -> add_statictext_to_panel(panel,"Frecce direzionali");
	glui->add_column_to_panel( panel, false );
	glui -> add_button_to_panel(panel, "^", -5, gestioneBottoni);	//Seconda colonna s??
	glui -> add_statictext_to_panel(panel,"q = sinistra");
	glui -> add_statictext_to_panel(panel,"a = sinistra");
	glui -> add_statictext_to_panel(panel,"z = sinistra");
	glui -> add_button_to_panel(panel, "v", 5, gestioneBottoni);	//Seconda colonna gi??
	glui -> add_statictext_to_panel(panel,"");
	glui -> add_statictext_to_panel(panel,"= ruota il cubo");
	glui->add_column_to_panel( panel, false );
	glui -> add_button_to_panel(panel, "^", -6, gestioneBottoni);	//Terza colonna s??
	glui -> add_statictext_to_panel(panel,"w = destra");
	glui -> add_statictext_to_panel(panel,"s = destra");
	glui -> add_statictext_to_panel(panel,"x = destra");
	glui -> add_button_to_panel(panel, "v", 6, gestioneBottoni);	//Terza colonna gi??
	glui->add_column_to_panel( panel, false );
	glui -> add_statictext_to_panel(panel,"");
	glui -> add_button_to_panel(panel, ">", 1, gestioneBottoni);	//Prima riga a destra
	glui -> add_button_to_panel(panel, ">", 2, gestioneBottoni);	//Seconda riga a destra
	glui -> add_button_to_panel(panel, ">", 3, gestioneBottoni);	//Terza riga a destra
	glui -> add_statictext_to_panel(panel,"");

	panel = glui->add_panel( "", false );
	spinner = glui ->add_spinner_to_panel(panel,"Mosse:",GLUI_SPINNER_INT,&numeroMosseMischiate,12,gestioneBottoni);
	spinner -> set_int_limits(1,100,GLUI_LIMIT_CLAMP);
	spinner -> set_speed(0.1);
	spinner -> set_int_val(numeroMosseMischiate);
	glui->add_column_to_panel( panel, false );
	glui -> add_button_to_panel(panel, "Mischia", 7, gestioneBottoni);
	
	panel = glui->add_panel( "", false );
	glui->add_button_to_panel( panel, "Reset", 8, gestioneBottoni);
	glui->add_column_to_panel( panel, false );
	glui -> add_button_to_panel(panel, "Precedente", 9, gestioneBottoni);
	glui->add_column_to_panel( panel, false );
	glui -> add_button_to_panel(panel, "Successivo", 10, gestioneBottoni);
	glui->add_column_to_panel( panel, false );
	glui -> add_button_to_panel(panel, "Risolvi", 11, gestioneBottoni);

	panel = glui -> add_panel("Colori assi", true);
	radio_group = glui -> add_radiogroup_to_panel(panel, &coloreAssi);
	glui -> add_radiobutton_to_group(radio_group, "Nero");
	glui -> add_radiobutton_to_group(radio_group, "Bianco");
	glui -> add_radiobutton_to_group(radio_group, "Rosso");
	glui -> add_radiobutton_to_group(radio_group, "Verde");
	glui -> add_radiobutton_to_group(radio_group, "Blu");
	glui -> add_radiobutton_to_group(radio_group, "Giallo");

	panel = glui -> add_panel("", false);
	glui -> add_statictext_to_panel(panel,"");
	glui->add_column_to_panel( panel, false );
	glui -> add_statictext_to_panel(panel,"");
	glui->add_column_to_panel( panel, false );
	glui -> add_statictext_to_panel(panel,"");
	glui->add_column_to_panel( panel, false );
	glui -> add_statictext_to_panel(panel,"");
	glui->add_column_to_panel( panel, false );
	glui->add_button_to_panel( panel, "Esci", 0, gestioneBottoni);

	glui -> set_main_gfx_window(windowsID);
}

//inizializza le variabili utilizzate per centrare le window 
void centraFinestraDesktop(){
	window_x = (glutGet (GLUT_SCREEN_WIDTH) - larghezza)/2 + spostamentoFinestra;
	window_y = (glutGet (GLUT_SCREEN_HEIGHT) - altezza)/2;
}

void main(int argc,char** argv){

	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
	larghezza = 700;
	altezza = 700;
	centraFinestraDesktop();
	glutInitWindowSize(larghezza,altezza);
	glutInitWindowPosition (window_x, window_y);
	windowsID = glutCreateWindow("Cubo Di Rubik");

	//per impostare l'icona nella finestra principale
	HWND hwnd = FindWindow(NULL, _T("Cubo di Rubik") );
	HANDLE icon = LoadImage(GetModuleHandle(NULL), _T("rubik64.ico"), IMAGE_ICON, 64, 64, LR_LOADFROMFILE | LR_COLOR);
	SendMessage(hwnd, (UINT)WM_SETICON, ICON_BIG, (LPARAM)icon);

	inizializzaCubo();
	init();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutTimerFunc(17, timer, 0);
	glutSpecialFunc(specialKeyboard);

	creaPannelloGlui();

	//per impostare l'icona nella finestra dei comandi
	hwnd = FindWindow(NULL, _T("Comandi Giocatore") );
	SendMessage(hwnd, (UINT)WM_SETICON, ICON_BIG, (LPARAM)icon);

	glutMainLoop();
}