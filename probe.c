/* プリミティブrectの陰関数表現式の導出 */
void rectArgs(int id)
{
	/* 6枚の平面 */
	A1[0][NS1] = -1.0; A1[1][NS1] =  0.0; A1[2][NS1] =  0.0;
	A1[3][NS1] =  PDB[id].data[0];
	A1Prim[NS1] = id;
	NS1++;
	A1[0][NS1] =  1.0; A1[1][NS1] =  0.0; A1[2][NS1] =  0.0;
	A1[3][NS1] = -PDB[id].data[1];
	A1Prim[NS1] = id;
	NS1++;
	A1[0][NS1] =  0.0; A1[1][NS1] = -1.0; A1[2][NS1] =  0.0;
	A1[3][NS1] =  PDB[id].data[2];
	A1Prim[NS1] = id;
	NS1++;
	A1[0][NS1] =  0.0; A1[1][NS1] =  1.0; A1[2][NS1] =  0.0;
	A1[3][NS1] = -PDB[id].data[3];
	A1Prim[NS1] = id;
	NS1++;
	A1[0][NS1] =  0.0; A1[1][NS1] =  0.0; A1[2][NS1] = -1.0;
	A1[3][NS1] =  PDB[id].data[4];
	A1Prim[NS1] = id;
	NS1++;
	A1[0][NS1] =  0.0; A1[1][NS1] =  0.0; A1[2][NS1] =  1.0;
	A1[3][NS1] = -PDB[id].data[5];
	A1Prim[NS1] = id;
	NS1++;
}

/* プリミティブcircの陰関数表現式の導出 */
void circArgs(int id)
{
	double x1, y1, r;

	/* 円柱面 */
	r = PDB[id].data[0]; x1 = PDB[id].data[1]; y1 = PDB[id].data[2];
	A2[0][NS2] = 1.0; A2[1][NS2] = 1.0; A2[2][NS2] = 0.0;
	A2[3][NS2] = 0.0; A2[4][NS2] = 0.0; A2[5][NS2] = 0.0;
	A2[6][NS2] = -2.0*x1;
	A2[7][NS2] = -2.0*y1;
	A2[8][NS2] = 0.0;
	A2[9][NS2] = x1*x1 + y1*y1 - r*r;
	A2Prim[NS2] = id;
	NS2++;
	
	/* 下端面と上端面 */
	A1[0][NS1] =  0.0; A1[1][NS1] =  0.0; A1[2][NS1] = -1.0;
	A1[3][NS1] =  PDB[id].data[3];
	A1Prim[NS1] = id;
	NS1++;
	A1[0][NS1] =  0.0; A1[1][NS1] =  0.0; A1[2][NS1] =  1.0;
	A1[3][NS1] = -PDB[id].data[4];
	A1Prim[NS1] = id;
	NS1++;
}

void coneArgs(int id)
{
	double x0, y0, z0, z1, h, r, a, b = 1;

	x0 = PDB[id].data[0]; // x of the center of bottom circle
    y0 = PDB[id].data[1]; // y
    z0 = PDB[id].data[2]; // z
	h  = PDB[id].data[3]; // Height of a cone
	z1 = z0 + h;  // z-value of the center of a cone
	r  = PDB[id].data[4]; // Radius of the bottom circle of a cone
	a = (b * h * h) / (r * r);

	A2[0][NS2] = a; A2[1][NS2] = a; A2[2][NS2] = -b;
	A2[3][NS2] = 0.0; A2[4][NS2] = 0.0; A2[5][NS2] = 0.0;
	A2[6][NS2] = -2.0*a*x0;
	A2[7][NS2] = -2.0*a*y0;
	A2[8][NS2] = 2.0*b*z1;
	A2[9][NS2] = a*x0*x0 + a*y0*y0 - b*z1*z1;
	A2Prim[NS2] = id;
	NS2++;
	
	// Top
	A1[0][NS1] =  0.0; A1[1][NS1] =  0.0; A1[2][NS1] = -1.0;
	A1[3][NS1] =  z1;
	A1Prim[NS1] = id;
	NS1++;
	// Bottom
	A1[0][NS1] =  0.0; A1[1][NS1] =  0.0; A1[2][NS1] =  1.0;
	A1[3][NS1] = -z0;
	A1Prim[NS1] = id;
	NS1++;
}

void spheArgs(int id)
{
	double x1, y1, z1, r;
	r = PDB[id].data[0]; 
	x1 = PDB[id].data[1]; y1 = PDB[id].data[2]; z1 = PDB[id].data[3];
	A2[0][NS2] = 1.0; A2[1][NS2] = 1.0; A2[2][NS2] = 1.0;
	A2[3][NS2] = 0.0; A2[4][NS2] = 0.0; A2[5][NS2] = 0.0;
	A2[6][NS2] = -2.0*x1;
	A2[7][NS2] = -2.0*y1;
	A2[8][NS2] = -2.0*z1;
	A2[9][NS2] = x1*x1 + y1*y1 + z1*z1 - r*r;
	A2Prim[NS2] = id;
	NS2++;
}

/* 陰関数表現式の導出 */
void surfaceArgs(void)
{
	int i;
	
	/* 平面と２次曲面の数の初期化 */
	NS1 = 0;
	NS2 = 0;
	
	/* プリミティブデータから曲面の陰関数表現式の係数を
	   計算し陰関数表現式テーブルに登録する */
	for (i=1; i<=NPri; i++) {
		if      (strcmp(PDB[i].type, "rect") == 0) rectArgs(i);
		else if (strcmp(PDB[i].type, "circ") == 0) circArgs(i);
		else if (strcmp(PDB[i].type, "sphe") == 0) spheArgs(i);
		else if (strcmp(PDB[i].type, "cone") == 0) coneArgs(i);
		else printf("bprim: wrong type (probe.c)\n");
	}
}

/* レイと平面との交点 */
void rayVSplane( 
		double c[], 		/* 視点 */
		double v[],			/* 単位方向ベクトル */
		double a[],			/* 平面式の係数
		                       a0*x + a1*y + a2*z + a3 = 0 */
		int *ans,			/* ans = 1 ... 交点あり
		                           = 0 ... 交点なし（または不定) */
		double *t,          /* 交点のパラメータ値 */
		double x[]          /* 交点座標 */
     )
{
	double s;
	
	s = a[0]*v[0] + a[1]*v[1] + a[2]*v[2];
	if (s == 0) 
		*ans = 0;
	else {
		*ans = 1;
		*t = -(a[0]*c[0] + a[1]*c[1] + a[2]*c[2] + a[3])/s;
		x[0] = c[0] + v[0]*(*t);
		x[1] = c[1] + v[1]*(*t);
		x[2] = c[2] + v[2]*(*t);
	}
}

/* ２次方程式の解 */
void quadeq( 
		double a, double b, double c, 	/* a*x^2 + b*x + c = 0 */
		int *ans,					  	/* 解の判別
		　　　　　　　　　　　　　　　　　　 ans = 2 ... ２解
		                                     ans = 1 ... 重解
		                                     ans = 0 ... 解なし（不定） */
		double x[]						/* 解 */
     )
{
	double d;
	
	if (a != 0) {
		b /= a; c /= a;		/* x^2 + b*x + c = 0 */
		if (c != 0) {
			b /= 2;			/* x^2 + 2*b'*x + c = 0 */
			d = b*b - c;
			if (d > 0) {			/* ２解 */
				*ans = 2;
				if (b > 0) x[0] = -b - sqrt(d);
				else       x[0] = -b + sqrt(d);
				x[1] = c/x[0];
			} else if (d == 0) {	/* 重解 */
				*ans = 1;
				x[0] = x[1] = -b;
			} else {				/* 解なし */
				*ans = 0;
			}
		} else {		/* x^2 + b*x = 0 */
			*ans = 2; 
			x[0] = 0.0;
			x[1] = -b;
		}
	} else if (b != 0) {	/* ２次式でない */
		*ans = 1;
		x[0] = -c/b;
	} else {
		*ans = 0;
	}
}

/* レイと２次曲面との交点 */
void rayVSquadric( 
		double c[], 		/* 視点 */
		double v[],			/* 単位方向ベクトル */
		double a[],			/* ２次曲面式の係数
		                       a0*x^2 + a1*y^2 + a2*z^2 +
		                       a3*x*y + a4*y*z + a5*z*x +
		                       a6*x   + a7*y   + a8*z   + a9 = 0 */
		int *ans,			/* ans = 2 ... 交点あり（２交点）
		                           = 1 ... 交点あり（重解）
		                           = 0 ... 交点なし（または不定) */
		double t[],         /* 交点のパラメータ値 */
		double x[][2]       /* 交点座標  x[i][j]  j=0,1(２交点分） */
     )
{
	int i;
	double b[3];
	
	/* ２次方程式の係数 */
	b[0] = a[0]*v[0]*v[0] + a[1]*v[1]*v[1] + a[2]*v[2]*v[2] +
	       a[3]*v[0]*v[1] + a[4]*v[1]*v[2] + a[5]*v[2]*v[0];
	b[1] = 2*a[0]*v[0]*c[0] + 2*a[1]*v[1]*c[1] + 2*a[2]*v[2]*c[2] + 
           a[3]*(v[0]*c[1] + v[1]*c[0]) +
           a[4]*(v[1]*c[2] + v[2]*c[1]) +
           a[5]*(v[2]*c[0] + v[0]*c[2]) +
           a[6]*v[0] + a[7]*v[1] + a[8]*v[2];
    b[2] = a[0]*c[0]*c[0] + a[1]*c[1]*c[1] + a[2]*c[2]*c[2] +
           a[3]*c[0]*c[1] + a[4]*c[1]*c[2] + a[5]*c[2]*c[0] +
           a[6]*c[0]      + a[7]*c[1]      + a[8]*c[2]      + a[9];
    
    /* 交点のパラメータ値(２次方程式の解) */
	quadeq( b[0], b[1], b[2], ans, t );
	
	/* 交点の座標値 */
	for (i=0; i<*ans; i++) {
		x[0][i] = c[0] + v[0]*t[i];
		x[1][i] = c[1] + v[1]*t[i];
		x[2][i] = c[2] + v[2]*t[i];
	}
}


/* レイと物体(CSG)との交点を求める */
void probe(double c[],      	 /* 始点 */
           double b[],  	     /* レイ上の１点 */
		   int *np, 	    	 /* 交点数 */
		   PointList plist[]     /* 交点リスト */
		  )
{
	double v[3], ap;
	int i, j, k;
	double a[10], t, x[3], t2[2], x2[3][2];
	int ans;
	
	/* 単位方向ベクトル */
	v[0] = b[0] - c[0];
	v[1] = b[1] - c[1];
	v[2] = b[2] - c[2];
	ap = sqrt( v[0]*v[0] + v[1]*v[1] + v[2]*v[2] );
	v[0] = v[0]/ap;
	v[1] = v[1]/ap;
	v[2] = v[2]/ap;

	/* 交点数の初期化 */
	*np = 0;
	
	/* 平面との交点計算 */
	for (i=0; i<NS1; i++) {		/* 各平面について */
		
		/* 平面と直線との交点 */
		for (j=0; j<4; j++) a[j] = A1[j][i];
		rayVSplane( c, v, a, &ans, &t, x );
		
		if (ans > 0) { 		/* 交点あり */
			if (t > 0) {		/* 視点より前 */
				if ((fabs(bshape( 1, x )) < ZERO) &&
				    (fabs(bprim( A1Prim[i], x )) < ZERO)) {	/* 有効交点 */
					plist[*np].x = x[0];
					plist[*np].y = x[1];
					plist[*np].z = x[2];
					plist[*np].t = t;
					plist[*np].type = 1;
					plist[*np].sf = i;
					plist[*np].prim = A1Prim[i];
					(*np)++;
				}
			}
		}

	}
	
	/* ２次曲面との交点計算 */
	for (i=0; i<NS2; i++) {		/* 各２次曲面について */
		
		/* ２次曲面と直線との交点 */
		for (j=0; j<10; j++) a[j] = A2[j][i];
		rayVSquadric( c, v, a, &ans, t2, x2 );
		
		if (ans > 0) { 		/* 交点あり */
			for (k=0; k<ans; k++) {	/* 各交点について */
				if (t2[k] > 0) {		/* 視点より前 */
					x[0] = x2[0][k]; x[1] = x2[1][k]; x[2] = x2[2][k]; 
					if ((fabs(bshape( 1, x )) < ZERO) && 
						(fabs(bprim( A2Prim[i], x )) < ZERO)) { /* 有効交点 */
						plist[*np].x = x[0];
						plist[*np].y = x[1];
						plist[*np].z = x[2];
						plist[*np].t = t2[k];
						plist[*np].type = 2;
						plist[*np].sf = i;
						plist[*np].prim = A2Prim[i];
						(*np)++;
					}
				}
			}
		}

	}
	
}
