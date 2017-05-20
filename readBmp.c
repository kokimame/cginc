void readBmp(
        char *filename,
        unsigned char image[Y_SIZE][X_SIZE][3],
        DWORD *width,
        DWORD *height
            )
{
    FILE *fp;
    int i, j, k;
    WORD dum;
    DWORD ddum;
    
    if ((fp = fopen(filename, "rb"))==NULL) {
        printf("readBmp: Open error!\n");
        exit(1);
    }
    printf("input file : %s\n", filename);

    fread(&dum, sizeof(dum), 1, fp);
    fread(&ddum, sizeof(ddum), 1, fp);
    fread(&dum, sizeof(dum), 1, fp);
    fread(&dum, sizeof(dum), 1, fp);
    fread(&ddum, sizeof(ddum), 1, fp);

    fread(&ddum, sizeof(ddum), 1, fp);
    fread(width, sizeof(*width), 1, fp);
    fread(height, sizeof(*height), 1, fp);
    fread(&dum, sizeof(dum), 1, fp);
    fread(&dum, sizeof(dum), 1, fp);
    fread(&ddum, sizeof(ddum), 1, fp);
    fread(&ddum, sizeof(ddum), 1, fp);
    fread(&ddum, sizeof(ddum), 1, fp);
    fread(&ddum, sizeof(ddum), 1, fp);
    fread(&ddum, sizeof(ddum), 1, fp);
    fread(&ddum, sizeof(ddum), 1, fp);

    for (i=0; i<*height; i++)
    for (j=0; j<*width; j++) {
        for (k=0; k<3; k++) {
            fread(&image[i][j][2-k], 1, 1, fp);
        }
    }
    fclose(fp);
}
