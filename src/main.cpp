//
// Created by Bruno on 15/05/2018.
//
#include <iostream>
#include <GL/glut.h>
#include <spider.h>
#include <types.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>

using namespace cv;
using namespace std;

#define FRAMES 24

int width = 800;
int height = 600;
Spider *s;
bool fog = false;
Mat groundTexture;
bool grounded = false;
bool skyBoxed = false;
GLuint *textures;

/**
 * @desc Desenha eixos de um sistema de coordenadas.
 * @param {float*} basePoint Ponto de origem de um sistema de coordenadas.
 * @param {float*} i Primeiro versor.
 * @param {float*} j Segundo versor.
 * @param {float*} k Terceiro versor.
 */
void drawAxes(float *basePoint, float *i, float *j, float *k){
    float currentColor[4];
    /** Armazena cor atual */
    glGetFloatv(GL_CURRENT_COLOR, currentColor);
    /** Desenha versores */
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINES);
    glVertex3f(basePoint[0], basePoint[1], basePoint[2]);
    glVertex3f(i[0], i[1], i[2]);
    glEnd();
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_LINES);
    glVertex3f(basePoint[0], basePoint[1], basePoint[2]);
    glVertex3f(j[0], j[1], j[2]);
    glEnd();
    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_LINES);
    glVertex3f(basePoint[0], basePoint[1], basePoint[2]);
    glVertex3f(k[0], k[1], k[2]);
    glEnd();
    /** Retorna para cor anterior */
    glColor3f(currentColor[0], currentColor[1], currentColor[2]);
}

point pointify(GLfloat x, GLfloat y, GLfloat z) {
   point p;
   p.x = x;
   p.y = y;
   p.z = z;
   return p;
}

void setSkybox() {
   vector<string> faces;//nomes dos arquivos
   vector<GLenum> faces_index;//tipo da textura cube map
   glBindTexture(GL_TEXTURE_CUBE_MAP, textures[2]);
   glEnable(GL_TEXTURE_CUBE_MAP);

   faces_index.push_back(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z); //front
   faces.push_back("images/front.png");
   faces_index.push_back(GL_TEXTURE_CUBE_MAP_POSITIVE_Z); //back
   faces.push_back("images/back.png");
   faces_index.push_back(GL_TEXTURE_CUBE_MAP_POSITIVE_X); //right
   faces.push_back("images/right.png");
   faces_index.push_back(GL_TEXTURE_CUBE_MAP_NEGATIVE_X); //left
   faces.push_back("images/left.png");
   faces_index.push_back(GL_TEXTURE_CUBE_MAP_POSITIVE_Y); //top
   faces.push_back("images/top.png");
   faces_index.push_back(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y); //bottom
   faces.push_back("images/bottom.png");

   for (int i = 0; i < faces.size(); i++) {
      Mat img = imread(faces[i].c_str());
      if (!img.data) {
         cout<<"could not load skybox\n";
         return;
      }
      glTexImage2D(faces_index[i], 0, GL_RGBA, img.cols, img.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, img.ptr());
   }
   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
   glDisable(GL_TEXTURE_CUBE_MAP);
}

void drawSkybox(GLfloat tam) {
   /*front*/
   glBegin(GL_QUADS);
   glTexCoord3f(-tam,  tam, -tam);
   glVertex3f(-tam,  tam, -tam);
   glTexCoord3f(-tam, -tam, -tam);
   glVertex3f(-tam, -tam, -tam);
   glTexCoord3f(tam, -tam, -tam);
   glVertex3f(tam, -tam, -tam);
   glTexCoord3f(tam,  tam, -tam);
   glVertex3f(tam,  tam, -tam);
   glTexCoord3f(-tam,  tam, -tam);
   glVertex3f(-tam,  tam, -tam);
   glEnd();

   /**/
   glBegin(GL_QUADS);
   glTexCoord3f(-tam, -tam,  tam);
   glVertex3f(-tam, -tam,  tam);
   glTexCoord3f(-tam, -tam, -tam);
   glVertex3f(-tam, -tam, -tam);
   glTexCoord3f(-tam,  tam, -tam);
   glVertex3f(-tam,  tam, -tam);
   glTexCoord3f(-tam,  tam,  tam);
   glVertex3f(-tam,  tam,  tam);
   glTexCoord3f(-tam, -tam,  tam);
   glVertex3f(-tam, -tam,  tam);
   glEnd();

   /**/
   glBegin(GL_QUADS);
   glTexCoord3f(tam, -tam, -tam);
   glVertex3f(tam, -tam, -tam);
   glTexCoord3f(tam, -tam,  tam);
   glVertex3f(tam, -tam,  tam);
   glTexCoord3f(tam,  tam,  tam);
   glVertex3f(tam,  tam,  tam);
   glTexCoord3f(tam,  tam, -tam);
   glVertex3f(tam,  tam, -tam);
   glTexCoord3f(tam, -tam, -tam);
   glVertex3f(tam, -tam, -tam);
   glEnd();

   /**/
   glBegin(GL_QUADS);
   glTexCoord3f(-tam, -tam,  tam);
   glVertex3f(-tam, -tam,  tam);
   glTexCoord3f(-tam,  tam,  tam);
   glVertex3f(-tam,  tam,  tam);
   glTexCoord3f(tam,  tam,  tam);
   glVertex3f(tam,  tam,  tam);
   glTexCoord3f(tam, -tam,  tam);
   glVertex3f(tam, -tam,  tam);
   glTexCoord3f(-tam, -tam,  tam);
   glVertex3f(-tam, -tam,  tam);
   glEnd();

   /**/
   glBegin(GL_QUADS);
   glTexCoord3f(-tam,  tam, -tam);
   glVertex3f(-tam,  tam, -tam);
   glTexCoord3f(tam,  tam, -tam);
   glVertex3f(tam,  tam, -tam);
   glTexCoord3f(tam,  tam,  tam);
   glVertex3f(tam,  tam,  tam);
   glTexCoord3f(-tam,  tam,  tam);
   glVertex3f(-tam,  tam,  tam);
   glTexCoord3f(-tam,  tam, -tam);
   glVertex3f(-tam,  tam, -tam);
   glEnd();

   /**/
   glBegin(GL_QUADS);
   glTexCoord3f(-tam, -tam, -tam);
   glVertex3f(-tam, -tam, -tam);
   glTexCoord3f(-tam, -tam,  tam);
   glVertex3f(-tam, -tam,  tam);
   glTexCoord3f(tam, -tam, -tam);
   glVertex3f(tam, -tam, -tam);
   glTexCoord3f(-tam, -tam,  tam);
   glVertex3f(-tam, -tam,  tam);
   glTexCoord3f(tam, -tam,  tam);
   glVertex3f(tam, -tam,  tam);
   glEnd();
}


//chamada nos mesmos pontos q drawGrid
void skybox(float size) {
   if (!skyBoxed) {
      setSkybox();
      skyBoxed = true;
   }
   glColor3f(1.0, 1.0, 1.0);
   glPushMatrix();
   glBindTexture(GL_TEXTURE_CUBE_MAP, textures[2]);
   glEnable(GL_TEXTURE_CUBE_MAP);

   drawSkybox(100);

   glPopMatrix();
   glDisable(GL_TEXTURE_CUBE_MAP);
}

void setGroundTexture() {
   glBindTexture(GL_TEXTURE_2D, textures[0]);
   glEnable(GL_TEXTURE_2D);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, groundTexture.cols, groundTexture.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, groundTexture.data);
   glDisable(GL_TEXTURE_2D);
}

/**
 * @desc Desenha um plano em formato de grade - adaptado do programa de matrix de projeção, de Song Ho Ahn (http://www.songho.ca/opengl/gl_transform.html).
 * @param {float} size Define o tamanho da grade.
 * @param {float} step Especifica a quantidade de intervalos de grades, causando a impressão de pisos no chão.
 */
void drawGrid(float size, float step) {
   if (!grounded) {
      setGroundTexture();
      grounded = true;
   }
   glColor3f(1.0, 1.0, 1.0);
   glPushMatrix();

   glBindTexture(GL_TEXTURE_2D, textures[0]);
   glEnable(GL_TEXTURE_2D);
   glBegin(GL_QUAD_STRIP);
   glTexCoord2f(0, 682);
   glVertex3f(-size, 0.0, -size);

   glTexCoord2f(1023, 682);
   glVertex3f(size, 0.0, -size);

   glTexCoord2f(1023, 0.5*682);
   glVertex3f(size, 0.0, 0.0);

   glTexCoord2f(0, 0.5*682);
   glVertex3f(-size, 0.0, 0.0);

   glTexCoord2f(0, 0);
   glVertex3f(-size, 0.0, size);

   glTexCoord2f(1023, 0);
   glVertex3f(size, 0.0, size);

   glTexCoord2f(1023, 0.5*682);
   glVertex3f(size, 0.0, 0.0);

   glTexCoord2f(0.5*1023, 0.5*682);
   glVertex3f(0, 0.0, 0);
   glEnd();

   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}

/**
 * @desc Desenha as coordenadas globais.
 */
void drawWCAxes()
{
   float *basePoint, *i, *j, *k;
   basePoint = new float[3];
   basePoint[0] = basePoint[1] = basePoint[2] = 0.0;
   i = new float[3];
   i[0] = 5.0;
   i[1] = i[2] = 0.0;
   j = new float[3];
   j[0] = j[2] = 0.0;
   j[1] = 5.0;
   k = new float[3];
   k[0] = k[1] = 0.0;
   k[2] = 5.0;
   drawAxes(basePoint, i, j, k);
}

/**
 * @desc Função de callback para desenho na tela.
 */
void displayCallback()
{
   /** Limpa a janela APENAS uma vez */
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glColor3f(1.0f, 1.0f, 1.0f);

   double x,y,z;
   x = s->getPosition()->x;
   y = s->getPosition()->y;
   z = s->getPosition()->z;

   /** Desenha a janela mais a esquerda */
   glViewport(0, 0, width/2, height/2);
   glLoadIdentity();
   gluLookAt(3.0 + x, 2.0 + y, 10.0 + z, x, y, z, 0.0, 1.0, 0.0);
   drawGrid(300, 0.7);
   skybox(300);
   drawWCAxes();
   s->draw();

   glViewport(width/2, 0, width/2, height/2);
   glLoadIdentity();
   gluLookAt(0.0 + x, 10.0 + y, 0.0 + z, x, y, z, 1.0, 0.0, 0.0);
   drawGrid(300, 0.7);
   skybox(300);
   drawWCAxes();
   s->draw();

   glViewport(0, height/2, width/2, height/2);
   glLoadIdentity();
   gluLookAt(10.0 + x, 0.0 + y, 0.0 + z, x, y, z, 0.0, 1.0, 0.0);
   drawGrid(300, 0.7);
   skybox(300);
   drawWCAxes();
   s->draw();

   glViewport(width/2, height/2, width/2, height/2);
   glLoadIdentity();
   gluLookAt(0.0 + x, 0.0 + y, 10.0 + z, x, y, z, 0.0, 1.0, 0.0);
   drawGrid(300, 0.7);
   skybox(300);
   drawWCAxes();
   s->draw();

   /** Dispara os comandos APENAS uma vez */
   glFlush();
}

/**
 * @desc Função de callback para reshape.
 * @param {int} w Nova largura da janela.
 * @param {int} h Nova altura da janela.
 */
void reshapeCallback(int w, int h)
{
   /** Atualiza os valores da janela */
   width = w;
   height = h;
   /** Define o volume de vista */
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(65.0, (GLfloat) width/(GLfloat) height, 1.0, 600.0);
   glMatrixMode(GL_MODELVIEW);
}

void special_callback(int key, int x, int y){
   if(key == GLUT_KEY_UP)
      s->move(true);
   if(key == GLUT_KEY_DOWN)
      s->move(false);
   if(key == GLUT_KEY_LEFT)
      s->rotate(false);
   if(key == GLUT_KEY_RIGHT)
      s->rotate(true);

   // displayCallback();

}

void keyboard_callback(unsigned char key, int x, int y){
   if(key == 'n'){
      if(fog) glDisable(GL_FOG);
      else glEnable(GL_FOG);

      fog = !fog;
   }

}

void update(int a){
   glutPostRedisplay();
   displayCallback();
   glutTimerFunc(1000/FRAMES, update, 0);
}

void lighting(){
   GLfloat position_type[] = {0, 80, 0, 0};
   glLightfv(GL_LIGHT0, GL_POSITION, position_type);

   glEnable(GL_COLOR_MATERIAL);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);

}

void setFog(){
   GLfloat color[] = {0.5, 0.5, 0.5, 1.0};

   glFogf(GL_FOG_MODE, GL_EXP);
   glFogfv(GL_FOG_COLOR, color);
   //glFogf(GL_FOG_START, 0);
   //glFogf(GL_FOG_END, 300);
   glFogf(GL_FOG_DENSITY, 0.15);
}

int main(int argc, char **argv) {
   groundTexture = imread("./images/ground.jpg", CV_LOAD_IMAGE_COLOR);
   if (!groundTexture.data) {
      std::cout<<"could not load ground texture\n";
      return -1;
   }
   /** Passo 1: Inicializa funções GLUT */
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowPosition(100, 100);
   glutInitWindowSize(width, height);
   glutCreateWindow("Spider 3D");
   glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

   textures = new GLuint[3];
   glGenTextures(3, textures);
   lighting();
   setFog();
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LESS);
   glEnable(GL_CULL_FACE);
   glCullFace(GL_BACK);

   s = new Spider(textures);
   /** Passo 2: Registra callbacks da OpenGl */
   glutDisplayFunc(displayCallback);
   glutReshapeFunc(reshapeCallback);
   glutKeyboardFunc(keyboard_callback);
   glutSpecialFunc(special_callback);
   glutTimerFunc(1000/FRAMES, update, 0);

   /** Passo 3: Executa o programa */
   glutMainLoop();
   return 0;
}
