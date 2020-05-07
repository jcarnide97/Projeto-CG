#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>
#include "RgbImage.h"

//--------------------------------- Definir cores
#define BLUE     0.0, 0.0, 1.0, 1.0
#define RED		 1.0, 0.0, 0.0, 1.0
#define YELLOW	 1.0, 1.0, 0.0, 1.0
#define GREEN    0.0, 1.0, 0.0, 1.0
#define ORANGE   1.0, 0.5, 0.1, 1.0
#define CYAN     0.0, 1.0, 1.0, 1.0
#define WHITE    1.0, 1.0, 1.0, 1.0
#define BLACK    0.0, 0.0, 0.0, 1.0
#define GRAY     0.3, 0.3, 0.3, 1.0
#define PI		 3.14159

void initMaterials(int material);

//================================================================================
//===========================================================Variaveis e constantes
GLint     msec = 10;
GLint     frenteVisivel = 2;   //disable cullface
char      texto[30];						//.. texto a ser escrito no ecran


//------------------------------------------------------------ Sistema Coordenadas + objectos
GLint		wScreen = 600, hScreen = 500;		//.. janela
GLfloat		yC = 100.0;	//.. Mundo

//------------------------------------------------------------ Observador 
GLfloat  rVisao = 8, aVisao = PI, incVisao = 0.05;
GLfloat  obsPini[] = { rVisao * cos(aVisao), 3.0, rVisao * sin(aVisao) };
GLfloat  obsPfin[] = { obsPini[0] - rVisao * cos(aVisao), obsPini[1], obsPini[2] - rVisao * sin(aVisao) };

GLuint texturas[4]; // 0 - madeira     1 - azulejo     2 - parede	3 - teto
RgbImage imag;

float anguloZ = 95;
int   tipoProjeccao = 0;

static GLfloat vertices[] = {
-0.500000, -0.000000, -0.960000, 0.500000, -0.000000, -0.960000, 0.500000, 0.000000, -0.000000, -0.500000, 0.000000, -0.000000, -0.500000, 0.000000, -0.960000, 0.500000, 0.000000, -0.960000, 0.500000, 0.160000, -0.960000, -0.500000, 0.160000, -0.960000, -0.500000, 0.160000, -1.280000, 0.500000, 0.160000, -1.280000, 0.500000, 0.160000, -0.960000, -0.500000, 0.160000, -0.960000, -0.500000, 0.160000, -1.280000, 0.500000, 0.160000, -1.280000, 0.500000, 0.320000, -1.280000, -0.500000, 0.320000, -1.280000, -0.500000, 0.320000, -1.600000, 0.500000, 0.320000, -1.600000, 0.500000, 0.320000, -1.280000, -0.500000, 0.320000, -1.280000, -0.500000, 0.320000, -1.600000, 0.500000, 0.320000, -1.600000, 0.500000, 0.480000, -1.600000, -0.500000, 0.480000, -1.600000, -0.500000, 0.480000, -2.560000, 0.500000, 0.480000, -2.560000, 0.500000, 0.480000, -1.600000, -0.500000, 0.480000, -1.600000, -0.500000, 0.480000, -2.560000, 0.500000, 0.480000, -2.560000, 0.500000, 0.640000, -2.560000, -0.500000, 0.640000, -2.560000, -0.500000, 0.640000, -2.880000, 0.500000, 0.640000, -2.880000, 0.500000, 0.640000, -2.560000, -0.500000, 0.640000, -2.560000, -0.500000, 0.640000, -2.880000, 0.500000, 0.640000, -2.880000, 0.500000, 0.800000, -2.880000, -0.500000, 0.800000, -2.880000, -0.500000, 0.800000, -3.200000, 0.500000, 0.800000, -3.200000, 0.500000, 0.800000, -2.880000, -0.500000, 0.800000, -2.880000, -0.500000, 0.800000, -3.200000, 0.500000, 0.800000, -3.200000, 0.500000, 0.960000, -3.200000, -0.500000, 0.960000, -3.200000, -0.500000, 0.960000, -4.160000, 0.500000, 0.960000, -4.160000, 0.500000, 0.960000, -3.200000, -0.500000, 0.960000, -3.200000
};
static GLfloat normais[] = {
0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000
};
static GLfloat cores[] = {
0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000
};

double roda_x = 0;
double roda_y = 0;
double roda_z = 0;

GLfloat trans_x = 0.0f;
GLfloat trans_z = 0.0f;

GLfloat angulo = 0.0f;

GLfloat y_mesa_cima = 3.0f;
GLfloat y_mesa_baixo = 2.5f;
GLfloat y_pernas = 2.5f;

GLUquadricObj* esfera = gluNewQuadric();

// VARIÁVEIS DE LUZ
// Luz ambiente
GLint dia = 1;	//1 - dia	0 - noite
GLfloat intensidadeDia = 0.7;
GLfloat luzAmbiente[4] = { intensidadeDia, intensidadeDia, intensidadeDia, 1.0 };
// Luz teto
GLint teto = 1;
GLfloat intensidadeTeto = 0.3f;
GLint luzR = 1;
GLint luzG = 1;
GLint luzB = 1;
GLfloat posicaoLuz[4] = { 0.0, 12.0, 0.0, 1.0 };
GLfloat localCorAmbiente[4] = { luzR, luzG, luzB, 1.0 };
GLfloat localCorDifusa[4] = { luzR, luzG, luzB, 1.0 };
GLfloat localCorEspecular[4] = { luzR, luzG, luzB, 1.0 };
// Luz foco
GLint foco1 = 0;
GLfloat posicaoFoco1[4] = { -15.0, 0.0, -15.0, 1.0 };
GLfloat corFoco1[4] = { 1.0, 0.0, 0.0, 1.0 };
GLfloat direcaoFoco1[3] = { 1.0, 1.0, 0.0 };
GLfloat anguloFoco = 45.0;
GLint foco2 = 0;
GLfloat posicaoFoco2[4] = { 15.0, 0.0, -15.0, 1.0 };
GLfloat corFoco2[4] = { 0.0, 1.0, 0.0, 1.0 };
GLfloat direcaoFoco2[3] = { -1.0, 1.0, 0.0 };
GLint foco3 = 0;
GLfloat posicaoFoco3[4] = { -15.0, 0.0, 15.0, 1.0 };
GLfloat corFoco3[4] = { 0.0, 0.0, 1.0, 1.0 };
GLfloat direcaoFoco3[3] = { 1.0, -1.0, 0.0 };
GLint foco4 = 0;
GLfloat posicaoFoco4[4] = { 15.0, 0.0, 15.0, 1.0 };
GLfloat corFoco4[4] = { 1.0, 0.0, 1.0, 1.0 };
GLfloat direcaoFoco4[3] = { -1.0, -1.0, 0.0 };

GLint malha = 0;

char Materiais[18][30] = {"Esmerald", "Jade", "obsidian", "Pearl", "Ruby", "Turquoise", "Brass", "Bronze", "Chrome", "Copper", "Gold", "Silver", "blackPlastic", "cyanPlastic", "greenPlastic", "redPlastic", "whitePlastic", "yellowPlastic" };
GLint materialParede = 1;

char textoInfo[30];

//Inicializar as texturas
void inicializaTexturas() {
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


void inicializaLuzes(void) {
	// luz ambiente
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);
	// luz teto
	glLightfv(GL_LIGHT1, GL_POSITION, posicaoLuz);
	glLightfv(GL_LIGHT0, GL_AMBIENT, localCorAmbiente);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, localCorDifusa);
	glLightfv(GL_LIGHT0, GL_SPECULAR, localCorEspecular);
	// luz foco 1
	glLightfv(GL_LIGHT2, GL_POSITION, posicaoFoco1);
	glLightfv(GL_LIGHT2, GL_AMBIENT, corFoco1);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, corFoco1);
	glLightfv(GL_LIGHT2, GL_SPECULAR, corFoco1);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, anguloFoco);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, direcaoFoco1);
	// luz foco 2
	glLightfv(GL_LIGHT3, GL_POSITION, posicaoFoco2);
	glLightfv(GL_LIGHT3, GL_AMBIENT, corFoco2);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, corFoco2);
	glLightfv(GL_LIGHT3, GL_SPECULAR, corFoco2);
	glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, anguloFoco);
	glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, direcaoFoco2);
	// luz foco 3
	glLightfv(GL_LIGHT4, GL_POSITION, posicaoFoco3);
	glLightfv(GL_LIGHT4, GL_AMBIENT, corFoco3);
	glLightfv(GL_LIGHT4, GL_DIFFUSE, corFoco3);
	glLightfv(GL_LIGHT4, GL_SPECULAR, corFoco3);
	glLightf(GL_LIGHT4, GL_SPOT_CUTOFF, anguloFoco);
	glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, direcaoFoco3);
	// luz foco 4
	glLightfv(GL_LIGHT5, GL_POSITION, posicaoFoco4);
	glLightfv(GL_LIGHT5, GL_AMBIENT, corFoco4);
	glLightfv(GL_LIGHT5, GL_DIFFUSE, corFoco4);
	glLightfv(GL_LIGHT5, GL_SPECULAR, corFoco4);
	glLightf(GL_LIGHT5, GL_SPOT_CUTOFF, anguloFoco);
	glLightfv(GL_LIGHT5, GL_SPOT_DIRECTION, direcaoFoco4);
}


//=========================================================================== INIT
void initialize(void) {
	glClearColor(BLACK);		//………………………………………………………………………………Apagar
	glEnable(GL_DEPTH_TEST);	//………………………………………………………………………………Profundidade
	glShadeModel(GL_SMOOTH);	//………………………………………………………………………………Interpolacao de cores	
	glEnable(GL_NORMALIZE);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT2);
	glEnable(GL_LIGHT3);
	glEnable(GL_LIGHT4);
	glEnable(GL_LIGHT5);

	inicializaLuzes();
	initMaterials(22);

	inicializaTexturas();

	glEnable(GL_CULL_FACE);		//………………………………………………………………………………Faces visiveis
	glCullFace(GL_BACK);		//………………………………………………………………………………Mostrar so as da frente

	glVertexPointer(3, GL_FLOAT, 0, vertices); //………………………………………Vertex arrays
	glEnableClientState(GL_VERTEX_ARRAY);
	glNormalPointer(GL_FLOAT, 0, normais);
	glEnableClientState(GL_NORMAL_ARRAY);
	glColorPointer(3, GL_FLOAT, 0, cores);
	glEnableClientState(GL_COLOR_ARRAY);

	srand(1);
}


void desenhaTexto(char* string, GLfloat x, GLfloat y) {
	glPushMatrix();
	glTranslatef(x, 0, y);
	glRasterPos2f(0, 0);
	while (*string)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *string++);
	glPopMatrix();
}


void drawEixos() {
	//x
	initMaterials(7); // bronze
	glBegin(GL_LINES);
		glVertex3i(0, 0, 0);
		glVertex3i(10, 0, 0);
	glEnd();
	//y
	initMaterials(0); // emerald
		glBegin(GL_LINES);
		glVertex3i(0, 0, 0);
		glVertex3i(0, 10, 0);
	glEnd();
	//z
	initMaterials(8); // chrome
	glBegin(GL_LINES);
		glVertex3i(0, 0, 0);
		glVertex3i(0, 0, 10);
	glEnd();
}


void drawAcento() {
	//glColor4f(RED);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, y_mesa_cima, 0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(3.0f, y_mesa_cima, 0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(3.0f, y_mesa_cima, 3.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, y_mesa_cima, 3.0f);
		
		glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, y_mesa_baixo, 0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(3.0f, y_mesa_baixo, 0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(3.0f, y_mesa_baixo, 3.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, y_mesa_baixo, 3.0f);
		
		glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, y_mesa_cima, 3.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0f, y_mesa_baixo, 3.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(3.0f, y_mesa_baixo, 3.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(3.0f, y_mesa_cima, 3.0f);
		
		glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, y_mesa_cima, 0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0f, y_mesa_baixo, 0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(3.0f, y_mesa_baixo, 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(3.0f, y_mesa_cima, 0.0f);
		
		glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, y_mesa_cima, 0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0f, y_mesa_baixo, 0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0f, y_mesa_baixo, 3.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, y_mesa_cima, 3.0f);
		
		glTexCoord2f(0.0f, 0.0f); glVertex3f(3.0f, y_mesa_cima, 0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(3.0f, y_mesa_baixo, 0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(3.0f, y_mesa_baixo, 3.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(3.0f, y_mesa_cima, 3.0f);
	glEnd();
}


void drawPernas() {
	//glColor4f(BLUE);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBegin(GL_QUADS);
		//perna 1
		glTexCoord2f(0.0f, 0.0f); glVertex3f(3.0f, 0.0f, 3.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(3.0f, 0.0f, 2.5f);	
		glTexCoord2f(1.0f, 1.0f); glVertex3f(3.0f, y_pernas, 0.0f);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f(3.0f, y_pernas, 0.5f);
		
		glTexCoord2f(0.0f, 0.0f); glVertex3f(3.0f, 0.0f, 3.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(3.0f, 0.0f, 2.5f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(2.5f, 0.0f, 2.5f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(2.5f, 0.0f, 3.0f);
		
		glTexCoord2f(0.0f, 0.0f); glVertex3f(2.5f, 0.0f, 3.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(2.5f, 0.0f, 2.5f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(2.5f, y_pernas, 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(2.5f, y_pernas, 0.5f);
		
		glTexCoord2f(0.0f, 0.0f); glVertex3f(3.0f, 0.0f, 2.5f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(2.5f, 0.0f, 2.5f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(2.5f, y_pernas, 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(3.0f, y_pernas, 0.0f);
		
		glTexCoord2f(0.0f, 0.0f); glVertex3f(3.0f, 0.0f, 3.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(3.0f, 0.0f, 2.5f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(3.0f, y_pernas, 0.5f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(2.5f, y_pernas, 0.5f);

		//perna 2
		glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 3.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0f, 0.0f, 2.5f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0f, y_pernas, 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, y_pernas, 0.5f);
		
		glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 3.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0f, 0.0f, 2.5f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 0.0f, 2.5f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(0.5f, 0.0f, 3.0f);
		
		glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5f, 0.0f, 3.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, 0.0f, 2.5f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, y_pernas, 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(0.5f, y_pernas, 0.5f);
		
		glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 2.5f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, 0.0f, 2.5f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, y_pernas, 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, y_pernas, 0.0f);
		
		glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 3.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0f, 0.0f, 2.5f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, y_pernas, 0.5f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, y_pernas, 0.5f);

		//perna 3
		glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 0.5f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0f, 0.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0f, y_pernas, 2.5f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, y_pernas, 3.0f);
		
		glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 0.5f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0f, 0.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 0.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(0.5f, 0.0f, 0.5f);
		
		glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5f, 0.0f, 0.5f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, 0.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, y_pernas, 2.5f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(0.5f, y_pernas, 3.0f);
		
		glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, 0.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, y_pernas, 2.5f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, y_pernas, 2.5f);
		
		glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 0.5f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0f, 0.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, y_pernas, 3.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, y_pernas, 3.0f);

		//perna 4
		glTexCoord2f(0.0f, 0.0f); glVertex3f(3.0f, 0.0f, 0.5f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(3.0f, 0.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(3.0f, y_pernas, 2.5f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(3.0f, y_pernas, 3.0f);
		
		glTexCoord2f(0.0f, 0.0f); glVertex3f(3.0f, 0.0f, 0.5f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(3.0f, 0.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(2.5f, 0.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(2.5f, 0.0f, 0.5f);
		
		glTexCoord2f(0.0f, 0.0f); glVertex3f(2.5f, 0.0f, 0.5f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(2.5f, 0.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(2.5f, y_pernas, 2.5f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(2.5f, y_pernas, 3.0f);
		
		glTexCoord2f(0.0f, 0.0f); glVertex3f(3.0f, 0.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(2.5f, 0.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(2.5f, y_pernas, 2.5f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(3.0f, y_pernas, 2.5f);
		
		glTexCoord2f(0.0f, 0.0f); glVertex3f(3.0f, 0.0f, 0.5f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(3.0f, 0.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(2.5f, y_pernas, 3.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(3.0f, y_pernas, 3.0f);
	glEnd();
}


void drawChao() {
	glNormal3f(0, 1, 0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texturas[1]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);	glVertex3f(-15.0f, -0.1f, -15.0f);
		glTexCoord2f(1.0f, 0.0f);	glVertex3f(-15.0f, -0.1f, 15.0f);
		glTexCoord2f(1.0f, 1.0f);	glVertex3f(15.0f, -0.1f, 15.0f);
		glTexCoord2f(0.0f, 1.0f);	glVertex3f(15.0f, -0.1f, -15.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}


void drawParedesMalha() {
	initMaterials(materialParede);
	glBegin(GL_QUADS);
		// parede 1
		glVertex3f(-15.0f, -0.1f, -15.0f);
		glVertex3f(-15.0f, -0.1f, -12.0f);
		glVertex3f(-15.0f, 12.0f, -12.0f);
		glVertex3f(-15.0f, 12.0f, -15.0f);
		glVertex3f(-15.0f, -0.1f, -15.0f);
		glVertex3f(-15.0f, -0.1f, -9.0f);
		glVertex3f(-15.0f, 12.0f, -9.0f);
		glVertex3f(-15.0f, 12.0f, -15.0f);
		glVertex3f(-15.0f, -0.1f, -15.0f);
		glVertex3f(-15.0f, -0.1f, -6.0f);
		glVertex3f(-15.0f, 12.0f, -6.0f);
		glVertex3f(-15.0f, 12.0f, -15.0f);
		glVertex3f(-15.0f, -0.1f, -15.0f);
		glVertex3f(-15.0f, -0.1f, -3.0f);
		glVertex3f(-15.0f, 12.0f, -3.0f);
		glVertex3f(-15.0f, 12.0f, -15.0f);
		glVertex3f(-15.0f, -0.1f, -15.0f);
		glVertex3f(-15.0f, -0.1f, 0.0f);
		glVertex3f(-15.0f, 12.0f, 0.0f);
		glVertex3f(-15.0f, 12.0f, -15.0f);
		glVertex3f(-15.0f, -0.1f, -15.0f);
		glVertex3f(-15.0f, -0.1f, 3.0f);
		glVertex3f(-15.0f, 12.0f, 3.0f);
		glVertex3f(-15.0f, 12.0f, -15.0f);
		glVertex3f(-15.0f, -0.1f, -15.0f);
		glVertex3f(-15.0f, -0.1f, 6.0f);
		glVertex3f(-15.0f, 12.0f, 6.0f);
		glVertex3f(-15.0f, 12.0f, -15.0f);
		glVertex3f(-15.0f, -0.1f, -15.0f);
		glVertex3f(-15.0f, -0.1f, 9.0f);
		glVertex3f(-15.0f, 12.0f, 9.0f);
		glVertex3f(-15.0f, 12.0f, -15.0f);
		glVertex3f(-15.0f, -0.1f, -15.0f);
		glVertex3f(-15.0f, -0.1f, 12.0f);
		glVertex3f(-15.0f, 12.0f, 12.0f);
		glVertex3f(-15.0f, 12.0f, -15.0f);
		glVertex3f(-15.0f, -0.1f, -15.0f);
		glVertex3f(-15.0f, -0.1f, 15.0f);
		glVertex3f(-15.0f, 12.0f, 15.0f);
		glVertex3f(-15.0f, 12.0f, -15.0f);
		// parede 2
		glVertex3f(-15.0f, -0.1f, 15.0f);
		glVertex3f(-12.0f, -0.1f, 15.0f);
		glVertex3f(-12.0f, 12.0f, 15.0f);
		glVertex3f(-15.0f, 12.0f, 15.0f);
		glVertex3f(-15.0f, -0.1f, 15.0f);
		glVertex3f(-9.0f, -0.1f, 15.0f);
		glVertex3f(-9.0f, 12.0f, 15.0f);
		glVertex3f(-15.0f, 12.0f, 15.0f);
		glVertex3f(-15.0f, -0.1f, 15.0f);
		glVertex3f(-6.0f, -0.1f, 15.0f);
		glVertex3f(-6.0f, 12.0f, 15.0f);
		glVertex3f(-15.0f, 12.0f, 15.0f);
		glVertex3f(-15.0f, -0.1f, 15.0f);
		glVertex3f(-3.0f, -0.1f, 15.0f);
		glVertex3f(-3.0f, 12.0f, 15.0f);
		glVertex3f(-15.0f, 12.0f, 15.0f);
		glVertex3f(-15.0f, -0.1f, 15.0f);
		glVertex3f(0.0f, -0.1f, 15.0f);
		glVertex3f(0.0f, 12.0f, 15.0f);
		glVertex3f(-15.0f, 12.0f, 15.0f);
		glVertex3f(-15.0f, -0.1f, 15.0f);
		glVertex3f(3.0f, -0.1f, 15.0f);
		glVertex3f(3.0f, 12.0f, 15.0f);
		glVertex3f(-15.0f, 12.0f, 15.0f);
		glVertex3f(-15.0f, -0.1f, 15.0f);
		glVertex3f(6.0f, -0.1f, 15.0f);
		glVertex3f(6.0f, 12.0f, 15.0f);
		glVertex3f(-15.0f, 12.0f, 15.0f);
		glVertex3f(-15.0f, -0.1f, 15.0f);
		glVertex3f(9.0f, -0.1f, 15.0f);
		glVertex3f(9.0f, 12.0f, 15.0f);
		glVertex3f(-15.0f, 12.0f, 15.0f);
		glVertex3f(-15.0f, -0.1f, 15.0f);
		glVertex3f(12.0f, -0.1f, 15.0f);
		glVertex3f(12.0f, 12.0f, 15.0f);
		glVertex3f(-15.0f, 12.0f, 15.0f);
		glVertex3f(-15.0f, -0.1f, 15.0f);
		glVertex3f(15.0f, -0.1f, 15.0f);
		glVertex3f(15.0f, 12.0f, 15.0f);
		glVertex3f(-15.0f, 12.0f, 15.0f);
		// parede 3
		glVertex3f(15.0f, -0.1f, -12.0f);
		glVertex3f(15.0f, -0.1f, -15.0f);
		glVertex3f(15.0f, 12.0f, -15.0f);
		glVertex3f(15.0f, 12.0f, -12.0f);
		glVertex3f(15.0f, -0.1f, -9.0f);
		glVertex3f(15.0f, -0.1f, -15.0f);
		glVertex3f(15.0f, 12.0f, -15.0f);
		glVertex3f(15.0f, 12.0f, -9.0f);
		glVertex3f(15.0f, -0.1f, -6.0f);
		glVertex3f(15.0f, -0.1f, -15.0f);
		glVertex3f(15.0f, 12.0f, -15.0f);
		glVertex3f(15.0f, 12.0f, -6.0f);
		glVertex3f(15.0f, -0.1f, -3.0f);
		glVertex3f(15.0f, -0.1f, -15.0f);
		glVertex3f(15.0f, 12.0f, -15.0f);
		glVertex3f(15.0f, 12.0f, -3.0f);
		glVertex3f(15.0f, -0.1f, 0.0f);
		glVertex3f(15.0f, -0.1f, -15.0f);
		glVertex3f(15.0f, 12.0f, -15.0f);
		glVertex3f(15.0f, 12.0f, 0.0f);
		glVertex3f(15.0f, -0.1f, 3.0f);
		glVertex3f(15.0f, -0.1f, -15.0f);
		glVertex3f(15.0f, 12.0f, -15.0f);
		glVertex3f(15.0f, 12.0f, 3.0f);
		glVertex3f(15.0f, -0.1f, 6.0f);
		glVertex3f(15.0f, -0.1f, -15.0f);
		glVertex3f(15.0f, 12.0f, -15.0f);
		glVertex3f(15.0f, 12.0f, 6.0f);
		glVertex3f(15.0f, -0.1f, 9.0f);
		glVertex3f(15.0f, -0.1f, -15.0f);
		glVertex3f(15.0f, 12.0f, -15.0f);
		glVertex3f(15.0f, 12.0f, 9.0f);
		glVertex3f(15.0f, -0.1f, 12.0f);
		glVertex3f(15.0f, -0.1f, -15.0f);
		glVertex3f(15.0f, 12.0f, -15.0f);
		glVertex3f(15.0f, 12.0f, 12.0f);
		glVertex3f(15.0f, -0.1f, 15.0f);
		glVertex3f(15.0f, -0.1f, -15.0f);
		glVertex3f(15.0f, 12.0f, -15.0f);
		glVertex3f(15.0f, 12.0f, 15.0f);
		// parede 4
		glVertex3f(-12.0f, -0.1f, -15.0f);
		glVertex3f(-15.0f, -0.1f, -15.0f);
		glVertex3f(-15.0f, 12.0f, -15.0f);
		glVertex3f(-12.0f, 12.0f, -15.0f);
		glVertex3f(-9.0f, -0.1f, -15.0f);
		glVertex3f(-15.0f, -0.1f, -15.0f);
		glVertex3f(-15.0f, 12.0f, -15.0f);
		glVertex3f(-9.0f, 12.0f, -15.0f);
		glVertex3f(-6.0f, -0.1f, -15.0f);
		glVertex3f(-15.0f, -0.1f, -15.0f);
		glVertex3f(-15.0f, 12.0f, -15.0f);
		glVertex3f(-6.0f, 12.0f, -15.0f);
		glVertex3f(-3.0f, -0.1f, -15.0f);
		glVertex3f(-15.0f, -0.1f, -15.0f);
		glVertex3f(-15.0f, 12.0f, -15.0f);
		glVertex3f(-3.0f, 12.0f, -15.0f);
		glVertex3f(0.0f, -0.1f, -15.0f);
		glVertex3f(-15.0f, -0.1f, -15.0f);
		glVertex3f(-15.0f, 12.0f, -15.0f);
		glVertex3f(0.0f, 12.0f, -15.0f);
		glVertex3f(3.0f, -0.1f, -15.0f);
		glVertex3f(-15.0f, -0.1f, -15.0f);
		glVertex3f(-15.0f, 12.0f, -15.0f);
		glVertex3f(3.0f, 12.0f, -15.0f);
		glVertex3f(6.0f, -0.1f, -15.0f);
		glVertex3f(-15.0f, -0.1f, -15.0f);
		glVertex3f(-15.0f, 12.0f, -15.0f);
		glVertex3f(6.0f, 12.0f, -15.0f);
		glVertex3f(9.0f, -0.1f, -15.0f);
		glVertex3f(-15.0f, -0.1f, -15.0f);
		glVertex3f(-15.0f, 12.0f, -15.0f);
		glVertex3f(9.0f, 12.0f, -15.0f);
		glVertex3f(12.0f, -0.1f, -15.0f);
		glVertex3f(-15.0f, -0.1f, -15.0f);
		glVertex3f(-15.0f, 12.0f, -15.0f);
		glVertex3f(12.0f, 12.0f, -15.0f);
		glVertex3f(15.0f, -0.1f, -15.0f);
		glVertex3f(-15.0f, -0.1f, -15.0f);
		glVertex3f(-15.0f, 12.0f, -15.0f);
		glVertex3f(15.0f, 12.0f, -15.0f);
	glEnd();
}


void drawParedes() {
	glNormal3f(0, 1, 0);
	initMaterials(materialParede);
	glBegin(GL_QUADS);
	// parede 1
	glVertex3f(-15.0f, -0.1f, -15.0f);
	glVertex3f(-15.0f, -0.1f, 15.0f);
	glVertex3f(-15.0f, 12.0f, 15.0f);
	glVertex3f(-15.0f, 12.0f, -15.0f);
	// parede 2
	glVertex3f(-15.0f, -0.1f, 15.0f);
	glVertex3f(15.0f, -0.1f, 15.0f);
	glVertex3f(15.0f, 12.0f, 15.0f);
	glVertex3f(-15.0f, 12.0f, 15.0f);
	// parede 3
	glVertex3f(15.0f, -0.1f, 15.0f);
	glVertex3f(15.0f, -0.1f, -15.0f);
	glVertex3f(15.0f, 12.0f, -15.0f);
	glVertex3f(15.0f, 12.0f, 15.0f);
	// parede 4
	glVertex3f(15.0f, -0.1f, -15.0f);
	glVertex3f(-15.0f, -0.1f, -15.0f);
	glVertex3f(-15.0f, 12.0f, -15.0f);
	glVertex3f(15.0f, 12.0f, -15.0f);
	glEnd();
}


void drawTeto() {
	initMaterials(16); // white plastic
	glBegin(GL_QUADS);
		glVertex3f(-15.0f, 12.0f, -15.0f);
		glVertex3f(-15.0f, 12.0f, 15.0f);
		glVertex3f(15.0f, 12.0f, 15.0f);
		glVertex3f(15.0f, 12.0f, -15.0f);
	glEnd();
}


void drawEsfera() {
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glBindTexture(GL_TEXTURE_2D, texturas[3]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glPushMatrix();
		glTranslatef(5, 0.5, 5);
		gluQuadricDrawStyle(esfera, GLU_FILL);
		gluQuadricNormals(esfera, GLU_SMOOTH);
		gluQuadricTexture(esfera, GL_TRUE);
		gluSphere(esfera, 0.5, 150, 150);
	glPopMatrix();
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}


void drawScene() {	
	//=================================================== VISIVEL
	glEnable(GL_CULL_FACE);		//………………………………………………………………………………Faces visiveis
	if (frenteVisivel == 0)		glCullFace(GL_BACK);  //glFrontFace(GL_CW);
	if (frenteVisivel == 1)		glCullFace(GL_FRONT);  //glFrontFace(GL_CCW);
	if (frenteVisivel == 2)  	glDisable(GL_CULL_FACE);		//………………………………………………………………………………Faces visiveis
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texturas[0]);
	glPushMatrix();
		glTranslatef(trans_x, 0, trans_z);
		glRotatef(angulo, 0, 1, 0);
		drawAcento();
		drawPernas();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	drawChao();
	if (malha == 1) {
		drawParedesMalha();
	}
	else {
		drawParedes();
	}
	drawTeto();
	drawEsfera();
}


void iluminacao() {
	if (teto) {
		glEnable(GL_LIGHT0);
	}
	else {
		glDisable(GL_LIGHT0);
	}
	if (foco1) {
		glEnable(GL_LIGHT2);
	}
	else {
		glDisable(GL_LIGHT2);
	}
	if (foco2) {
		glEnable(GL_LIGHT3);
	}
	else {
		glDisable(GL_LIGHT3);
	}
	if (foco3) {
		glEnable(GL_LIGHT4);
	}
	else {
		glDisable(GL_LIGHT4);
	}
	if (foco4) {
		glEnable(GL_LIGHT5);
	}
	else {
		glDisable(GL_LIGHT5);
	}
}

void desenhaInfo(char* string, GLfloat x, GLfloat y, GLfloat z) {
	glRasterPos3f(x, y, z);
	while (*string)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *string++);
}

void display(void) {

	//================================================================= APaga 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Viewport 0 para a informação
	glDisable(GL_LIGHTING);
	glViewport(0, 400, wScreen / 6, hScreen / 5);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-16.0, 16.0, -16.0, 16.0, -15.0, 15.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 5, 0, 0, 0, 0, 0, 0, -1);

	glColor3f(1, 1, 1);
	sprintf_s(textoInfo, 30, "Luz Teto 'L' - %d", teto);
	desenhaInfo(textoInfo, -12, 1, -8);
	sprintf_s(textoInfo, 30, "  Luz 'RGB' - %d%d%d", luzR, luzG, luzB);
	desenhaInfo(textoInfo, -12, 1, -5);
	sprintf_s(textoInfo, 30, "  Intensidade 'I' - %4.2f", intensidadeTeto);
	desenhaInfo(textoInfo, -12, 1, -2);
	sprintf_s(textoInfo, 30, "Material 'P' - %s", Materiais[materialParede]);
	desenhaInfo(textoInfo, -12, 1, 1);
	sprintf_s(textoInfo, 30, "Foco '1' - %d", foco1);
	desenhaInfo(textoInfo, -12, 1, 4);
	sprintf_s(textoInfo, 30, "Foco '2' - %d", foco2);
	desenhaInfo(textoInfo, -12, 1, 7);
	sprintf_s(textoInfo, 30, "Foco '3' - %d", foco3);
	desenhaInfo(textoInfo, -12, 1, 10);
	sprintf_s(textoInfo, 30, "Foco '4' - %d", foco4);
	desenhaInfo(textoInfo, -12, 1, 13);
	sprintf_s(textoInfo, 30, "Malha 'C' - %d", malha);
	desenhaInfo(textoInfo, -12, 1, 16);

	//================================================================= Viewport 1 MAPA
	// projecao ortogonal
	glEnable(GL_LIGHTING);
	glViewport(0, 0, 0.25 * wScreen, 0.25 * hScreen);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-20, 20, -20, 20, -100, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 10, 0, 0, 0, 0, 0, 0, -1);
	//…………………………………………………………………………………………………………………………………………………………Objectos
	drawEixos();
	drawScene();

	//================================================================= Viewport 2
	glEnable(GL_LIGHTING);
	glViewport(0.25 * wScreen, 0.25 * hScreen, 0.75 * wScreen, 0.75 * hScreen);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (tipoProjeccao == 0)
		gluPerspective(anguloZ, (float)wScreen / hScreen, 0.1, 9999);
	if (tipoProjeccao == 1)
		glOrtho(-20, 20, -20, 20, -100, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(obsPini[0], obsPini[1], obsPini[2], obsPfin[0], obsPfin[1], obsPfin[2], 0, 1, 0);

	//Luz e iluminação
	iluminacao();

	//Rotações dos eixos face ao observador
	glRotatef(roda_x, 1.0f, 0.0f, 0.0f);
	glRotatef(roda_y, 0.0f, 1.0f, 0.0f);
	glRotatef(roda_z, 0.0f, 0.0f, 1.0f);
	
	// Eixos e Objetos
	drawEixos();
	drawScene();

	// Atualizacao
	glutSwapBuffers();
}


void Timer(int value) {
	glutPostRedisplay();
	glutTimerFunc(msec, Timer, 1);
}


void updateLuz() {
	localCorAmbiente[0] = luzR * intensidadeTeto;
	localCorAmbiente[1] = luzG * intensidadeTeto;
	localCorAmbiente[2] = luzB * intensidadeTeto;
	localCorDifusa[0] = luzR * intensidadeTeto;
	localCorDifusa[1] = luzG * intensidadeTeto;
	localCorDifusa[2] = luzB * intensidadeTeto;
	localCorEspecular[0] = luzR * intensidadeTeto;
	localCorEspecular[1] = luzG * intensidadeTeto;
	localCorEspecular[2] = luzB * intensidadeTeto;
	glLightfv(GL_LIGHT0, GL_AMBIENT, localCorAmbiente);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, localCorDifusa);
	glLightfv(GL_LIGHT0, GL_SPECULAR, localCorEspecular);
}


void keyboard(unsigned char key, int x, int y) {
	switch (key) {
		case 'f': case 'F':
			frenteVisivel = (frenteVisivel + 1) % 3;
			glutPostRedisplay();
			break;

		// translação do banco
		case 'w': case 'W':
			trans_z -= 0.5;
			glutPostRedisplay();
			break;
		case 'a': case 'A':
			trans_x -= 0.5;
			glutPostRedisplay();
			break;
		case 's': case 'S':
			trans_z += 0.5;
			glutPostRedisplay();
			break;
		case 'd': case 'D':
			trans_x += 0.5;
			break;

		//rotação do banco
		case 'e': case 'E':
			angulo += 5;
			glutPostRedisplay();
			break;
		case 't': case 'T':
			angulo -= 5;
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

		// rotacao de 10 graus com os diferentes eixos
		case 'x': case 'X':
			roda_x += 10;
			glutPostRedisplay();
			break;
		case 'y': case 'Y':
			roda_y += 10;
			glutPostRedisplay();
			break;
		case 'z': case 'Z':
			roda_z += 10;
			glutPostRedisplay();
			break;

		// alternar entre projeção perspetiva e ortogonal
		case 'o': 	case 'O':
			tipoProjeccao = (tipoProjeccao + 1) % 2;
			glutPostRedisplay();
			break;

		case 'p':	case 'P':
			materialParede = (materialParede + 1) % 18;
			initMaterials(materialParede);
			glutPostRedisplay();
			break;
		/*luz ambiente (efeito dia/noite)
		case 'l':	case 'L':
			dia = !dia;
			if (dia) {
				luzAmbiente[0] = 0.8f;
				luzAmbiente[1] = 0.8f;
				luzAmbiente[2] = 0.8f;
			}
			else {
				luzAmbiente[0] = 0.2f;
				luzAmbiente[1] = 0.2f;
				luzAmbiente[2] = 0.2f;
			}
			glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);
			glutPostRedisplay();
			break;
		*/
		//luz teto
		case 'l':	case 'L':
			teto = !teto;
			updateLuz();
			glutPostRedisplay();
			break;
		//iluminação sala
		case 'i':	case 'I':
			intensidadeTeto = intensidadeTeto + 0.1;
			if (intensidadeTeto > 1.1) intensidadeTeto = 0.1;
			updateLuz();
			glutPostRedisplay();
			break;
		case 'r':	case 'R':
			luzR = (luzR + 1) % 2;
			updateLuz();
			glutPostRedisplay();
			break;
		case 'g':	case 'G':
			luzG = (luzG + 1) % 2;
			updateLuz();
			glutPostRedisplay();
			break;
		case 'b':	case 'B':
			luzB = (luzB + 1) % 2;
			updateLuz();
			glutPostRedisplay();
			break;
		// luz focos
		case '1':
			foco1 = !foco1;
			glutPostRedisplay();
			break;
		case '2':
			foco2 = !foco2;
			glutPostRedisplay();
			break;
		case '3':
			foco3 = !foco3;
			glutPostRedisplay();
			break;
		case '4':
			foco4 = !foco4;
			glutPostRedisplay();
			break;
		case 'c':	case 'C':
			malha = !malha;
			glutPostRedisplay();
			break;

		// escape
		case 27:
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

//======================================================= MAIN
int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(wScreen, hScreen);
	glutInitWindowPosition(400, 100);
	glutCreateWindow("Projeto CG - Banco");

	initialize();

	glutSpecialFunc(teclasNotAscii);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(msec, Timer, 1);

	glutMainLoop();

	return 0;
}