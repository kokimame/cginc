/*
 * 輝度計算のための関数群
 */

/* CSGトリーの探索(差演算が施される回数) */
void treeWalk(int lr, int id, int Count, char op)
{
	if (lr == 1 && op == '-') Count++;
	if (ST[id].op == 'E') {
		Nnega[ST[id].left] = Count;
	} else {
		treeWalk(-1, ST[id].left,  Count, ST[id].op);
		treeWalk( 1, ST[id].right, Count, ST[id].op);
	}
}

/* 各プリミティブの法線ベクトルの符号 */
void signPrim(void)
{
	int i;
	int Count = 0;

	/* '-'の数 */
	for (i=1; i<=NPri; i++) Nnega[i] = 0;
	treeWalk( 1, 1, Count, ST[0].op);
	
	/* 法線ベクトルの符号 */
	for (i=1; i<=NPri; i++) {
		if (Nnega[i]%2 == 0) Sign[i] =  1;
		else                 Sign[i] = -1;
	}
}

/* 単位ベクトル */
/* 始点p1[]と終点p2[]を与えて，始点から終点へ向かう単位ベクトルを求める */
void unitVector( double p1[], double p2[], double vv[] )
{
	double d;
	vv[0] = p2[0] - p1[0];
	vv[1] = p2[1] - p1[1];
	vv[2] = p2[2] - p1[2];
	d = sqrt( vv[0]*vv[0] + vv[1]*vv[1] + vv[2]*vv[2] );
	vv[0] = vv[0]/d; vv[1] = vv[1]/d; vv[2] = vv[2]/d;
}

/* 単位法線ベクトル */
void normalVector(
		int type, 	/* 面タイプ */
		int sf,		/* 面番号 */
		int prim,	/* プリミティブ番号 */
		double x[],	/* 点の座標 */
		double n[]	/* 単位法線ベクトル */
	 )
{
	double d;
	if (type == 1) {		/* 平面 */
		n[0] = A1[0][sf];
		n[1] = A1[1][sf];
		n[2] = A1[2][sf];
	} else if (type == 2) {	/* ２次曲面 */
		n[0] = 2*A2[0][sf]*x[0] + A2[3][sf]*x[1] + A2[5][sf]*x[2] + A2[6][sf];
		n[1] = 2*A2[1][sf]*x[1] + A2[3][sf]*x[0] + A2[4][sf]*x[2] + A2[7][sf];
		n[2] = 2*A2[2][sf]*x[2] + A2[4][sf]*x[1] + A2[5][sf]*x[0] + A2[8][sf];
	} else printf("normalVector: wrong type\n");
	
	/* 単位ベクトル化 */	
	d = sqrt(n[0]*n[0] + n[1]*n[1] + n[2]*n[2]);
	n[0] = n[0]/d; n[1] = n[1]/d; n[2] = n[2]/d;
	
	/* 法線の向き */
	n[0] = Sign[prim]*n[0];
	n[1] = Sign[prim]*n[1];
	n[2] = Sign[prim]*n[2];
}

/* 可視点を求め情報を得る */
void vPoint(
		int np,				/* 交点数 */
		PointList plist[],	/* 交点データ */
		double x[],			/* 可視点の座標 */
		int *type,			/* 可視点の面タイプ */
		int *sf,			/* 可視点の面番号 */
		int *prim			/* 可視点のプリミティブ番号 */
	 )
{
	int i, id;
	double tmin;
	
	/* 可視点（t最小の交点）を求める */
	id = 0; tmin = plist[0].t;
	for (i=1; i<np; i++) {
		if (plist[i].t < tmin) {
			id = i; tmin = plist[i].t;
		}
	}
	x[0] = plist[id].x; x[1] = plist[id].y; x[2] = plist[id].z;
	*type = plist[id].type; *sf = plist[id].sf; *prim = plist[id].prim;
}

/* 光源強度の正規化 */
void normalizeLight(void)
{
	int j;
	double df, total;
	
	/* Red */
	df = Dark[0] + RgbEnv[0];
	total = 0;
	for (j=0; j<NLight; j++) total += Light[j].r;
	for (j=0; j<NLight; j++) Light[j].r = (1.0 - df)/total * Light[j].r;
	
	/* Green */
	df = Dark[1] + RgbEnv[1];
	total = 0;
	for (j=0; j<NLight; j++) total += Light[j].g;
	for (j=0; j<NLight; j++) Light[j].g = (1.0 - df)/total * Light[j].g;

	/* Blue */
	df = Dark[2] + RgbEnv[2];
	total = 0;
	for (j=0; j<NLight; j++) total += Light[j].b;
	for (j=0; j<NLight; j++) Light[j].b = (1.0 - df)/total * Light[j].b;
}

/* 輝度計算 */
void bright(
		int np,				/* 交点の数 */
		PointList plist[],	/* 交点データ */
		double rgb[]		/* 可視点の輝度 */
	 )
{
	int i, j, k;
	double diff;
	double x[3];	/* 可視点 */
	double vl[3]; 	/* 単位光線ベクトル */
	double vn[3];	/* 単位法線ベクトル */
	double vr[3];	/* 単位正反射ベクトル */
	double vv[3];	/* 単位視線ベクトル */
	double rgbObject[3];	/* 物体の色 */
	int type, sf, prim;
	double d, sln, srv, kd, ks, rgbLight[3], y[3], x2[3];
	double theta, w;
	
	/* 可視点 */
	vPoint( np, plist, x, &type, &sf, &prim );

	if(PDB[prim].type[0] == 'c' && PDB[prim].type[1] == 'i' && x[2] != PDB[prim].data[4])
	{
		double x0, y0, z0;
		double px, py, pz;
		int u, v, h;

		x0 = PDB[prim].data[1]; y0 = PDB[prim].data[2]; z0 = PDB[prim].data[3];
		h = PDB[prim].data[4] - z0;
		px = x[0]; py = x[1]; pz = x[2];
		theta = atan2(fabs(py - y0),fabs(px - x0));
		w = pz - z0;

		u = (int)((WTex * theta)/(2 * PI));
		v = (int)((HTex * w) / h);

		for(j=0; j<3; j++)
			rgb[j] = (double)TexImage[v][u][j] / 255;

		return;
	}
	
	/* 物体の色 */
	for (j=0; j<3; j++)	rgbObject[j] = PDB[prim].rgb[j];

	/* 単位法線ベクトル */
	normalVector( type, sf, prim, x, vn );
	
	/* 輝度の初期化 */
	for (j=0; j<3; j++)	rgb[j] = Dark[j];
	
	/* 環境光による輝度 */
	for (j=0; j<3; j++)
		rgb[j] += rgbObject[j] * RgbEnv[j];
	
	/* 輝度の計算 */
	for (i=0; i<NLight; i++) {	/* 各光源について */

		/* 単位光線ベクトル */
		y[0] = Light[i].x; y[1] = Light[i].y; y[2] = Light[i].z; 
		unitVector( x, y, vl );

		probe(x, y, &np, plist);
		if(np > 1) continue;
		
		/* 光線ベクトルと法線ベクトルの内積 L・N */
		sln = vl[0]*vn[0] + vl[1]*vn[1] + vl[2]*vn[2];
		if (sln < 0.0) sln = 0.0;

		/* 単位正反射ベクトル */
		vr[0] = 2*sln*vn[0] - vl[0];
		vr[1] = 2*sln*vn[1] - vl[1];
		vr[2] = 2*sln*vn[2] - vl[2];
		d = sqrt( vr[0]*vr[0] + vr[1]*vr[1] + vr[2]*vr[2] );
		vr[0] = vr[0]/d; vr[1] = vr[1]/d; vr[2] = vr[2]/d;

		/* 単位視線ベクトル */
		unitVector( x, C, vv );

		/* 反射係数 */
		kd = PDB[prim].kd; ks = PDB[prim].ks;
		
		/* 光源のRGB */
		rgbLight[0] = Light[i].r; 
		rgbLight[1] = Light[i].g; 
		rgbLight[2] = Light[i].b; 



		for (j=0; j<3; j++) {	/* 各RBGについて */
		
			/* 拡散反射による輝度 */
			rgb[j] += kd * rgbObject[j] * rgbLight[j] * sln;
			
			/* 鏡面反射による輝度 */
			srv = vr[0]*vv[0] + vr[1]*vv[1] + vr[2]*vv[2];
			if (srv < 0.0 || sln == 0.0) srv = 0.0;
			rgb[j] += ks * rgbLight[j] * pow(srv, PDB[prim].n);
			          
		}
	}
	
}