#include <stdio.h>
#include <process.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define PI 3.14159265
#define ZERO 0.001

/* [0, 1.0)の実数乱数 */
#define rnd() ((double)rand()/32767.5)

/* BMPファイル用 */
typedef unsigned short WORD;
typedef unsigned long DWORD;

/* ビットマップデータ */
#define Y_SIZE 512
#define X_SIZE 512
unsigned char Pixel[Y_SIZE][X_SIZE][3];

/* テクスチャマッピング用画像データ */
unsigned char TexImage[Y_SIZE][X_SIZE][3];
DWORD WTex, HTex;

/* CSGデータ構造　*/
typedef struct {	/* プリミティブのデータ構造 */
    char *type;			/* プリミティブの種類 */
    double rgb[3];      /* 色(R,G,B:[0,1]) */
    double kd;			/* 拡散反射係数 */
    double ks;			/* 鏡面反射係数 */
    int n;				/* 鏡面度 */
    double data[20];	/* プリミティブの形状パラメータ
                           (半径，中心座標など) */
} Primitive;

typedef struct {	/* CSG トリー構造 */
    char op;	/* オペレータ(和：'+'，差：'-'，積：'*'，プリミティブ：'E') */
    int left;	/* 左側下位ノードへのポインタ（または，プリミティブ番号) */
    int right;	/* 右側下位ノードへのポインタ */
} CSG;

/* 背景色 */
double Dark[3] = {0.2, 0.2, 0.2};

/* 環境光の強さ */
double RgbEnv[3] = {0.1, 0.1, 0.1};

/* 光源データ構造 */
typedef struct {
	double x,y,z;	/* 位置 */
	double r,g,b;	/* 色[0,1] */
} LIGHT;

/* 光源データ */
int NLight;			/* 光源数 */
LIGHT Light[10];	/* 光源の位置と色 */

/* カメラ・スクリーンデータ */
double C[3]; 	/* 視点座標(カメラ位置) */
double B[3]; 	/* スクリーン中心点 */
double VUP[3]; 	/* アップベクトル */
int WS; 		/* 画像幅 */
int HS; 		/* 画像高さ */
double D;		/* 画像の拡大率 */
double W[3];	/* 視線単位ベクトル */
double U[3];	/* スクリーン座標u方向単位ベクトル */
double V[3];	/* スクリーン座標v方向単位ベクトル */

/* プリミティブの陰関数表現式のテーブル */
#define M1 200
#define M2 100
double A1[ 4][M1];	/* 平面の陰関数表現式の係数
                       A1[i][j]  i...係数番号  j...面番号
                       陰関数表現式
                         a0*x + a1*y + a2*z + a3 = 0
                    */
int A1Prim[M1];		/* プリミティブ番号 */
double A2[10][M2];	/* ２次曲面の陰関数表現式の係数
                       A2[i][j]  i...係数番号  j...面番号
                       陰関数表現式
                         a0*x^2 + a1*y^2 + a2*z^2 +
                         a3*x*y + a4*y*z + a5*z*x +
                         a6*x + a7*y + a8*z + a9 = 0
                    */
int A2Prim[M2];		/* プリミティブ番号 */
int NS1; 		    /* 平面の数 */
int NS2; 		    /* ２次曲面の数 */

/* 交点のデータ構造 */
typedef struct {
	double x, y, z; /* 座標値 */
	double t;		/* パラメータ値 */
	int type;		/* 面のタイプ =1...平面 =2...２次曲面 */
	int sf;			/* 面番号  type=1ならA1[][sf]
	                               =2ならA2[][sf] */
	int prim;		/* プリミティブ番号 */
} PointList;

int Sign[M2];		/* 法線ベクトルの符号
                        差演算をすると法線ベクトルの向きが逆転するので，
                        何回差演算が施されたかをカウントして，
                        偶数回なら +1 奇数回なら -1 とする． */
int Nnega[M2];		/* 各プリミティブの'-'の数を数えるための配列 */
