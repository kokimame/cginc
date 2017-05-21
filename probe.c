void rectArgs(int id)
{
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

void cyliArgs(int id)
{
    double x0, y0, z0, z1, r;

    x0 = PDB[id].data[0];
	y0 = PDB[id].data[1];
	z0 = PDB[id].data[2];
	z1 = PDB[id].data[3];
	r = PDB[id].data[4];

    A2[0][NS2] = 1.0; A2[1][NS2] = 1.0; A2[2][NS2] = 0.0;
    A2[3][NS2] = 0.0; A2[4][NS2] = 0.0; A2[5][NS2] = 0.0;
    A2[6][NS2] = -2.0*x0;
    A2[7][NS2] = -2.0*y0;
    A2[8][NS2] = 0.0;
    A2[9][NS2] = x0*x0 + y0*y0 - r*r;
    A2Prim[NS2] = id;
    NS2++;
    
    A1[0][NS1] =  0.0; A1[1][NS1] =  0.0; A1[2][NS1] = -1.0;
    A1[3][NS1] =  z0;
    A1Prim[NS1] = id;
    NS1++;
    A1[0][NS1] =  0.0; A1[1][NS1] =  0.0; A1[2][NS1] =  1.0;
    A1[3][NS1] = -z1;
    A1Prim[NS1] = id;
    NS1++;
}

void coneArgs(int id)
{
    double x0, y0, z0, z1, h, r, a, b = 1;

    x0 = PDB[id].data[0]; // x of the center of the base circle
    y0 = PDB[id].data[1]; // y
    z0 = PDB[id].data[2]; // z
	r = PDB[id].data[3]; // Radius of the base circle
    z1  = PDB[id].data[4]; // Cap z value
    h = z1 - z0;
    a = (b * h * h) / (r * r);
	if (h < 0) { printf("Error: Height of cone is negative.\n"); exit(1); }

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
    double x0, y0, z0, r;

    x0 = PDB[id].data[0]; y0 = PDB[id].data[1]; z0 = PDB[id].data[2];
	r = PDB[id].data[3];

    A2[0][NS2] = 1.0; A2[1][NS2] = 1.0; A2[2][NS2] = 1.0;
    A2[3][NS2] = 0.0; A2[4][NS2] = 0.0; A2[5][NS2] = 0.0;
    A2[6][NS2] = -2.0*x0;
    A2[7][NS2] = -2.0*y0;
    A2[8][NS2] = -2.0*z0;
    A2[9][NS2] = x0*x0 + y0*y0 + z0*z0 - r*r;
    A2Prim[NS2] = id;
    NS2++;
}

void surfaceArgs(void)
{
    int i;
    
    NS1 = 0;
    NS2 = 0;
    
    for (i=1; i<=NPri; i++) {
        if      (strcmp(PDB[i].type, "rect") == 0) rectArgs(i);
        else if (strcmp(PDB[i].type, "cyli") == 0) cyliArgs(i);
        else if (strcmp(PDB[i].type, "sphe") == 0) spheArgs(i);
        else if (strcmp(PDB[i].type, "cone") == 0) coneArgs(i);
        else printf("bprim: wrong type (probe.c)\n");
    }
}

void rayVSplane( 
        double c[],
        double v[],
        double a[],
        int *ans,
        double *t,
        double x[]
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

void quadeq( 
        double a, double b, double c,
        int *ans,
        double x[]
     )
{
    double d;
    
    if (a != 0) {
        b /= a; c /= a;     /* x^2 + b*x + c = 0 */
        if (c != 0) {
            b /= 2;         /* x^2 + 2*b'*x + c = 0 */
            d = b*b - c;
            if (d > 0) {
                *ans = 2;
                if (b > 0) x[0] = -b - sqrt(d);
                else       x[0] = -b + sqrt(d);
                x[1] = c/x[0];
            } else if (d == 0) {
                *ans = 1;
                x[0] = x[1] = -b;
            } else {
                *ans = 0;
            }
        } else {        /* x^2 + b*x = 0 */
            *ans = 2; 
            x[0] = 0.0;
            x[1] = -b;
        }
    } else if (b != 0) {
        *ans = 1;
        x[0] = -c/b;
    } else {
        *ans = 0;
    }
}

void rayVSquadric( 
        double c[],
        double v[],
        double a[],         /*
                               a0*x^2 + a1*y^2 + a2*z^2 +
                               a3*x*y + a4*y*z + a5*z*x +
                               a6*x   + a7*y   + a8*z   + a9 = 0 */
        int *ans,
        double t[],
        double x[][2]
     )
{
    int i;
    double b[3];
    
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
    
    quadeq( b[0], b[1], b[2], ans, t );
    
    for (i=0; i<*ans; i++) {
        x[0][i] = c[0] + v[0]*t[i];
        x[1][i] = c[1] + v[1]*t[i];
        x[2][i] = c[2] + v[2]*t[i];
    }
}

void probe(double c[],
           double b[],
           int *np,
           PointList plist[]
          )
{
    double v[3], ap;
    int i, j, k;
    double a[10], t, x[3], t2[2], x2[3][2];
    int ans;
    
    v[0] = b[0] - c[0];
    v[1] = b[1] - c[1];
    v[2] = b[2] - c[2];
    ap = sqrt( v[0]*v[0] + v[1]*v[1] + v[2]*v[2] );
    v[0] = v[0]/ap;
    v[1] = v[1]/ap;
    v[2] = v[2]/ap;

    *np = 0;
    
    for (i=0; i<NS1; i++) {
        for (j=0; j<4; j++) a[j] = A1[j][i];
        rayVSplane( c, v, a, &ans, &t, x );
        
        if (ans > 0) {
            if (t > 0) {
                if ((fabs(bshape( 1, x )) < ZERO) &&
                    (fabs(bprim( A1Prim[i], x )) < ZERO)) {
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
    
    for (i=0; i<NS2; i++) {
        for (j=0; j<10; j++) a[j] = A2[j][i];
        rayVSquadric( c, v, a, &ans, t2, x2 );
        
        if (ans > 0) {
            for (k=0; k<ans; k++) {
                if (t2[k] > 0) {
                    x[0] = x2[0][k]; x[1] = x2[1][k]; x[2] = x2[2][k]; 
                    if ((fabs(bshape( 1, x )) < ZERO) && 
                        (fabs(bprim( A2Prim[i], x )) < ZERO)) {
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
