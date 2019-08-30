/*
Visualizador 3d de moleculas en formato .xyz
*/
#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace::std;

typedef struct // Almacena los datos del archivo .xyz
{
  float codxyz[200][3]; // Almacena las coodernadas xyz
  string at_type[200]; // Almacena el tipo de atomo
  int num_at; // Almacena la cantidad de atomos del archivo .xyz
} fileXYZ;

fileXYZ read_filexyz(string namexyz) // Lee el archivo .xyz
{
  ifstream archivo;
  fileXYZ A; 
  string read_xyz, cod, temp, M[200];
  int i = 0, j = 0;

  archivo.open(namexyz);
  if(archivo.fail())
  {
    cout << "Error al abrir el archivo" << endl;
    return A;
  }
  else
  {
    while (!archivo.eof())
    {
      getline(archivo, read_xyz);
      if (read_xyz != "\0")
      {     
        if (i == 0)
        { 
          A.num_at = stoi(read_xyz); // Lee el numero total de atomos
          A.codxyz[A.num_at][3];
          A.at_type[A.num_at];
        }
        else  
          if (i != 1 && read_xyz.length() != 2)
          {
            M[j] = read_xyz;
            A.at_type[j] = read_xyz[0]; // Lee el tipo de atomos por linea
            cod = M[j].substr(10, read_xyz.length());
            istringstream iss(cod);
            iss >> temp;
            A.codxyz[j][0] = stof(temp); // Almacena la coordenada x
            iss >> temp;
            A.codxyz[j][1] = stof(temp); // Almacena la coordenada y
            iss >> temp;
            A.codxyz[j][2] = stof(temp); // Almacena la coordenada z
            j++;
          }
        i++;
      }
    }
    return A;
  }
}

fileXYZ A;
void name_file() // Abre el archivo
{
  string name;
  cout << "Ingrese el nombre del archivo" << endl;
  cin >> name;
  name = name + ".xyz";
  A = read_filexyz(name);
}

/* Datos de elementos 
elemt -> Tipo de elemento
radiuselemt -> Radio de VanderWaals
colelemt -> Colores por tipo de elemento
*/
float radius = 0.0;
string elemt[6] = {"H", "C", "N", "O", "P", "S"};
float radiuselemt[6] = {0.120, 0.170, 0.155, 0.152, 0.180, 0.180}; 
float colelemt[6][3] = {{1.0, 1.0, 1.0},{0.3, 0.3, 0.3},{0.0, 0.2, 0.7},{1.0, 0.0, 0.0},{0.88, 0.46, 0.02},{0.61, 0.61, 0.02}};

void display() 
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClear(GL_COLOR_BUFFER_BIT);

  if (A.at_type[0] != "\0")
    for(int i = 0; i < A.num_at; ++i) // Dibuja las esferas 
    {
      glPushMatrix();
        glTranslatef(A.codxyz[i][0], A.codxyz[i][1], A.codxyz[i][2]); // Posicion de cada esfera
        for (int j = 0; j < 6; ++j)        
          if (A.at_type[i] == elemt[j])
          {
            radius = radiuselemt[j]; // Asigna el radio
            glColor3f(colelemt[j][0], colelemt[j][1], colelemt[j][2]); // Asigna el color          
          }
        glutSolidSphere(radius * 8, 30, 30);
      glPopMatrix();
    }
  glutSwapBuffers(); 
}

void init() {

  //glClearColor(0.1, 0.39, 0.88, 1.0);

	float mat_shininess = 50;
	float light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	float light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	float light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	float light_position[] = { 1.0, 1.0, 1.0, 0.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glMaterialfv(GL_FRONT, GL_SPECULAR, light_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, &mat_shininess);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
}


#define scale 10.0
void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
		glOrtho(-scale,scale,-scale*h/w,scale*h/w,-10.0,10.0);
	else
		glOrtho(-scale*w/h,scale*w/h,-scale,scale,-10.0,10.0);
	glMatrixMode(GL_MODELVIEW);
	glutPostRedisplay();
}
float *GLMnew, *GLMold;
float GLMatAux[32] = {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
void MultMatrixR(float *M)
{
  GLMnew[0] = M[0]*GLMold[0] + M[1]*GLMold[1] + M[2]*GLMold[2];
  GLMnew[1] = M[3]*GLMold[0] + M[4]*GLMold[1] + M[5]*GLMold[2];
  GLMnew[2] = M[6]*GLMold[0] + M[7]*GLMold[1] + M[8]*GLMold[2];
  GLMnew[4] = M[0]*GLMold[4] + M[1]*GLMold[5] + M[2]*GLMold[6];
  GLMnew[5] = M[3]*GLMold[4] + M[4]*GLMold[5] + M[5]*GLMold[6];
  GLMnew[6] = M[6]*GLMold[4] + M[7]*GLMold[5] + M[8]*GLMold[6];
  GLMnew[8] = M[0]*GLMold[8] + M[1]*GLMold[9] + M[2]*GLMold[10];
  GLMnew[9] = M[3]*GLMold[8] + M[4]*GLMold[9] + M[5]*GLMold[10];
  GLMnew[10] = M[6]*GLMold[8] + M[7]*GLMold[9] + M[8]*GLMold[10];
  glLoadMatrixf(GLMnew);
  {float *t = GLMold; GLMold = GLMnew; GLMnew = t;}
  glutPostRedisplay();
}

#define deltaang 5.0f
float mxp[9] = {1, 0, 0, 0, cosf(deltaang*(M_PI/180)), sinf(deltaang*(M_PI/180)), 0, -sinf(deltaang*(M_PI/180)), cosf(deltaang*(M_PI/180))};
float mxn[9] = {1, 0, 0, 0, cosf(deltaang*(M_PI/180)), -sinf(deltaang*(M_PI/180)), 0, sinf(deltaang*(M_PI/180)), cosf(deltaang*(M_PI/180))};
float myp[9] = {cosf(deltaang*(M_PI/180)), 0, -sinf(deltaang*(M_PI/180)), 0, 1, 0, sinf(deltaang*(M_PI/180)), 0, cosf(deltaang*(M_PI/180))};
float myn[9] = {cosf(deltaang*(M_PI/180)), 0, sinf(deltaang*(M_PI/180)), 0, 1, 0, -sinf(deltaang*(M_PI/180)), 0, cosf(deltaang*(M_PI/180))};

void ControlTeclado(int key, int x, int y)
{
  switch(key)
  {
    case GLUT_KEY_UP:
    	MultMatrixR(mxp);
      break;

    case GLUT_KEY_DOWN:
    	MultMatrixR(mxn);
      break;

    case GLUT_KEY_LEFT:
    	MultMatrixR(myp);
      break;

    case GLUT_KEY_RIGHT:
    	MultMatrixR(myn);
      break;
  }
}

int main(int argc, char** argv)
 {
  name_file();
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowPosition(80, 80);
  glutInitWindowSize(800, 600);
  glutCreateWindow("Molecule Display");
  glutDisplayFunc(display);
  glutSpecialFunc(ControlTeclado);
  glutReshapeFunc(resize);
  init();
  GLMold = GLMatAux;
  GLMnew = GLMatAux + 16;
  glutMainLoop();
}
