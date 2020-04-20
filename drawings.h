#include <GL/glut.h>
#include <math.h>
#include <iostream>

using namespace std;

GLfloat red[]={1.0,0.0,0.0};
GLfloat green[]={0.0,1.0,0.0};
GLfloat blue[]={0.0,0.0,1.0};
GLfloat disable[] = {0.0,0.0,0.0};

GLUquadric *quad = gluNewQuadric();

GLfloat Ka1[] = {1.0,1.0,0.45};
GLfloat Ka2[] = {1.0,1.0,0.5};
GLfloat Ka3[] = {1.0,1.0,0.55};
GLfloat Ka4[] = {1.0,1.0,0.6};
float center_1[] = {-100,-100};
float center_2[] = {-100,100};
float center_3[] = {100,100};
float center_4[] = {100,-100};

void cloud_balls() {
	gluSphere(quad, 30, 30, 30);
	glTranslatef(-20, -30.0, 0.0);
	gluSphere(quad, 30, 30, 30);
	glTranslatef(40, 0.0, 0.0);
	gluSphere(quad, 30, 30, 30);
	glTranslatef(-20, 30.0, 0.0);
}

void Nuvem(int x, int y, int z, float r) {
	glDisable(GL_LIGHTING);
	glColor3f(1.0,1.0,1.0);
	glTranslatef(x, y, z);
	glRotatef(r, 0.0, 1.0, 0.0);
	cloud_balls();
	glRotatef(-r, 0.0, 1.0, 0.0);
	glTranslatef(-x, -y, -z);
	glEnable(GL_LIGHTING);
}

float *clouds_coordinates(float *cc) {
	float clouds_sphere_radius = 600.0/sqrt(2.0);
	float thetas[] = {0, 30, 45, 90, 120, 135, 180, -30, -45, -90, -120, -135};
	for(int i = 0, j = 0; i < 24; i+=2, j++) {
		cc[i] = clouds_sphere_radius*cos(thetas[j]);
		cc[i+1] = clouds_sphere_radius*sin(thetas[j]);
	}
	return cc;
}

float arr[24];
float *cc = clouds_coordinates(arr);
float thetas[] = {0, 30, 45, 90, 120, 135, 180, -30, -45, -90, -120, -135};
float rot = 0;

void Nuvens() {
	rot += 0.05;
	if(rot >= 360.0) rot = 0;
	glRotatef(rot, 0.0, 1.0, 0.0);
	for(int i = 0, j = 0; i < 24; i+=2, j++)
		Nuvem(cc[i], 150, cc[i+1], thetas[j]+45);
	glRotatef(-rot, 0.0, 1.0, 0.0);
}

void Folha() {
	glColor3f(0.0, 0.8, 0.0);
	glBegin(GL_POLYGON);
		glVertex3i(0, 30, 0);
		glVertex3i(-7, 33, -3);
		glVertex3i(-7, 33, 3);
	glEnd();
	glBegin(GL_POLYGON);
		glVertex3i(-7, 33, -3);
		glVertex3i(-8, 23, 0);
		glVertex3i(-7, 33, 3);
	glEnd();
}

void Coco() {
	glColor3f(0.12, 0.63, 0.0);
	glTranslatef(-2.0, 29.0, 0.0);
	gluSphere(quad, 1.0, 30, 30);
	glTranslatef(2.0, -29.0, 0.0);
}

void Folhas() {
	Folha();
	Coco();
	glRotatef(90.0, 0.0, 1.0, 0.0);
	Folha();
	Coco();
	glRotatef(90.0, 0.0, 1.0, 0.0);
	Folha();
	Coco();
	glRotatef(90.0, 0.0, 1.0, 0.0);
	Folha();
	Coco();
	glRotatef(90.0, 0.0, 1.0, 0.0);
}

void Arvore(int x, int z) {
	glColor3f(0.5, 0.3, 0.0);
	glTranslatef(x, 0.0, z);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	gluCylinder(quad, 2.0, 1.0, 10.0, 5, 5);
	glTranslatef(0.0, 0.0, 5.0);
	gluCylinder(quad, 1.0, 1.0, 25.0, 5, 5);
	glTranslatef(0.0, 0.0, -5.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	Folhas();
	glTranslatef(-x, -0.0, -z);
}

void Draw_Floor_Square(float center[], int size, GLfloat color[]) {
	size /= 2;
	// glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);
	glColor3f(color[0], color[1], color[2]);
	glBegin(GL_POLYGON);
		glVertex3i(center[0]-size, 0, center[1]-size);
		glVertex3i(center[0]-size, 0, center[1]+size);
		glVertex3i(center[0]+size, 0, center[1]+size);
		glVertex3i(center[0]+size, 0, center[1]-size);
	glEnd();
}
void Desenha_Chao() {
	glDisable(GL_LIGHTING);
	Arvore(0,0);
	Arvore(150,140);
	Arvore(-150,-140);
	Arvore(150,30);
	Arvore(60,-80);
	Arvore(-80,170);
	Arvore(15,170);
	Arvore(-160, -9);
	Draw_Floor_Square(center_1, 200, Ka1);
	Draw_Floor_Square(center_2, 200, Ka2);
	Draw_Floor_Square(center_3, 200, Ka3);
	Draw_Floor_Square(center_4, 200, Ka4);
	glEnable(GL_LIGHTING);
}

void Desenha_Paredes() {
	Nuvens();
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	float Kd[] = {0.0,0.63,0.99};
	float Ks[] = {1.0,1.0,1.0};
	GLfloat Se[] = {30};
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Kd);
	// glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Ks);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, Se);
	glBegin(GL_POLYGON);
		glVertex3i(-200, 20, 200);
		glVertex3i(-200, 0, 200);
		glVertex3i(200, 0, 200);
		glVertex3i(200, 20, 200);
	glEnd();
	glBegin(GL_POLYGON);
		glVertex3i(-200, 0, -200);
		glVertex3i(-200, 20, -200);
		glVertex3i(200, 20, -200);
		glVertex3i(200, 0, -200);
	glEnd();
	glBegin(GL_POLYGON);
		glVertex3i(-200, 0, 200);
		glVertex3i(-200, 20, 200);
		glVertex3i(-200, 20, -200);
		glVertex3i(-200, 0, -200);
	glEnd();
	glBegin(GL_POLYGON);
		glVertex3i(200, 20, 200);
		glVertex3i(200, 0, 200);
		glVertex3i(200, 0, -200);
		glVertex3i(200, 20, -200);
	glEnd();
	glDisable(GL_CULL_FACE);
}

void escrever(int x, int y, char *texto, int tamanho, int avanco=11) {
	for(int i = 0, a = 0; i < tamanho; i++, a += avanco) {
		glRasterPos2d(x + a, y);
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, texto[i]);
	}
}

void menu_box(int y, int window_width, int window_height, char *text) {
	glColor3f(0.8,0.8,0.8);
	glBegin(GL_QUADS);
		glVertex2f(window_width/2-100,window_height/2 + y);
		glVertex2f(window_width/2-100,window_height/2 + y + 50);
		glVertex2f(window_width/2+100,window_height/2 + y + 50);
		glVertex2f(window_width/2+100,window_height/2 + y);
	glEnd();
	glColor4f(0.0, 0.0, 1.0, 0.0);
	escrever(window_width/2-60, window_height/2 + y + 30, text, 11, 11);
}

void Pause_Menu(int window_width, int window_height, char *points, bool full_screen=0) {
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, window_width, window_height, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// Pontuação:
	glColor4f(0.0, 0.0, 0.0, 1.0);
	escrever(100, 100, points, 13, 11);
	// Caixas do Menu:
	menu_box(-120, window_width, window_height, (char *)" CONTINUAR ");
	menu_box(-60, window_width, window_height, (char *)" REINICIAR ");
	if(full_screen) menu_box(0, window_width, window_height, (char *)"TELA NORMAL");
	else menu_box(0, window_width, window_height, (char *)"TELA CHEIA ");
	menu_box(60, window_width, window_height, (char *)"   SAIR    ");
	// Voltando à projeção normal
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0f, ((GLfloat)window_width/(GLfloat)window_height), 1, 1600.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glutSwapBuffers();
}

void Lost_Menu(int window_width, int window_height, char *points) {
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, window_width, window_height, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glColor3f(0.7, 0.0, 0.0);
	glBegin(GL_POLYGON);
		glVertex2f(window_width/2 -200, window_height/2 -200);
		glVertex2f(window_width/2 -200, window_height/2 +200);
		glVertex2f(window_width/2 +200, window_height/2 +200);
		glVertex2f(window_width/2 +200, window_height/2 -200);
	glEnd();
	glColor3f(1.0, 1.0, 1.0);
	escrever(window_width/2 - 60, window_height/2 -110, (char *)"VOCE PERDEU!", 12);
	escrever(window_width/2 - 70, window_height/2 -60, points, 13);
	menu_box(0, window_width, window_height, (char *)" REINICIAR ");
	menu_box(60, window_width, window_height, (char *)"   SAIR    ");

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0f, ((GLfloat)window_width/(GLfloat)window_height), 1, 1600.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glutSwapBuffers();
}

void points_to_char(int p, char *c) {
	char p1[] = {'P','O','N','T','O','S',':',' '};
	for(int i = 0; i < 8; i++) c[i] = p1[i];
	for(int i = 0; i < 5; i++) c[i+8] = '0' + (p % int(pow(10, 5-i)) / int(pow(10, 4-i)));
}

void Gold_Color() {
	float color[] = {1.0, 1.0, 0.0};
	float Ka[] = {0.3,0.3,0.0};
	float Ks[] = {1.0,1.0,1.0};
	float Ke[] = {0.3,0.3,0.0};
	GLfloat Se[] = {18};
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, Ka);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Ks);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, Ke);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, Se);
}

void Desenha_Bau() {
	float color[] = {0.4, 0.2, 0.0};
	float Ks[] = {0.3,0.3,0.5};
	float Ke2[] = {0.0,0.0,0.0};
	GLfloat Se[] = {40};
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Ks);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, Se);
	
	glTranslatef(190.0, 2.5, -190.0);
	glRotatef(45, 0.0, 1.0, 0.0);
	
	glutSolidCube(5); // Caixa
	
	// Tampa
	glTranslatef(3, 5.0, 0.0);
	glScalef(1.0, 5.0, 5.0);
	glutSolidCube(1);
	glScalef(1.0, 0.2, 0.2);
	glTranslatef(-3, -5.0, 0.0);

	Gold_Color();
	glTranslatef(0.0, 2.0, 0.0);
	glScalef(1.0, 1.0, 1.25);
	gluSphere(quad, 2, 10, 5);
	glScalef(1.0, 1.0, 0.8);
	glTranslatef(0.0, -2.0, 0.0);
	
	glRotatef(-45, 0.0, 1.0, 0.0);
	glTranslatef(-190.0, -2.5, 190.0);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, Ke2);
}