void loadEnvSetting(char *filename, char *o_file, int o_size)
{
    int i;
    FILE *fp;

    if ((fp = fopen(filename, "r")) == NULL) {
        printf("File open error: %s", filename);
        exit(1);
    }

    fgets(o_file, o_size, fp);
    strtok(o_file, "\n");

    fscanf(fp, "%d", &NLight);
    printf("Number of Lights : %d\n", NLight);
    for (i = 0; i < NLight; i++) {
        fscanf(fp, "%lf,%lf,%lf", &Light[i].x, &Light[i].y, &Light[i].z);
        fscanf(fp, "%lf,%lf,%lf", &Light[i].r, &Light[i].g, &Light[i].b);
        printf("Position of Light%d(%2.2lf,%2.2lf,%2.2lf)\n", i+1, Light[i].x, Light[i].y, Light[i].z);
        printf("Color of Light%d(%2.2lf,%2.2lf,%2.2lf)\n", i + 1, Light[i].r, Light[i].g, Light[i].b);

    }
    fscanf(fp, "%lf,%lf,%lf", &C[0], &C[1], &C[2]);
    printf("Point of View (%2.2lf,%2.2lf,%2.2lf)\n", C[0], C[1], C[2]);
    
    fscanf(fp, "%lf,%lf,%lf", &B[0], &B[1], &B[2]);
    printf("Center of the Screen (%2.2lf,%2.2lf,%2.2lf)\n", B[0], B[1], B[2]);
    
    fscanf(fp, "%lf,%lf,%lf", &VUP[0], &VUP[1], &VUP[2]);
    printf("Up-Vector (%2.2lf,%2.2lf,%2.2lf)\n", VUP[0], VUP[1], VUP[2]);
    
    fscanf(fp, "%d,%d", &WS, &HS);
    fscanf(fp, "%lf", &D);
    printf("Size of Output image : (%d,%d)\n", WS, HS);
    printf("Resize : %2.2lf\n", D);
}

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
