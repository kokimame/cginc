#include <stdio.h>
#include <process.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define PI 3.14159265
#define ZERO 0.001

/* [0, 1.0)�̎������� */
#define rnd() ((double)rand()/32767.5)

/* BMP�t�@�C���p */
typedef unsigned short WORD;
typedef unsigned long DWORD;

/* �r�b�g�}�b�v�f�[�^ */
#define Y_SIZE 512
#define X_SIZE 512
unsigned char Pixel[Y_SIZE][X_SIZE][3];

/* �e�N�X�`���}�b�s���O�p�摜�f�[�^ */
unsigned char TexImage[Y_SIZE][X_SIZE][3];
DWORD WTex, HTex;

/* CSG�f�[�^�\���@*/
typedef struct {	/* �v���~�e�B�u�̃f�[�^�\�� */
    char *type;			/* �v���~�e�B�u�̎�� */
    double rgb[3];      /* �F(R,G,B:[0,1]) */
    double kd;			/* �g�U���ˌW�� */
    double ks;			/* ���ʔ��ˌW�� */
    int n;				/* ���ʓx */
    double data[20];	/* �v���~�e�B�u�̌`��p�����[�^
                           (���a�C���S���W�Ȃ�) */
} Primitive;

typedef struct {	/* CSG �g���[�\�� */
    char op;	/* �I�y���[�^(�a�F'+'�C���F'-'�C�ρF'*'�C�v���~�e�B�u�F'E') */
    int left;	/* �������ʃm�[�h�ւ̃|�C���^�i�܂��́C�v���~�e�B�u�ԍ�) */
    int right;	/* �E�����ʃm�[�h�ւ̃|�C���^ */
} CSG;

/* �w�i�F */
double Dark[3] = {0.2, 0.2, 0.2};

/* �����̋��� */
double RgbEnv[3] = {0.1, 0.1, 0.1};

/* �����f�[�^�\�� */
typedef struct {
	double x,y,z;	/* �ʒu */
	double r,g,b;	/* �F[0,1] */
} LIGHT;

/* �����f�[�^ */
int NLight;			/* ������ */
LIGHT Light[10];	/* �����̈ʒu�ƐF */

/* �J�����E�X�N���[���f�[�^ */
double C[3]; 	/* ���_���W(�J�����ʒu) */
double B[3]; 	/* �X�N���[�����S�_ */
double VUP[3]; 	/* �A�b�v�x�N�g�� */
int WS; 		/* �摜�� */
int HS; 		/* �摜���� */
double D;		/* �摜�̊g�嗦 */
double W[3];	/* �����P�ʃx�N�g�� */
double U[3];	/* �X�N���[�����Wu�����P�ʃx�N�g�� */
double V[3];	/* �X�N���[�����Wv�����P�ʃx�N�g�� */

/* �v���~�e�B�u�̉A�֐��\�����̃e�[�u�� */
#define M1 200
#define M2 100
double A1[ 4][M1];	/* ���ʂ̉A�֐��\�����̌W��
                       A1[i][j]  i...�W���ԍ�  j...�ʔԍ�
                       �A�֐��\����
                         a0*x + a1*y + a2*z + a3 = 0
                    */
int A1Prim[M1];		/* �v���~�e�B�u�ԍ� */
double A2[10][M2];	/* �Q���Ȗʂ̉A�֐��\�����̌W��
                       A2[i][j]  i...�W���ԍ�  j...�ʔԍ�
                       �A�֐��\����
                         a0*x^2 + a1*y^2 + a2*z^2 +
                         a3*x*y + a4*y*z + a5*z*x +
                         a6*x + a7*y + a8*z + a9 = 0
                    */
int A2Prim[M2];		/* �v���~�e�B�u�ԍ� */
int NS1; 		    /* ���ʂ̐� */
int NS2; 		    /* �Q���Ȗʂ̐� */

/* ��_�̃f�[�^�\�� */
typedef struct {
	double x, y, z; /* ���W�l */
	double t;		/* �p�����[�^�l */
	int type;		/* �ʂ̃^�C�v =1...���� =2...�Q���Ȗ� */
	int sf;			/* �ʔԍ�  type=1�Ȃ�A1[][sf]
	                               =2�Ȃ�A2[][sf] */
	int prim;		/* �v���~�e�B�u�ԍ� */
} PointList;

int Sign[M2];		/* �@���x�N�g���̕���
                        �����Z������Ɩ@���x�N�g���̌������t�]����̂ŁC
                        ���񍷉��Z���{���ꂽ�����J�E���g���āC
                        ������Ȃ� +1 ���Ȃ� -1 �Ƃ���D */
int Nnega[M2];		/* �e�v���~�e�B�u��'-'�̐��𐔂��邽�߂̔z�� */
