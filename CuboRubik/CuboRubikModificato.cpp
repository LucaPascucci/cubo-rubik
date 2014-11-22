//#include <windows.h>
//#include <gl/glut.h>
//#include <math.h>
//
//void drawRubikCube(GLfloat edge);
//
//GLfloat arrowXrot=0.0;
//GLfloat arrowYrot=0.0;
//GLfloat arrowZrot=0.0;
//
//void init(void)
//{ /* Abilita il Culling e il Depth testing...ok */ 
//	glEnable(GL_DEPTH_TEST);
//	glEnable(GL_CULL_FACE);
//
//	/* Imposta lo smooth shading */
//	glShadeModel(GL_SMOOTH);
//
//	/* colore di sfondo nero */
//	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//}
//
//void oscilla()
//{
//	GLfloat offset=0.1;
//	GLfloat maxAngleSize=15.0;
//	static GLfloat angle=90.0;
//	static GLfloat angleOld;
//	GLfloat zRot=sin(angle)*3;
//	GLfloat xRot=cos(angle)*3;
//
//	angleOld=angle+10.0;
//
//	glRotatef(zRot, 0.0, 0.0, 1.0);
//	glRotatef(xRot, 0.0, 1.0, 0.0);
//
//	/* imposta un leggero movimento oscillatorio di default */
//	if(angleOld != angle && angle > -maxAngleSize)
//	{
//		angle-=offset;
//	}
//	else if(angle <= maxAngleSize)
//	{
//		angle+=offset;
//		angleOld=angle;
//	}
//	else if(angleOld == angle && angle < maxAngleSize)
//	{
//		angle+=offset; 
//		angleOld+=offset;
//	}
//	else
//		angleOld=angle+10.0;
//}
//
//void timer(int valore)
//{
//	switch(valore)
//	{
//	case 0:
//
//		glutPostRedisplay();
//		glutTimerFunc(100, timer, 0); 
//
//	break;
//
//	case 1:
//		glutPostRedisplay();
//		glutTimerFunc(100, timer, 1); 
//		break;
//	}
//}
//
//void keyboard(unsigned char key, int x, int y)
//{
//
//}
//
//void specialKeyboard(int key, int x, int y)
//{
//	if(key == GLUT_KEY_UP)
//	{
//		arrowXrot-=3;
//		if(arrowXrot < -360.0)
//			arrowXrot=0;
//	}
//
//	if(key == GLUT_KEY_DOWN)
//	{
//		arrowXrot+=3;
//		if(arrowXrot>360.0)
//			arrowXrot=0;
//	}
//
//	if(key == GLUT_KEY_LEFT)
//	{
//		arrowZrot+=3;
//		if(arrowZrot > 360.0)
//			arrowZrot=0;
//	}
//
//	if(key == GLUT_KEY_RIGHT)
//	{
//		arrowZrot-=3;
//		if(arrowZrot < -360.0)
//			arrowZrot=0;
//	}
//
//	if(key == GLUT_KEY_PAGE_DOWN)
//	{
//		arrowYrot-=3;
//		if(arrowYrot < -360.0)
//			arrowYrot=0;
//	}
//
//	if(key == GLUT_KEY_END)
//	{
//		arrowYrot+=3;
//		if(arrowYrot>360.0)
//			arrowYrot=0;
//	}
//
//	/* Aggiorna l'immagine */
//	glutPostRedisplay();
//}
//
//void drawGround(void)
//{ 
//	glPushMatrix();
//	glBegin(GL_QUADS);
//	glColor3ub(0, 0, 0);
//	glVertex3f(600.0f, -50.0f, -400.0f);
//	glVertex3f(-600.0f, -50.0f, -400.0f);
//	glColor3ub(200, 200, 200);
//	glVertex3f(-600.0f, -50.0f, -20.0f);
//	glVertex3f(600.0f, -50.0f, -20.0f);
//	glEnd();
//	glPopMatrix();
//}
//
//void drawColumn(int direzione, GLfloat spostamento, GLfloat lato)
//{
//	switch(direzione)
//	{
//	case 1: 
//		glTranslatef(spostamento, 0.0, 0.0);
//		break;
//
//	case -1: 
//		glTranslatef(-spostamento, 0.0, 0.0);
//		break;
//
//	case 3: 
//		glTranslatef(0.0, 0.0, spostamento);
//		break;
//
//	case -3: 
//		glTranslatef(0.0, 0.0, -spostamento);
//		glColor3ub(0, 255, 0);
//		break;
//	}
//
//	glutWireCube(lato);
//
//	glPushMatrix();
//
//	glTranslatef(0.0, spostamento, 0.0);
//	glutWireCube(lato);
//
//	glPopMatrix();
//
//	glPushMatrix();
//
//	glTranslatef(0.0, -spostamento, 0.0);
//	glutWireCube(lato);
//
//	glPopMatrix();
//}
//
//void drawRubikCube(GLfloat edge)
//{
//	/* salva la posizione corrente e spostati in alto per disegnare il cubo */
//	glPushMatrix();
//
//	glTranslatef(0.0, edge, 0.0);
//	glColor3ub(255, 0, 0);
//	
//	glutWireCube(edge);
//
//	/* ripristina la posizione iniziale */
//	glPopMatrix();
//
//	glColor3ub(0, 255, 0);
//	/* salva la posizione corrente e spostati in basso per disegnare il cubo */
//	glPushMatrix();
//
//	glTranslatef(0.0, -edge, 0.0);
//
//	glutWireCube(edge); 
//	
//
//	/* ripristina la posizione iniziale */
//	glPopMatrix();
//
//	drawColumn(1, edge, edge);
//	drawColumn(3, edge, edge);
//	drawColumn(-1, edge, edge);
//	drawColumn(-1, edge, edge);
//	drawColumn(-3, edge, edge);
//	drawColumn(-3, edge, edge);
//	drawColumn(1, edge, edge);
//	drawColumn(1, edge, edge);
//}
//
//void display(void)
//{
//	/* inizializza il color buffer e il depth buffer */
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//	/* d'ora in poi si modifica la matrice modelview*/
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//
//	drawGround();
//
//	glTranslatef(0.0, 0.0, -50.0);
//
//	glRotatef(45.0, 1.0, 0.0, 0.0);
//	glRotatef(45.0, 0.0, 1.0, 0.0);
//
//	glRotatef(arrowXrot, 1.0, 0.0, 0.0);
//	glRotatef(arrowYrot, 0.0, 1.0, 0.0);
//	glRotatef(arrowZrot, 0.0, 0.0, 0.0);
//
//	oscilla();
//	drawRubikCube(10.0);
//
//	glutSwapBuffers();
//}
//
//void reshape(GLsizei w, GLsizei h)
//{
//	GLfloat ratio;
//
//	if(h==0)
//		h=1;
//
//	glViewport(0, 0, w, h);
//
//	ratio=(GLfloat) w/ (GLfloat) h;
//
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//
//	gluPerspective(90.0, ratio, 20.0, 500.0);
//	//glOrtho(-100.0, 100.0, -75.0, 75.0, 20.0, 300.0);
//
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//}
//
//void main()
//{
//	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
//
//	glutInitWindowSize(800, 600);
//	glutInitWindowPosition(0, 0);
//	glutCreateWindow("Prova");
//
//	glutTimerFunc(250, timer, 0);
//	glutDisplayFunc(display);
//	glutReshapeFunc(reshape);
//	//glutKeyboardFunc(keyboard);
//	glutSpecialFunc(specialKeyboard);
//	//glutIdleFunc(idle);
//
//	init();
//
//	glutMainLoop();
//}