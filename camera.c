void camera(void)
{
    int i;
    double sw, vw, sv;
    
    for (i=0; i<3; i++) W[i] = B[i] - C[i];
    sw = W[0]*W[0] + W[1]*W[1] + W[2]*W[2];
    sw = sqrt(sw);
    for (i=0; i<3; i++) W[i] = W[i]/sw;
    
    vw = VUP[0]*W[0] + VUP[1]*W[1] + VUP[2]*W[2];
    for (i=0; i<3; i++) V[i] = VUP[i] - vw*W[i];
    sv = V[0]*V[0] + V[1]*V[1] + V[2]*V[2];
    sv = sqrt(sv);
    for (i=0; i<3; i++) V[i] = V[i]/sv;
    
    U[0] = W[1]*V[2] - W[2]*V[1];
    U[1] = W[2]*V[0] - W[0]*V[2];
    U[2] = W[0]*V[1] - W[1]*V[0];
}

void screen2world( int u, int v, double s[] )
{
    s[0] = B[0] + D*(u - WS/2)*U[0] + D*(v - HS/2)*V[0];
    s[1] = B[1] + D*(u - WS/2)*U[1] + D*(v - HS/2)*V[1];
    s[2] = B[2] + D*(u - WS/2)*U[2] + D*(v - HS/2)*V[2];
}
