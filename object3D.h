typedef struct  {
	float **points; //Coordenadas x, y, z de cada ponto
	int **lines; //Indice do primeiro e ultimo ponto da linha
	int* trans; // Coordenadas x, y e z para projecao no sist de referencia do universo
	int* ang; //Angulos de rotacao nas coordenadas x, y e z
	float* scale; //Escala do objeto, x, y e z
	int p, l; //qnt de pontos e linhas
}graphObject;

graphObject* newObject(int n_points, int m_lines);

void destroyObject(graphObject* obj);

void printMtr (int linha, int coluna, int** mtr);

void printMtrFloat (int linha, int coluna, float** mtr);

graphObject* newCube ();

graphObject* newPyramid ();

graphObject* newDiamond ();

void printObjectInfo (graphObject* obj);

graphObject* copyObject (graphObject* orig);

void matrixMult (graphObject* obj, float mtr[][3]);

void matrixSum (graphObject* obj, int mtr[3]);

void rotateX(graphObject* obj, float angle);

void rotateY(graphObject* obj, float angle);

void rotateZ(graphObject* obj, float angle);

void scaleX(graphObject* obj, float scale);

void scaleY(graphObject* obj, float scale);

void scaleZ(graphObject* obj, float scale);

void transX(graphObject* obj, int trans);

void transY(graphObject* obj, int trans);

graphObject* scaleObject(graphObject* obj, float scale);

graphObject* moveCenter(graphObject* obj, int center);

void cabinet_proj (graphObject* obj);

void showLinesCube (graphObject* cube, int idColor);
