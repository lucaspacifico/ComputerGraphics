#include <GL/glut.h>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include "graphicPrimitives.h"

#define NUM_COL_P 4
#define NUM_COL_L 2

#define PI 3.14159265

float GTOR = PI/180;

typedef struct  {
	float **points; //Coordenadas x, y, z de cada ponto
	int **lines; //Indice do primeiro e ultimo ponto da linha
	int* trans; // Coordenadas x, y e z para projecao no sist de referencia do universo
	int* ang; //Angulos de rotacao nas coordenadas x, y e z
	float* scale; //Escala do objeto, x, y e z
	int p, l; //qnt de pontos e linhas
}graphObject;

graphObject* newObject(int n_points, int m_lines){
  graphObject* obj = (graphObject *)malloc(sizeof(graphObject));
	obj->p = n_points;
	obj->l = m_lines;

	// Tres arrays de informações
	obj->scale = (float*) malloc (sizeof(float)*3);
	obj->ang = (int *)malloc (sizeof(int)*3);
	obj->trans =  (int *) malloc (sizeof(int)*3);

	for (size_t i = 0; i < 3; i++) {
		obj->scale[i] = 0;
		obj->ang[i] = 0;
		obj->trans[i] = 0;
	}

	// Tabela de pontos
	obj->points = (float**) malloc(sizeof(float*)*n_points);
	for (size_t i = 0; i < n_points; i++) {
			obj->points[i] = (float*) malloc(sizeof(float)*NUM_COL_P);
	}
	//Tabela de linhas
	obj->lines = (int**) malloc(sizeof(int*)*m_lines);
	for (size_t i = 0; i < m_lines; i++) {
		 obj->lines[i] = (int*) malloc(sizeof(int)*NUM_COL_L);
	}

	return obj;
}

void destroyObject(graphObject* obj){
	for (size_t i = 0; i < obj->p; i++) {
		free(obj->points[i]);
	}
	for (size_t i = 0; i < obj->l; i++) {
		free(obj->lines[i]);
	}
	free(obj->points);
	free(obj->lines);
	free(obj->scale);
	free(obj->ang);
	free(obj->trans);
	free(obj);
}

graphObject* newCube (){
	graphObject* cube = newObject(8,12);

	cube->points[0][0] = -1.0f; cube->points[0][1] =  1.0f; cube->points[0][2] =  1.0f; cube->points[0][3] = 1.0f;//p3
	cube->points[1][0] = -1.0f; cube->points[1][1] =  1.0f; cube->points[1][2] = -1.0f; cube->points[1][3] = 1.0f;//p6
	cube->points[2][0] = -1.0f; cube->points[2][1] = -1.0f; cube->points[2][2] =  1.0f; cube->points[2][3] = 1.0f;//p2
	cube->points[3][0] = -1.0f; cube->points[3][1] = -1.0f; cube->points[3][2] = -1.0f; cube->points[3][3] = 1.0f;//p7

	cube->points[4][0] =  1.0f; cube->points[4][1] =  1.0f; cube->points[4][2] =  1.0f; cube->points[4][3] = 1.0f;//p4
	cube->points[5][0] =  1.0f; cube->points[5][1] =  1.0f; cube->points[5][2] = -1.0f; cube->points[5][3] = 1.0f;//p5
	cube->points[6][0] =  1.0f; cube->points[6][1] = -1.0f; cube->points[6][2] =  1.0f; cube->points[6][3] = 1.0f;//p1
	cube->points[7][0] =  1.0f; cube->points[7][1] = -1.0f; cube->points[7][2] = -1.0f; cube->points[7][3] = 1.0f;//p8

	//face frente
	cube->lines [0][0] = 6; cube->lines [0][1] = 2;
	cube->lines [1][0] = 2; cube->lines [1][1] = 0;
	cube->lines [2][0] = 0; cube->lines [2][1] = 4;
	cube->lines [3][0] = 4; cube->lines [3][1] = 6;
	//face tras
	cube->lines [4][0] = 5; cube->lines [4][1] = 1;
	cube->lines [5][0] = 1; cube->lines [5][1] = 3;
	cube->lines [6][0] = 3; cube->lines [6][1] = 7;
	cube->lines [7][0] = 7; cube->lines [7][1] = 5;
	//interligações
	cube->lines [8][0] = 4; cube->lines [8][1] = 5;
	cube->lines [9][0] = 0; cube->lines [9][1] = 1;
	cube->lines[10][0] = 2; cube->lines[10][1] = 3;
	cube->lines[11][0] = 6; cube->lines[11][1] = 7;

	return cube;
}

graphObject* newPyramid (){
	graphObject* pyramid = newObject(5,8);

	pyramid->points[0][0] =  1.0f; pyramid->points[0][1] = -1.0f; pyramid->points[0][2] =  1.0f; pyramid->points[0][3] = 1.0f;//p1
	pyramid->points[1][0] = -1.0f; pyramid->points[1][1] = -1.0f; pyramid->points[1][2] =  1.0f; pyramid->points[1][3] = 1.0f;//p2
	pyramid->points[2][0] = -1.0f; pyramid->points[2][1] = -1.0f; pyramid->points[2][2] = -1.0f; pyramid->points[2][3] = 1.0f;//p3
	pyramid->points[3][0] =  1.0f; pyramid->points[3][1] = -1.0f; pyramid->points[3][2] = -1.0f; pyramid->points[3][3] = 1.0f;//p4
	pyramid->points[4][0] =  0.0f; pyramid->points[4][1] =  1.0f; pyramid->points[4][2] =  0.0f; pyramid->points[4][3] = 1.0f;//p5

	// quadrado inferior
	pyramid->lines [0][0] = 0; pyramid->lines [0][1] = 1;
	pyramid->lines [1][0] = 1; pyramid->lines [1][1] = 2;
	pyramid->lines [2][0] = 2; pyramid->lines [2][1] = 3;
	pyramid->lines [3][0] = 3; pyramid->lines [3][1] = 0;
	// triangulos
	pyramid->lines [4][0] = 0; pyramid->lines [4][1] = 4;
	pyramid->lines [5][0] = 1; pyramid->lines [5][1] = 4;
	pyramid->lines [6][0] = 2; pyramid->lines [6][1] = 4;
	pyramid->lines [7][0] = 3; pyramid->lines [7][1] = 4;

	return pyramid;
}

graphObject* newDiamond (){
	graphObject* diamond = newObject(8,12);

	diamond->points[0][0] =  0.0f; diamond->points[0][1] =  1.0f; diamond->points[0][2] =  1.0f; diamond->points[0][3] = 1.0f;//p1
	diamond->points[1][0] = -1.0f; diamond->points[1][1] =  0.0f; diamond->points[1][2] =  1.0f; diamond->points[1][3] = 1.0f;//p2
	diamond->points[2][0] =  0.0f; diamond->points[2][1] = -1.0f; diamond->points[2][2] =  1.0f; diamond->points[2][3] = 1.0f;//p3
	diamond->points[3][0] =  1.0f; diamond->points[3][1] =  0.0f; diamond->points[3][2] =  1.0f; diamond->points[3][3] = 1.0f;//p4

	diamond->points[4][0] =  0.0f; diamond->points[4][1] =  1.0f; diamond->points[4][2] = -1.0f; diamond->points[4][3] = 1.0f;//p5
	diamond->points[5][0] = -1.0f; diamond->points[5][1] =  0.0f; diamond->points[5][2] = -1.0f; diamond->points[5][3] = 1.0f;//p6
	diamond->points[6][0] =  0.0f; diamond->points[6][1] = -1.0f; diamond->points[6][2] = -1.0f; diamond->points[6][3] = 1.0f;//p7
	diamond->points[7][0] =  1.0f; diamond->points[7][1] =  0.0f; diamond->points[7][2] = -1.0f; diamond->points[7][3] = 1.0f;//p8

	//face frente
	diamond->lines [0][0] = 0; diamond->lines [0][1] = 1;
	diamond->lines [1][0] = 1; diamond->lines [1][1] = 2;
	diamond->lines [2][0] = 2; diamond->lines [2][1] = 3;
	diamond->lines [3][0] = 3; diamond->lines [3][1] = 0;
	//face tras
	diamond->lines [4][0] = 4; diamond->lines [4][1] = 5;
	diamond->lines [5][0] = 5; diamond->lines [5][1] = 6;
	diamond->lines [6][0] = 6; diamond->lines [6][1] = 7;
	diamond->lines [7][0] = 7; diamond->lines [7][1] = 4;
	//interligações
	diamond->lines [8][0] = 0; diamond->lines [8][1] = 4;
	diamond->lines [9][0] = 1; diamond->lines [9][1] = 5;
	diamond->lines[10][0] = 2; diamond->lines[10][1] = 6;
	diamond->lines[11][0] = 3; diamond->lines[11][1] = 7;

	return diamond;
}


graphObject* copyObject (graphObject* orig){
	graphObject* copy = newObject(orig->p,orig->l);
	for (size_t j = 0; j < copy->p; j++) {
		for (size_t i = 0; i < NUM_COL_P; i++) {
			copy->points[j][i] = orig->points[j][i];
		}
	}
	for (size_t j = 0; j < copy->l; j++) {
		for (size_t i = 0; i < NUM_COL_L; i++) {
			copy->lines[j][i] = orig->lines[j][i];
		}
	}
	return copy;
}

void matrixMult (graphObject* obj, float mtr[][4]){
	float x,y,z,m;
	for (size_t i = 0; i < obj->p; i++) {
		x = obj->points[i][0];
		y = obj->points[i][1];
		z = obj->points[i][2];
		m = obj->points[i][3];
		obj->points[i][0] =  x * mtr[0][0] + y * mtr[1][0] + z * mtr[2][0] + m * mtr[3][0];
		obj->points[i][1] =  x * mtr[0][1] + y * mtr[1][1] + z * mtr[2][1] + m * mtr[3][1];
		obj->points[i][2] =  x * mtr[0][2] + y * mtr[1][2] + z * mtr[2][2] + m * mtr[3][2];
		obj->points[i][3] =  x * mtr[0][3] + y * mtr[1][3] + z * mtr[2][3] + m * mtr[3][3];
	}
}

void rotateX(graphObject* obj, float angle){
	obj->ang[0] += angle;

	float rotateMtr[4][4] = {1, 0, 						 			0,								0,
												 	 0, (float)cos(angle*GTOR), 	 	(float)sin(angle*GTOR),	0,
												 	 0,	(float)sin(angle*GTOR)*-1, (float)cos(angle*GTOR),	0,
													 0,	0,									0,								1};
 	matrixMult(obj,rotateMtr);
}

void rotateY(graphObject* obj, float angle){
	obj->ang[1] += angle;
	float rotateMtr[4][4] = {(float)cos(angle*GTOR), 0, 	(float)sin(angle*GTOR)*-1,	0,
												 	 0, 							1,	0,									0,
												 	 (float)sin(angle*GTOR),	0,	(float)cos(angle*GTOR),		0,
													 0,								0,	0,									1};
 	matrixMult(obj,rotateMtr);
}

void rotateZ(graphObject* obj, float angle){
	obj->ang[2] += angle;
	float rotateMtr[4][4] = {(float)cos(angle*GTOR), 		(float)sin(angle*GTOR), 	0,	0,
									 			 	 (float)sin(angle*GTOR)*-1, 	(float)cos(angle*GTOR), 	0,	0,
									 		 	 	 0,										0,								1,	0,
													 0,										0,								0,	1};
	matrixMult(obj,rotateMtr);
}

void scaleX(graphObject* obj, float scale){
	obj->scale[0] += scale;
	float scaleMtr[4][4] = { scale, 0, 0, 0,
												 	 0, 		1, 0, 0,
												 	 0,			0, 1, 0,
													 0,			0, 0, 1};
 	matrixMult(obj,scaleMtr);
}

void scaleY(graphObject* obj, float scale){
	obj->scale[1] += scale;
	float scaleMtr[4][4] = { 1, 0, 		0, 0,
												 	 0,	scale,0, 0,
												 	 0,	0,  	1, 0,
													 0,	0, 		0, 1};
 	matrixMult(obj,scaleMtr);
}

void scaleZ(graphObject* obj, float scale){
	obj->scale[2] += scale;
	float scaleMtr[4][4] = { 1, 0, 0,			0,
												 	 0,	1, 0,			0,
												 	 0,	0, scale,	0,
													 0,	0, 0,			1};
 	matrixMult(obj,scaleMtr);
}

void transX(graphObject* obj, int trans){
	obj->trans[0] += trans;
	float transMtr[4][4] = {1, 			0, 	0,	0,
													0,			1, 	0,	0,
													0, 			0,	1,	0,
													(float)trans,	0, 	0,	1};
 	matrixMult(obj,transMtr);
}

void transY(graphObject* obj, int trans){
	obj->trans[1] += trans;
	float transMtr[4][4] = {1, 	0, 			0,	0,
													0,	1, 			0,	0,
													0, 	0,			1,	0,
													0,	(float)trans, 	0,	1};
 	matrixMult(obj,transMtr);
}

void scaleObject(graphObject* obj, float scale){
	scaleX(obj,scale);
	scaleY(obj,scale);
	scaleZ(obj,scale);
}

void moveCenter(graphObject* obj, int center){
	transX(obj,center);
	transY(obj,center);
}


void cabinet_proj (graphObject* obj){
	float alfa = 0.707/2;
	float cabMtr[4][4] = {1, 		0, 		0,	0,
										 		0,		1, 		0,	0,
									 	 		alfa, alfa,	0,	0,
												0,		0, 		0,	1};
	matrixMult(obj,cabMtr);
}

void showLinesCube (graphObject* obj, int idColor){
	float x1, y1, x2, y2;
	//int idColor = 5;
	for (size_t j = 0; j < obj->l; j++) {
		x1 = (obj->points[ obj->lines[j][0] ][0])/obj->points[ obj->lines[j][0] ][3];
		y1 = (obj->points[ obj->lines[j][0] ][1])/obj->points[ obj->lines[j][0] ][3];
		x2 = (obj->points[ obj->lines[j][1] ][0])/obj->points[ obj->lines[j][1] ][3];
		y2 = (obj->points[ obj->lines[j][1] ][1])/obj->points[ obj->lines[j][1] ][3];
		createLineMtr(x1,y1,x2,y2,idColor);
	}
}
