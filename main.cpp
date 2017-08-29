#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <cstdio>
#include "object3D.h"
#include "graphicPrimitives.h"


#include"/usr/include/opencv2/highgui/highgui.hpp"
#include "/usr/include/opencv2/core/core.hpp"
#include <cv.h>


#define ANGLE_SPEED 2
#define SCALE_SPEED 1.1
#define Pyramid_SPEED 5
#define GLUT_KEY_TAB 9
#define GLUT_KEY_SHIFT 15

using namespace cv;
using namespace std;


CascadeClassifier face_cascade, eyes_cascade;
String window_name = "Face Detection";

// Direita x diminui, esquerda aumenta
typedef struct FaceLocation{
  int x;
  int y;
  int width;
  int height;
}facelocation;

/**
 * Detects faces and draws an ellipse around them
 */
 int flag = 0;
 facelocation fps;


struct listaObject{
  graphObject* data;
  struct listaObject *prox;
};

typedef struct listaObject listao;

listao* cria(graphObject* orig){
  listao* retorno = (listao*) malloc(sizeof(listao));
  retorno->data = orig;
  retorno->prox = retorno;

  return retorno;
}

void insere(listao* lista, graphObject* orig){
  if(lista == NULL) return;

  listao* loop;

  for(loop = lista; loop->prox != lista; loop = loop->prox);

  loop = loop->prox = (listao*) malloc(sizeof(listao));

  loop->data = orig;
  loop->prox = lista;
  loop = NULL;
}

void destroi(listao* lista){

  if(lista == NULL) return;

  listao *loop = lista->prox, *deleta;

  while(loop != lista){ //deleta os elementos da lista até sobrar só o primeiro

      deleta = loop;

      loop = loop->prox;

      free(deleta);

      deleta = NULL;
  }

  free(lista); //deleta o primeiro

  lista = loop = NULL;
}

int funcProj;
int win, main_menu, proj_menu;
int idColor = 8, idColorSel = 1;
listao* listaObjetos;
listao* listaCube;
listao* listaPyramid;
listao* listaDiamond;
listao* listaAux;
graphObject* orig;
graphObject* orig2;
graphObject* orig3;
graphObject* copy1;
graphObject* copy2;
graphObject* copy3;

void setup(){
   glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
   gluOrtho2D(0, 599, 0, 599);

   orig = newCube();
   orig2 = newPyramid();
   orig3 = newDiamond();

   listaObjetos = cria(orig);
   insere(listaObjetos, orig2);
   insere(listaObjetos, orig3);

   listaCube = listaObjetos;
   listaPyramid = listaObjetos->prox;
   listaDiamond = listaPyramid->prox;
   listaAux = listaObjetos;

   scaleObject(listaCube->data,50.0f);
   scaleObject(listaPyramid->data,50.0f);
   scaleObject(listaDiamond->data,50.0f);
}

void proj (){
  copy1 = copyObject(listaCube->data);
  copy2 = copyObject(listaPyramid->data);
  copy3 = copyObject(listaDiamond->data);

      cabinet_proj(copy1);
      cabinet_proj(copy2);
      cabinet_proj(copy3);

  moveCenter(copy1,300);
  moveCenter(copy2,500);
  moveCenter(copy3,100);

  showLinesCube(copy1, idColor);
  showLinesCube(copy2, idColor);
  showLinesCube(copy3, idColor);

  destroyObject(copy1);
  destroyObject(copy2);
  destroyObject(copy3);
}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(0.0f, 0.0f, 0.0f);

  clearMtrDisplay();
  proj();
  printMtrDisplay();
  glutSwapBuffers();
}

void transformations (unsigned char key,int x, int y){
  switch (key) {
    case GLUT_KEY_TAB:
      listaAux = listaAux->prox;
      idColor = idColorSel + idColor;
      idColorSel = idColor - idColorSel;
      idColor -= idColorSel;

      break;
    // Rotacionar
    case 'x':
      rotateY(listaCube->data,ANGLE_SPEED);
      rotateY(listaPyramid->data,ANGLE_SPEED);
      rotateY(listaDiamond->data,ANGLE_SPEED);
      glutIdleFunc(display);

      break;
    case 's':
      rotateY(listaCube->data,-ANGLE_SPEED);
      rotateY(listaPyramid->data,-ANGLE_SPEED);
      rotateY(listaDiamond->data,-ANGLE_SPEED);
      glutIdleFunc(display);

      break;
  }
     glFlush();
  glutPostRedisplay();
}



void detectFaces(Mat frame) {

  std::vector<Rect> faces;
  Mat frame_gray;
  cvtColor(frame, frame_gray, COLOR_BGR2GRAY);	// Convert to gray scale
  equalizeHist(frame_gray, frame_gray);     	// Equalize histogram

  // Detect faces
  face_cascade.detectMultiScale(frame_gray, faces, 1.1, 3,
				0|CASCADE_SCALE_IMAGE, Size(30, 30));

    if(flag == 0 ){
    fps.x = faces[0].x;
    fps.y = faces[0].y;
    fps.width = faces[0].width;
    fps.height = faces[0].height;
    flag = 1;
    }

  // cout << fps.x << fps.y << endl;
  // Iterate over all of the faces
  for(size_t i = 0; i < faces.size(); i++) {

    // Find center of faces
    Point center(faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2);

    if(faces[i].x < (fps.x-4)){
    // cout << "left" << endl;
    transformations('x',0,0);
  }else if(faces[i].x > (fps.x+4)){
    // cout << "right" << endl;
    transformations('s',0,0);

  }

  fps.x = faces[i].x;
  fps.y = faces[i].y;
  fps.width = faces[i].width;
  fps.height = faces[i].height;
    // cout << "X >: "<< faces[i].x << "Y >: " << faces[i].y << "Type >: " << sizeof(faces[i].x) << "I >: "<< i <<endl;
    Mat face = frame_gray(faces[i]);
    std::vector<Rect> eyes;

    // Try to detect eyes, inside each face
    eyes_cascade.detectMultiScale(face, eyes, 1.1, 2,
				  0 |CASCADE_SCALE_IMAGE, Size(30, 30) );

    // Check to see if eyes inside of face, if so, draw ellipse around face

     if(eyes.size() > 0)
       ellipse(frame, center, Size(faces[i].width/2, faces[i].height/2),
	      0, 0, 360, Scalar( 15, 0,255  ), 4, 8, 0 );
    }

  imshow( window_name, frame );
}



void *sub_main_disp(void *){
   glutInitDisplayMode(GLUT_DOUBLE);
   glutInitWindowSize(800,600);

   win = glutCreateWindow("Objetos3D");

   glutKeyboardFunc(transformations);

   setup();
   glutDisplayFunc(display);
   glutMainLoop();
}

int main (int argc, char *argv[]){

  int ver1;

  pthread_t t1;

  glutInit(&argc, argv);
  pthread_create(&t1, NULL, &sub_main_disp, NULL);

  VideoCapture cap(0);        // Open default camera
  Mat frame;
  // Load preconstructed classifier
  face_cascade.load("haarcascade_frontalface_alt.xml");
  eyes_cascade.load("haarcascade_eye.xml");

  while(cap.read(frame)) {
   detectFaces(frame);       // Call function to detect faces
   if( waitKey(30) >= 0)     // Pause key
     break;
  }
  return 0;
}
