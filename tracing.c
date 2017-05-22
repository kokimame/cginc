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

void signPrim(void)
{
    int i;
    int Count = 0;

    for (i=1; i<=NPri; i++) Nnega[i] = 0;
    treeWalk( 1, 1, Count, ST[0].op);
    
    for (i=1; i<=NPri; i++) {
        if (Nnega[i]%2 == 0) Sign[i] =  1;
        else                 Sign[i] = -1;
    }
}

void unitVector( double p1[], double p2[], double vv[] )
{
    double d;
    vv[0] = p2[0] - p1[0];
    vv[1] = p2[1] - p1[1];
    vv[2] = p2[2] - p1[2];
    d = sqrt( vv[0]*vv[0] + vv[1]*vv[1] + vv[2]*vv[2] );
    vv[0] = vv[0]/d; vv[1] = vv[1]/d; vv[2] = vv[2]/d;
}

void normalVector(
        int type,
        int sf,
        int prim,
        double x[],
        double n[]
     )
{
    double d;
    if (type == 1) {
        n[0] = A1[0][sf];
        n[1] = A1[1][sf];
        n[2] = A1[2][sf];
    } else if (type == 2) {
        n[0] = 2*A2[0][sf]*x[0] + A2[3][sf]*x[1] + A2[5][sf]*x[2] + A2[6][sf];
        n[1] = 2*A2[1][sf]*x[1] + A2[3][sf]*x[0] + A2[4][sf]*x[2] + A2[7][sf];
        n[2] = 2*A2[2][sf]*x[2] + A2[4][sf]*x[1] + A2[5][sf]*x[0] + A2[8][sf];
    } else printf("normalVector: wrong type\n");
    
    d = sqrt(n[0]*n[0] + n[1]*n[1] + n[2]*n[2]);
    n[0] = n[0]/d; n[1] = n[1]/d; n[2] = n[2]/d;
    
    n[0] = Sign[prim]*n[0];
    n[1] = Sign[prim]*n[1];
    n[2] = Sign[prim]*n[2];
}

void vPoint(
        int np,
        PointList plist[],
        double x[],
        int *type,
        int *sf,
        int *prim
     )
{
    int i, id;
    double tmin;
    
    id = 0; tmin = plist[0].t;
    for (i=1; i<np; i++) {
        if (plist[i].t < tmin) {
            id = i; tmin = plist[i].t;
        }
    }
    x[0] = plist[id].x; x[1] = plist[id].y; x[2] = plist[id].z;
    *type = plist[id].type; *sf = plist[id].sf; *prim = plist[id].prim;
}

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

void textureMappingToCone(double *x, double rgb[], int prim)
{
	double x0, y0, z0;
	double px, py, pz, theta, w;
	int u, v, h, i, j, k;

	x0 = PDB[prim].data[1]; y0 = PDB[prim].data[2]; z0 = PDB[prim].data[3];
	h = PDB[prim].data[4] - z0;
	px = x[0]; py = x[1]; pz = x[2];
	theta = atan2(fabs(py - y0), fabs(px - x0));
	w = pz - z0;

	u = (int)((WTex * theta) / (2 * PI));
	v = (int)((HTex * w) / h);

	for (j = 0; j<3; j++)
		rgb[j] = (double)TexImage[v][u][j] / 255;
}

void initializeRGB(double  rgb[], double rgbObject[])
{
	int j;
	for (j = 0; j<3; j++) rgb[j] = Dark[j];

	for (j = 0; j<3; j++)
		rgb[j] += rgbObject[j] * RgbEnv[j];
}


void reflected_light(
	int np, PointList plist[], double rgb[])
{
	int i, j, k;
	double x[3];	// Position of visible point
	double vl[3];	// Unit vector to light source
	double vn[3];	// Unit surface normal vector
	double vr[3];	// Unit reflected vector
	double vv[3];	// Unit vector to camera
	double rgbObject[3];
	int type, sf, prim;
	double d, sln, srv, kd, ks, rgbLight[3], y[3];

	// Set visible point to x and its Model id to prim
	vPoint(np, plist, x, &type, &sf, &prim);

	// Store the color of the model of the visible point
	for (j = 0; j < 3; j++) rgbObject[j] = PDB[prim].rgb[j];

	// Set unit surface normal vector to vn
	normalVector(type, sf, prim, x, vn);

	// Initialize RGB
	for (j = 0; j<3; j++) rgb[j] = Dark[j];

	// Add atmospheric lighting to RGB
	for (j = 0; j<3; j++)
		rgb[j] += rgbObject[j] * RgbEnv[j];

	// Compute RGB at the given pixel for every light source
	for (i = 0; i < NLight; i++) {
		y[0] = Light[i].x; y[1] = Light[i].y; y[2] = Light[i].z;
		// Set unit vector to the light source
		unitVector(x, y, vl);

		probe(x, y, &np, plist);
		if (np > 1) continue;

		sln = vl[0] * vn[0] + vl[1] * vn[1] + vl[2] * vn[2];
		if (sln < 0.0) sln = 0.0;

		vr[0] = 2 * sln*vn[0] - vl[0];
		vr[1] = 2 * sln*vn[1] - vl[1];
		vr[2] = 2 * sln*vn[2] - vl[2];
		d = sqrt(vr[0] * vr[0] + vr[1] * vr[1] + vr[2] * vr[2]);
		// Set reflected vector to vr
		vr[0] = vr[0] / d; vr[1] = vr[1] / d; vr[2] = vr[2] / d;

		unitVector(x, C, vv);

		kd = PDB[prim].kd; ks = PDB[prim].ks;

		rgbLight[0] = Light[i].r;
		rgbLight[1] = Light[i].g;
		rgbLight[2] = Light[i].b;

		for (j = 0; j < 3; j++) {
			rgb[j] += kd * rgbObject[j] * rgbLight[j] * sln;
			srv = vr[0] * vv[0] + vr[1] * vv[1] + vr[2] * vv[2];
			if (srv < 0.0 || sln == 0.0) srv = 0.0;
			rgb[j] += ks * rgbLight[j] * pow(srv, PDB[prim].n);
		}
	}
}

void bright(
        int np,
        PointList plist[],
        double rgb[]
     )
{
    int i, j, k;
    double x[3];	// Position of visible point
    double vl[3];	// Unit vector to light source
    double vn[3];	// Unit surface normal vector
    double vr[3];	// Unit reflected vector
    double vv[3];	// Unit vector to camera
    double rgbObject[3];
    int type, sf, prim;
    double d, sln, srv, kd, ks, rgbLight[3], y[3];
    
	// Set visible point to x and its Model id to prim
    vPoint( np, plist, x, &type, &sf, &prim );

	// Store the color of the model of the visible point
	for (j = 0; j < 3; j++) rgbObject[j] = PDB[prim].rgb[j];

	// Set unit surface normal vector to vn
    normalVector( type, sf, prim, x, vn );

	// Initialize RGB
	for (j = 0; j<3; j++) rgb[j] = Dark[j];

	// Add atmospheric lighting to RGB
	for (j = 0; j<3; j++)
		rgb[j] += rgbObject[j] * RgbEnv[j];

	// Compute RGB at the given pixel for every light source
	for (i = 0; i < NLight; i++) {
		y[0] = Light[i].x; y[1] = Light[i].y; y[2] = Light[i].z;
		// Set unit vector to the light source
		unitVector(x, y, vl);

		probe(x, y, &np, plist);
		if (np > 1) continue;

		sln = vl[0] * vn[0] + vl[1] * vn[1] + vl[2] * vn[2];
		if (sln < 0.0) sln = 0.0;

		vr[0] = 2 * sln*vn[0] - vl[0];
		vr[1] = 2 * sln*vn[1] - vl[1];
		vr[2] = 2 * sln*vn[2] - vl[2];
		d = sqrt(vr[0] * vr[0] + vr[1] * vr[1] + vr[2] * vr[2]);
		// Set reflected vector to vr
		vr[0] = vr[0] / d; vr[1] = vr[1] / d; vr[2] = vr[2] / d;

		unitVector(x, C, vv);

		kd = PDB[prim].kd; ks = PDB[prim].ks;

		rgbLight[0] = Light[i].r;
		rgbLight[1] = Light[i].g;
		rgbLight[2] = Light[i].b;

		for (j = 0; j < 3; j++) {
			rgb[j] += kd * rgbObject[j] * rgbLight[j] * sln;
			srv = vr[0] * vv[0] + vr[1] * vv[1] + vr[2] * vv[2];
			if (srv < 0.0 || sln == 0.0) srv = 0.0;
			rgb[j] += ks * rgbLight[j] * pow(srv, PDB[prim].n);
		}

		
		// ===== Reflection test =====
		double kr1 = 0.8;
		double rgbReflected[3];
		double r1[3] = { x[0] + vr[0], x[1] + vr[1] ,x[2] + vr[2] };
		probe(x, r1, &np, plist);
		if (np > 1) {
			reflected_light(np, plist, rgbReflected);
			for (j = 0; j < 3; j++) rgb[j] += rgbReflected[j] * kr1;
		}		
	}
}
