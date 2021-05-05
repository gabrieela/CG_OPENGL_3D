/* 
Trabalho de Computação Gráfica sobre objetos 3D 
Universidade Federal do Pará
Grupo:
  Gabriela da Silva de Souza - 201706840035
  Marcos Matheus Rodrigues Sá de Souza - 201706840001
  Ulrich Kauê Mendes Alencar da Silva - 201706840013
*/ 

#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include <GL/glut.h>
#include <SOIL.h>

// define o valor de PI para uso nas esferas
#define PI 3.1415 

// Definição de variáveis utilizadas ao longo do código
GLuint texture[0];
GLint WIDTH = 320;
GLint HEIGHT = 240; 
GLfloat obs[3] = {0.0,7.0,0.0};
GLfloat olho[3] = {0.0,3.0,0.0};

// Define o número de fatias (longitude), o número de pilhas (latitude) e o raio das esferas da cena
GLint fatias=25;
GLint pilhas=25;
GLint raioEsfera=1.75;

// Indica a posição e as componente de luz para reflexão difusa e especular e a componente para reflexão ambiente dos dois pontos de luz presentes no código
GLfloat posicao_luz0[]    = { 0.0, 10.0, 0.0, 1.0};
GLfloat cor_luz0[]        = { 1.0, 1.0, 1.0, 1.0};
GLfloat cor_luz0_amb[]    = { 0.5, 0.5, 0.5, 1.0};

GLfloat mat_a_difusa[]    = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_a_especular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_a_brilho[]    = { 50.0 };

GLfloat mat_b_difusa[]    = { 0.7, 0.7, 0.7, 1.5 };
GLfloat mat_b_especular[] = { 1.0, 1.0, 1.0, 1.5 };
GLfloat mat_b_brilho[]    = { 50.0 };

GLfloat posicao_luz1[]    = { 0.0, 10.0, 5.0, 1.0};
GLfloat cor_luz1[]        = { 0.0, 0.0, 1.0, 1.0};
GLfloat direcao_luz1[]    = { 0.0, -10.0, -5.0, 1.0};
GLint   spot_luz1         = 30;

GLfloat sem_cor[]         = { 0.0, 0.0, 0.0, 1.0};

GLfloat tetaxz=0;
GLfloat raioxz=6;

void reshape(int width, int height)
{
  WIDTH = width;
  HEIGHT = height;
  
  glViewport(0,0,(GLint)width,(GLint)height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(70.0,width/(float)height,0.1,30.0);
  glMatrixMode(GL_MODELVIEW);
}

// Função responsável por definir a janela e seus atributos, como cor do fundo e tamanho
void display(void)
{
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);

  glDepthMask(GL_TRUE);
  glClearColor(0.0,0.0,0.0,0.0);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  glPushMatrix();
  
  // Calculando a posicao do observador
  obs[0] = raioxz*cos(2*PI*tetaxz/360);
  obs[2] = raioxz*sin(2*PI*tetaxz/360);

  gluLookAt(obs[0],obs[1],obs[2],olho[0],olho[1],olho[2],0.0,1.0,0.0);

  // Recebendo a imagem png que será usada como textura com auxílio da bibliteca SOIL
  texture[0] = SOIL_load_OGL_texture 
    (
      "xadrez.png",
      SOIL_LOAD_AUTO,
      SOIL_CREATE_NEW_ID,
      SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
    );
    
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture[0]);
    
  // Desenhando o plano
  glNormal3f(0,1,0);
  glBegin(GL_QUADS);
  glTexCoord3f(0, 0, 0);
  glVertex3f(-10,0,10);
  glTexCoord3f(1, 0, 0);
  glVertex3f(10,0,10);
  glTexCoord3f(1, 1, 0);
  glVertex3f(10,0,-10);
  glTexCoord3f(0, 1, 0); 
  glVertex3f(-10,0,-10);
  glEnd();

  // Definição de duas esferas para serem os pontos de onde são emitidas as luzes, 
  // e as duas esferas que ficarão no plano
  glPushMatrix();
  glTranslatef(posicao_luz0[0],posicao_luz0[1],posicao_luz0[2]);
  glColor3f(1,0,0);
  glMaterialfv(GL_FRONT, GL_EMISSION, cor_luz0);
  glutSolidSphere(0.3,5,5);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(posicao_luz1[0],posicao_luz1[1],posicao_luz1[2]);
  glMaterialfv(GL_FRONT, GL_EMISSION, cor_luz1);
  glutSolidSphere(0.3,5,5);
  glPopMatrix();

  glMaterialfv(GL_FRONT, GL_EMISSION, sem_cor);

  glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_a_difusa);
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_a_especular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_a_brilho);

  glPushMatrix();
  glTranslatef(0.0,3.0,-3.0);
  glutSolidSphere(raioEsfera,fatias,pilhas);
  glPopMatrix();

  glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_b_difusa);
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_b_especular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_b_brilho);

  glTranslatef(0.0,+3.0,+3.0);

  glScalef(1.5f, 1.5f, 1.5f); 
  glutSolidSphere(raioEsfera,fatias,pilhas);

  glPopMatrix();
  glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y){
  switch (key) {
  case 27:
    exit(0);
    break;
  case 'r':
    raioxz=raioxz+1;
    glutPostRedisplay();
    break;
  case 'R':
    if(raioxz>1){
      raioxz=raioxz-1;
      glutPostRedisplay();
    }
    break;
  }
}

void special(int key, int x, int y){
  switch (key) {
    case GLUT_KEY_UP:
      obs[1]=obs[1]+1;
      glutPostRedisplay();
      break;
    case GLUT_KEY_DOWN:
      obs[1] =obs[1]-1;
      glutPostRedisplay();
      break;
    case GLUT_KEY_LEFT:
      tetaxz=tetaxz+2;
      glutPostRedisplay();
      break;
    case GLUT_KEY_RIGHT:
      tetaxz=tetaxz-2;
      glutPostRedisplay();
      break;
  }
}

void init(){
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);


  // Define a posição, orientação, ângulo de corte e as componente de cor para a fonte de luz
  glLightfv(GL_LIGHT0, GL_DIFFUSE, cor_luz0);
  glLightfv(GL_LIGHT0, GL_SPECULAR, cor_luz0);
  glLightfv(GL_LIGHT0, GL_AMBIENT, cor_luz0_amb);
  glLightfv(GL_LIGHT0, GL_POSITION, posicao_luz0);

  glLightfv(GL_LIGHT1, GL_DIFFUSE, cor_luz1);
  glLightfv(GL_LIGHT1, GL_SPECULAR, cor_luz1);
  glLightf (GL_LIGHT1, GL_SPOT_CUTOFF, spot_luz1);
  glLightfv(GL_LIGHT1, GL_POSITION, posicao_luz1);
  glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direcao_luz1);


  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);

  glEnable(GL_AUTO_NORMAL);
  glEnable(GL_NORMALIZE);
}

// Função principal responsável por chamar as outras função e iniciar o programa em OpenGL
int main(int argc,char **argv){
  glutInitWindowPosition(0,0);
  glutInitWindowSize(720, 480);
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_RGB|GLUT_DEPTH|GLUT_DOUBLE);
  glutCreateWindow("Trabalho de objetos 3D");
  init();
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);  
  glutSpecialFunc(special);
  glEnable(GL_DEPTH_TEST);
  glutMainLoop();
  return(0);
}


