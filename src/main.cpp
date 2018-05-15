//
// Created by Bruno on 15/05/2018.
//
#include <iostream>
#include <GL/glut.h>

int width = 800;
int height = 600;

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
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0f, 0.0f, 0.0f);

    /** Desenha a janela mais a esquerda */
    glViewport(0, 0, width/2, height/2);
    glLoadIdentity();
    gluLookAt(3.0, 2.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    drawGrid(30, 0.7);
    drawWCAxes();
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    glutWireCone(1.0, 2.0, 4, 4);
    //Desenhar aranha

    /** Desenha a janela mais a direita */
    glViewport(width/2, 0, width/2, height/2);
    glLoadIdentity();
    gluLookAt(0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0);
    drawGrid(30, 0.7);
    drawWCAxes();
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    glutWireCone(1.0, 2.0, 4, 4);
    //Desenhar Aranha

    glViewport(0, height/2, width/2, height/2);
    glLoadIdentity();
    gluLookAt(10.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    drawGrid(30, 0.7);
    drawWCAxes();
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    glutWireCone(1.0, 2.0, 4, 4);
    //Desenhar Aranha

    glViewport(width/2, height/2, width/2, height/2);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    drawGrid(30, 0.7);
    drawWCAxes();
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    glutWireCone(1.0, 2.0, 4, 4);
    //Desenhar Aranha

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

int main(int argc, char **argv)
{
    /** Passo 1: Inicializa funções GLUT */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(width, height);
    glutCreateWindow("Spider 3D");
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    /** Passo 2: Registra callbacks da OpenGl */
    glutDisplayFunc(displayCallback);
    glutReshapeFunc(reshapeCallback);

    /** Passo 3: Executa o programa */
    glutMainLoop();
    return 0;
}


