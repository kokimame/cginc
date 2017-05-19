#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifdef WIN32
#include <process.h>
#endif

#define PI 3.14159265
#define ZERO 0.001

#define rnd() ((double)rand()/32767.5)

typedef unsigned short WORD;
typedef unsigned long DWORD;

#define Y_SIZE 512
#define X_SIZE 512
unsigned char Pixel[Y_SIZE][X_SIZE][3];

unsigned char TexImage[Y_SIZE][X_SIZE][3];
DWORD WTex, HTex;

typedef struct {
    char *type;
    double rgb[3];
    double kd;
    double ks;
    int n;
    double data[20];
} Primitive;

typedef struct {
    char op;
    int left;
    int right;
} CSG;

int NPri = 0;
int NTree = 0;
Primitive PDB[100];
CSG ST[100];

double Dark[3] = {0.2, 0.2, 0.2};

double RgbEnv[3] = {0.1, 0.1, 0.1};

typedef struct {
    double x,y,z;
    double r,g,b;
} LIGHT;

int NLight;
LIGHT Light[10];

double C[3];
double B[3];
double VUP[3];
int WS;
int HS;
double D;
double W[3];
double U[3];
double V[3];

#define M1 200
#define M2 100
double A1[ 4][M1];
int A1Prim[M1];
double A2[10][M2];
int A2Prim[M2];
int NS1;
int NS2;

typedef struct {
    double x, y, z;
    double t;
    int type;
    int sf;
    int prim;
} PointList;

int Sign[M2];
int Nnega[M2];
