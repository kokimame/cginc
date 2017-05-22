double bshape( int, double [] );
double minIn( double [], int );
double maxIn( double [], int );
double brect( int, double [] );
double bcyli( int, double [] );
double bsphe( int, double [] );
double bcone( int, double [] );
double bprim( int, double [] );

double bshape( int id, double x[] )
{
    double f[2];
    
    if (ST[id].op == 'E')
        return bprim(ST[id].left, x);
    else if (ST[id].op == '+' ) {
        f[0] = bshape( ST[id].left,  x );
        f[1] = bshape( ST[id].right, x );
        return minIn( f, 2 );
    }
    else if (ST[id].op == '-' ) {
        f[0] = bshape( ST[id].left,  x );
        f[1] = - bshape( ST[id].right, x );
        return maxIn( f, 2 );
    }
    else if(ST[id].op == '*'){
        f[0] = bshape( ST[id].left,  x );
        f[1] = bshape( ST[id].right, x );

        return maxIn(f, 2);
    }
    else 
        printf("bshape: Bad operator\n");
}

double bprim( int id, double x[] )
{
    if      (strcmp(PDB[id].type, "rect") == 0)
        return brect(id, x);
    else if (strcmp(PDB[id].type, "cyli") == 0)
        return bcyli(id, x);
    else if(strcmp(PDB[id].type, "sphe") == 0)
        return bsphe(id, x);
    else if(strcmp(PDB[id].type, "cone") == 0)
        return bcone(id, x);
    else
        printf("bprim: wrong type (beval.c)\n");
}

double brect(int id, double x[])
{
    double x1, x2, y1, y2, z1, z2;
    double f[6], ff;

    x1 = PDB[id].data[0];
    x2 = PDB[id].data[1];
    y1 = PDB[id].data[2];
    y2 = PDB[id].data[3];
    z1 = PDB[id].data[4];
    z2 = PDB[id].data[5];

    f[0] = x[0] - x1;
    f[1] = x2 - x[0];
    f[2] = x[1] - y1;
    f[3] = y2 - x[1];
    f[4] = x[2] - z1;
    f[5] = z2 - x[2];
    ff = - minIn( f, 6 );

    return ( ff * fabs(ff) );
}

double bcyli( int id, double x[] )
{
    double x0, y0, z0, z1, r, f[3], ff;

    x0 = PDB[id].data[0];
    y0 = PDB[id].data[1];
    z0 = PDB[id].data[2];
    z1 = PDB[id].data[3];
	r = PDB[id].data[4];

    f[0] = r*r - (x[0] - x0)*(x[0] - x0) - (x[1] - y0)*(x[1] - y0);
    f[1] = (x[2] - z0) * fabs( x[2] - z0 );
    f[2] = (z1 - x[2]) * fabs( z1 - x[2] );
    ff = - minIn( f, 3 );

    return (ff);
}

double bcone(int id, double x[])
{
    double x0, y0, z0, z1, h, r, a, b = 1, f[3];

    x0 = PDB[id].data[0]; // x of the center of the base circle
    y0 = PDB[id].data[1]; // y
    z0 = PDB[id].data[2]; // z
	r = PDB[id].data[3]; // Radius of the base circle
    z1 = PDB[id].data[4]; // Height of a cone
    h = z1 - z0;  // z-value of the center of a cone
	if (h < 0) { printf("Error: Height of cone is negative\n"); exit(1); }
    a = (b * h * h) / (r * r);

    f[0] = -a * (x[0] - x0)*(x[0] - x0) -
           a * (x[1] - y0)*(x[1] - y0) +
           b * (x[2] - z1)*(x[2] - z1);

    f[1] = (x[2] - z0) * fabs( x[2] - z0 );
    f[2] = (z1 - x[2]) * fabs( z1 - x[2] );

    return -minIn(f, 3);
}

double bsphe(int id, double x[])
{
    double x0, y0, z0, r;
    
    x0 = PDB[id].data[0];
    y0 = PDB[id].data[1];
    z0 = PDB[id].data[2];
	r = PDB[id].data[3];

    return  fabs(x[0] - x0)*fabs(x[0] - x0) +
            fabs(x[1] - y0)*fabs(x[1] - y0) +
            fabs(x[2] - z0)*fabs(x[2] - z0) - r*r;
}

double minIn( double f[], int n )
{
    double min;
    int i;
    min = f[0];
    for (i=1; i<n; i++) {
        if (f[i] < min) min = f[i];
    }
    return (min);
}

double maxIn( double f[], int n )
{
    double max;
    int i;
    max = f[0];
    for (i=1; i<n; i++) {
        if (f[i] > max) max = f[i];
    }
    return (max);
}
