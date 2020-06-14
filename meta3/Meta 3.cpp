#include <windows.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include <GL\glew.h>
#include <GL\freeGlut.h>
#include "RgbImage.h"
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"glu32.lib")
#pragma comment(lib,"opengl32.lib")

#define PI		 3.14159

GLint wScreen = 600, hScreen = 500;
GLfloat		yC = 100.0;	//.. Mundo
GLint msec = 40;

GLint uniOp;
GLint uniDir;
float Direcao[] = { 0, 1, 0 };
float opcao = -45;

char filenameV[] = "vertex_shader.txt";
char filenameF[] = "fragment_shader.txt";

char* VertexShaderSource;
char* FragmentShaderSource;
GLuint VertexShader, FragmentShader;
GLuint ShaderProgram;

GLfloat y_mesa_cima = 3.0f;
GLfloat y_mesa_baixo = 2.5f;
GLfloat y_pernas = 2.5f;

GLfloat  rVisao = 8, aVisao = PI, incVisao = 0.05;
GLfloat  obsPini[] = { rVisao * cos(aVisao), 3.0, rVisao * sin(aVisao) };
GLfloat  obsPfin[] = { obsPini[0] - rVisao * cos(aVisao), obsPini[1], obsPini[2] - rVisao * sin(aVisao) };

GLint uniCor;
GLfloat cor[4] = { 1.0f, 1.0f, 1.0f , 1};

float time = 0;

GLfloat angulo = 0.0f;

GLuint text;
GLint textLoc;
GLuint texturas[4]; // 0 - madeira     1 - azulejo     2 - parede	3 - teto
RgbImage imag;

GLUquadricObj* esfera = gluNewQuadric();

char* readShaderFile(char* FileName);

char* readShaderFile(char* FileName) {
	char* DATA = NULL;
	int   flength = 0;
	FILE* filepoint;
	errno_t err;

	err = fopen_s(&filepoint, FileName, "r");
	if (!err) {
		fseek(filepoint, 0, SEEK_END);
		flength = ftell(filepoint);
		rewind(filepoint);

		DATA = (char*)malloc(sizeof(char) * (flength + 1));
		flength = fread(DATA, sizeof(char), flength, filepoint);

		DATA[flength] = '\0';
		fclose(filepoint);
		return DATA;
	}
	else {
		printf("Error reading file %s ", FileName);
	}
}

void builtShader(void) {
	GLEW_ARB_vertex_shader;
	GLEW_ARB_fragment_shader;

	VertexShader = glCreateShader(GL_VERTEX_SHADER);
	FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	VertexShaderSource = readShaderFile(filenameV);
	FragmentShaderSource = readShaderFile(filenameF);

	const char* VS = VertexShaderSource;
	const char* FS = FragmentShaderSource;
	glShaderSource(VertexShader, 1, &VS, NULL);
	glShaderSource(FragmentShader, 1, &FS, NULL);
	free(VertexShaderSource);
	free(FragmentShaderSource);

	glCompileShaderARB(VertexShader);
	glCompileShaderARB(FragmentShader);

	ShaderProgram = glCreateProgramObjectARB();
	glAttachShader(ShaderProgram, VertexShader);
	glAttachShader(ShaderProgram, FragmentShader);
	glLinkProgram(ShaderProgram);

	glUseProgramObjectARB(ShaderProgram);
}

void iniciaShader(void) {
	builtShader();

	uniDir = glGetUniformLocation(ShaderProgram, "Direcao");
	glUniform3fv(uniDir, 0, Direcao);
	uniOp = glGetUniformLocation(ShaderProgram, "opcao");
	glUniform1f(uniOp, opcao);
	uniCor = glGetUniformLocation(ShaderProgram, "cor");
	glUniform4fv(uniCor, 0, cor);
}

void libertaShader(void) {
	glDetachShader(ShaderProgram, VertexShader);
	glDetachShader(ShaderProgram, FragmentShader);
	glDeleteShader(ShaderProgram);
}

void inicializaTextura() {
	// Textura de madeira para o banco
	glGenTextures(1, &texturas[0]);
	glBindTexture(GL_TEXTURE_2D, texturas[0]);
	imag.LoadBmpFile("madeira.bmp");
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
		imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());

	// Textura de azulejo para chão
	glGenTextures(1, &texturas[1]);
	glBindTexture(GL_TEXTURE_2D, texturas[1]);
	imag.LoadBmpFile("chao.bmp");
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
		imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());

	// Textura para paredes
	glGenTextures(1, &texturas[2]);
	glBindTexture(GL_TEXTURE_2D, texturas[2]);
	imag.LoadBmpFile("azulejo.bmp");
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
		imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());

	// Textura para o teto
	glGenTextures(1, &texturas[3]);
	glBindTexture(GL_TEXTURE_2D, texturas[3]);
	imag.LoadBmpFile("safira.bmp");
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
		imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());
}

void inicializa(void) {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	inicializaTextura();
}

void Timer(int value) {
	glutPostRedisplay();
	glutTimerFunc(msec, Timer, 1);
}

void drawAcento() {
	glNormal3f(0, 1, 0);
	glBegin(GL_QUADS);
		glVertex3f(-1.5f, y_mesa_cima, -1.5f);
		glVertex3f(1.5f, y_mesa_cima, -1.5f);
		glVertex3f(1.5f, y_mesa_cima, 1.5f);
		glVertex3f(-1.5f, y_mesa_cima, 1.5f);

		glVertex3f(-1.5f, y_mesa_baixo, -1.5f);
		glVertex3f(1.5f, y_mesa_baixo, -1.5f);
		glVertex3f(1.5f, y_mesa_baixo, 1.5f);
		glVertex3f(-1.5f, y_mesa_baixo, 1.5f);

		glVertex3f(-1.5f, y_mesa_cima, 1.5f);
		glVertex3f(-1.5f, y_mesa_baixo, 1.5f);
		glVertex3f(1.5f, y_mesa_baixo, 1.5f);
		glVertex3f(1.5f, y_mesa_cima, 1.5f);

		glVertex3f(-1.5f, y_mesa_cima, -1.5f);
		glVertex3f(-1.5f, y_mesa_baixo, -1.5f);
		glVertex3f(1.5f, y_mesa_baixo, -1.5f);
		glVertex3f(1.5f, y_mesa_cima, -1.5f);

		glVertex3f(-1.5f, y_mesa_cima, -1.5f);
		glVertex3f(-1.5f, y_mesa_baixo, -1.5f);
		glVertex3f(-1.5f, y_mesa_baixo, 1.5f);
		glVertex3f(-1.5f, y_mesa_cima, 1.5f);

		glVertex3f(1.5f, y_mesa_cima, -1.5f);
		glVertex3f(1.5f, y_mesa_baixo, -1.5f);
		glVertex3f(1.5f, y_mesa_baixo, 1.5f);
		glVertex3f(1.5f, y_mesa_cima, 1.5f);
	glEnd();
}

void drawPernas() {
	glNormal3f(0, 1, 0);
	glBegin(GL_QUADS);
		//perna 1
		glVertex3f(1.5f, 0.0f, 1.5f);
		glVertex3f(1.5f, 0.0f, 1.0f);
		glVertex3f(1.5f, y_pernas, -1.5f);
		glVertex3f(1.5f, y_pernas, -1.0f);

		glVertex3f(1.5f, 0.0f, 1.5f);
		glVertex3f(1.5f, 0.0f, 1.0f);
		glVertex3f(1.0f, 0.0f, 1.0f);
		glVertex3f(1.0f, 0.0f, 1.5f);

		glVertex3f(1.0f, 0.0f, 1.5f);
		glVertex3f(1.0f, 0.0f, 1.0f);
		glVertex3f(1.0f, y_pernas, -1.5f);
		glVertex3f(1.0f, y_pernas, -1.0f);

		glVertex3f(1.5f, 0.0f, 1.0f);
		glVertex3f(1.0f, 0.0f, 1.0f);
		glVertex3f(1.0f, y_pernas, -1.5f);
		glVertex3f(1.5f, y_pernas, -1.5f);
	
		glVertex3f(1.5f, 0.0f, 1.5f);
		glVertex3f(1.5f, 0.0f, 1.0f);
		glVertex3f(1.5f, y_pernas, -1.0f);
		glVertex3f(1.0f, y_pernas, -1.0f);
	
		//perna 2
		glVertex3f(-1.5f, 0.0f, 1.5f);
		glVertex3f(-1.5f, 0.0f, 1.0f);
		glVertex3f(-1.5f, y_pernas, -1.5f);
		glVertex3f(-1.5f, y_pernas, -1.0f);

		glVertex3f(-1.5f, 0.0f, 1.5f);
		glVertex3f(-1.5f, 0.0f, 1.0f);
		glVertex3f(-1.0f, 0.0f, 1.0f);
		glVertex3f(-1.0f, 0.0f, 1.5f);

		glVertex3f(-1.0f, 0.0f, 1.5f);
		glVertex3f(-1.0f, 0.0f, 1.0f);
		glVertex3f(-1.0f, y_pernas, -1.5f);
		glVertex3f(-1.0f, y_pernas, -1.0f);

		glVertex3f(-1.5f, 0.0f, 1.0f);
		glVertex3f(-1.0f, 0.0f, 1.0f);
		glVertex3f(-1.0f, y_pernas, -1.5f);
		glVertex3f(-1.5f, y_pernas, -1.5f);

		glVertex3f(-1.5f, 0.0f, 1.5f);
		glVertex3f(-1.5f, 0.0f, 1.0f);
		glVertex3f(-1.0f, y_pernas, -1.0f);
		glVertex3f(-1.5f, y_pernas, -1.0f);

		//perna 3
		glVertex3f(-1.5f, 0.0f, -1.0f);
		glVertex3f(-1.5f, 0.0f, -1.5f);
		glVertex3f(-1.5f, y_pernas, 1.0f);
		glVertex3f(-1.5f, y_pernas, 1.5f);

		glVertex3f(-1.5f, 0.0f, -1.0f);
		glVertex3f(-1.5f, 0.0f, -1.5f);
		glVertex3f(-1.0f, 0.0f, -1.5f);
		glVertex3f(-1.0f, 0.0f, -1.0f);

		glVertex3f(-1.0f, 0.0f, -1.0f);
		glVertex3f(-1.0f, 0.0f, -1.5f);
		glVertex3f(-1.0f, y_pernas, 1.0f);
		glVertex3f(-1.0f, y_pernas, 1.5f);
	
		glVertex3f(-1.5f, 0.0f, -1.5f);
		glVertex3f(-1.0f, 0.0f, -1.5f);
		glVertex3f(-1.0f, y_pernas, 1.0f);
		glVertex3f(-1.5f, y_pernas, 1.0f);

		glVertex3f(-1.5f, 0.0f, -1.0f);
		glVertex3f(-1.5f, 0.0f, -1.5f);
		glVertex3f(-1.0f, y_pernas, 1.5f);
		glVertex3f(-1.5f, y_pernas, 1.5f);

		//perna 4
		glVertex3f(1.5f, 0.0f, -1.0f);
		glVertex3f(1.5f, 0.0f, -1.5f);
		glVertex3f(1.5f, y_pernas, 1.0f);
		glVertex3f(1.5f, y_pernas, 1.5f);

		glVertex3f(1.5f, 0.0f, -1.0f);
		glVertex3f(1.5f, 0.0f, -1.5f);
		glVertex3f(1.0f, 0.0f, -1.5f);
		glVertex3f(1.0f, 0.0f, -1.0f);

		glVertex3f(1.0f, 0.0f, -1.0f);
		glVertex3f(1.0f, 0.0f, -1.5f);
		glVertex3f(1.0f, y_pernas, 1.0f);
		glVertex3f(1.0f, y_pernas, 1.5f);
	
		glVertex3f(1.5f, 0.0f, -1.5f);
		glVertex3f(1.0f, 0.0f, -1.5f);
		glVertex3f(1.0f, y_pernas, 1.0f);
		glVertex3f(1.5f, y_pernas, 1.0f);
	
		glVertex3f(1.5f, 0.0f, -1.0f);
		glVertex3f(1.5f, 0.0f, -1.5f);
		glVertex3f(1.0f, y_pernas, 1.5f);
		glVertex3f(1.5f, y_pernas, 1.5f);
	glEnd();
}

void drawTeapot() {
	glPushMatrix();
		glTranslatef(4, 3, 0);
		time = time + 1;
		glRotatef(time * 0.1, 1, 1, 0);
		glutSolidTeapot(1);
	glPopMatrix();
}

void drawTeacup() {
	glPushMatrix();
		glTranslatef(0, 3, 4);
		time = time + 1;
		glRotatef(time * 0.1, 1, 1, 0);
		glutSolidTeacup(1);
	glPopMatrix();
}

void drawEsfera() {
	glPushMatrix();
		glTranslatef(0, 5, 0);
		time = time + 1;
		glRotatef(time * 0.1, 1, 1, 0);
		gluQuadricDrawStyle(esfera, GLU_FILL);
		gluQuadricNormals(esfera, GLU_SMOOTH);
		gluQuadricTexture(esfera, GL_TRUE);
		gluSphere(esfera, 1, 150, 150);
	glPopMatrix();
}

void drawChao() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texturas[0]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);	glVertex3f(-5.0f, -0.1f, -5.0f);
		glTexCoord2f(1.0f, 0.0f);	glVertex3f(-5.0f, -0.1f, 5.0f);
		glTexCoord2f(1.0f, 1.0f);	glVertex3f(5.0f, -0.1f, 5.0f);
		glTexCoord2f(0.0f, 1.0f);	glVertex3f(5.0f, -0.1f, -5.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void drawScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, wScreen, hScreen);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, 1, 0.1, 9999);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(obsPini[0], obsPini[1], obsPini[2], obsPfin[0], obsPfin[1], obsPfin[2], 0, 1, 0);

	glUniform1f(uniOp, opcao);
	glUniform3fv(uniDir, 1, Direcao);
	glUniform4fv(uniCor, 1, cor);

	Direcao[0] = cos(3.14 * opcao / 180.0);
	Direcao[2] = sin(3.14 * opcao / 180.0);
	glPushMatrix();
		glRotatef(angulo, 0, 1, 0);
		drawAcento();
		drawPernas();
	glPopMatrix();
	drawEsfera();
	drawTeapot();
	drawTeacup();
	drawChao();

	glutSwapBuffers();
}

void Teclado(unsigned char key, int x, int y) {
	switch (key) {
		case 'o': case 'O':
			opcao = opcao + 10;
			glutPostRedisplay();
			break;

		// subida do banco
		case 'n': case 'N':
			y_mesa_cima += 0.5f;
			y_mesa_baixo += 0.5f;
			y_pernas += 0.5f;
			glutPostRedisplay();
			break;
		// descida do banco
		case 'm': case 'M':
			y_mesa_cima -= 0.5f;
			y_mesa_baixo -= 0.5f;
			y_pernas -= 0.5f;
			glutPostRedisplay();
			break;

		case 'r': case 'R':
			cor[0] = cor[0] + 0.1;
			if (cor[0] > 1.1) cor[0] = 0.0;
			printf("R %4.2f\tG %4.2f\tB %4.2f\n", cor[0], cor[1], cor[2]);
			glutPostRedisplay();
			break;

		case 'g': case 'G':
			cor[1] = cor[1] + 0.1;
			if (cor[1] > 1.1) cor[1] = 0.0;
			printf("R %4.2f\tG %4.2f\tB %4.2f\n", cor[0], cor[1], cor[2]);
			glutPostRedisplay();
			break;

		case 'b': case 'B':
			cor[2] = cor[2] + 0.1;
			if (cor[2] > 1.1) cor[2] = 0.0;
			printf("R %4.2f\tG %4.2f\tB %4.2f\n", cor[0], cor[1], cor[2]);
			glutPostRedisplay();
			break;

		case 'p': case 'P':
			angulo += 5;
			glutPostRedisplay();
			break;

		case 27:		// ESC
			exit(0);
			break;
	}
}

void teclasNotAscii(int key, int x, int y) {
	//------------------------------- olhar para a origem
	obsPfin[0] = 0; obsPfin[1] = 0; obsPfin[2] = 0;

	if (key == GLUT_KEY_UP)    obsPini[1] = obsPini[1] + 0.5;
	if (key == GLUT_KEY_DOWN)  obsPini[1] = obsPini[1] - 0.5;
	if (key == GLUT_KEY_LEFT)  aVisao = aVisao + 0.1;
	if (key == GLUT_KEY_RIGHT) aVisao = aVisao - 0.1;

	if (obsPini[1] > yC)   obsPini[1] = yC;
	if (obsPini[1] < -yC)  obsPini[1] = -yC;
	obsPini[0] = rVisao * cos(aVisao);
	obsPini[2] = rVisao * sin(aVisao);

	glutPostRedisplay();

}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(700, 500);
	glutInitWindowPosition(200, 200);
	glutCreateWindow("Projeto CG Meta 3 - Shaders");

	inicializa();

	//GLEW
	GLenum err = glewInit();
	iniciaShader();

	libertaShader();

	//----------------------------------------------------------
	glutSpecialFunc(teclasNotAscii);
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(Teclado);
	glutIdleFunc(drawScene);

	glutMainLoop();
	return 1;
}