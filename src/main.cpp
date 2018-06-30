//
// Created by Bruno on 15/05/2018.
//
#include <iostream>
#include <GL/glut.h>
#include <spider.h>
#include <types.h>

#define FRAMES 24

int width = 800;
int height = 600;
Spider *s;
bool fog = false;


/**
 * @desc Desenha eixos de um sistema de coordenadas.
 * @param {float*} basePoint Ponto de origem de um sistema de coordenadas.
 * @param {float*} i Primeiro versor.
 * @param {float*} j Segundo versor.
 * @param {float*} k Terceiro versor.
 */
void drawAxes(float *basePoint, float *i, float *j, float *k)
{
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

/**
 * @desc Desenha um plano em formato de grade - adaptado do programa de matrix de projeção, de Song Ho Ahn (http://www.songho.ca/opengl/gl_transform.html).
 * @param {float} size Define o tamanho da grade.
 * @param {float} step Especifica a quantidade de intervalos de grades, causando a impressão de pisos no chão.
 */
void drawGrid(float size, float step)
{
    glPushMatrix();
    glTranslated(-size/2,  0,  -size / 2);

    float i;
    glColor3f(0.3, 0.3, 0.3);
    for(i = 0; i < size; i = i + step)
    {
        /** Translada para o ponto no eixo x para desenhar a linha paralela ao eixo z */
        glTranslatef(i, 0.0, 0.0);
        glBegin(GL_LINES);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(0.0, 0.0, size);
        glEnd();
        glTranslatef(-i, 0.0, 0.0);
        /** Translada para o ponto no eixo z para desenhar a linha paralela ao eixo x */
        glTranslatef(0.0, 0.0, i);
        glBegin(GL_LINES);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(size, 0.0, 0.0);
        glEnd();
        glTranslatef(0.0, 0.0, -i);
    }
    glPopMatrix();
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

    glColor3f(1.0f, 0.0f, 0.0f);

    double x,y,z;
    x = s->getPosition()->x;
    y = s->getPosition()->y;
    z = s->getPosition()->z;

    /** Desenha a janela mais a esquerda */
    glViewport(0, 0, width/2, height/2);
    glLoadIdentity();
    gluLookAt(3.0 + x, 2.0 + y, 10.0 + z, x, y, z, 0.0, 1.0, 0.0);
    drawGrid(300, 0.7);
    drawWCAxes();
    s->draw();

    glViewport(width/2, 0, width/2, height/2);
    glLoadIdentity();
    gluLookAt(0.0 + x, 10.0 + y, 0.0 + z, x, y, z, 1.0, 0.0, 0.0);
    drawGrid(300, 0.7);
    drawWCAxes();
    s->draw();

    glViewport(0, height/2, width/2, height/2);
    glLoadIdentity();
    gluLookAt(10.0 + x, 0.0 + y, 0.0 + z, x, y, z, 0.0, 1.0, 0.0);
    drawGrid(300, 0.7);
    drawWCAxes();
    s->draw();

    glViewport(width/2, height/2, width/2, height/2);
    glLoadIdentity();
    gluLookAt(0.0 + x, 0.0 + y, 10.0 + z, x, y, z, 0.0, 1.0, 0.0);
    drawGrid(300, 0.7);
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
    gluPerspective(65.0, (GLfloat) width/(GLfloat) height, 1.0, 20.0);
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
    GLfloat position_type[] = {1, 1, 1, 0};
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

int main(int argc, char **argv)
{
    /** Passo 1: Inicializa funções GLUT */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(width, height);
    glutCreateWindow("Spider 3D");
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    lighting();
    setFog();
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    s = new Spider();

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
