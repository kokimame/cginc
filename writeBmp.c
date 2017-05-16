/* 
   unsigned char型配列に格納された
   24ビット-ビットマップデータをBMPファイルに出力する
*/
void bmpout(char *filename, int width, int height)
{
	FILE *fp;
	int i, j, k;

	WORD bfType           = 19778;
	DWORD bfSize;
	WORD bfReserved1      = 0,
	     bfReserved2      = 0;
	DWORD bfOffBits       = 54;
	DWORD biSize          = 40, 
	      biWidth, biHeight;
	WORD biPlanes         = 1,
	     biBitCount       = 24;
	DWORD biCompression   = 0, 
	      biSizeImage, 
	      biXPelsPerMeter = 3780,
	      biYPelsPerMeter = 3780,
	      biClrUsed       = 0, 
	      biClrImportant  = 0;
	
	biWidth     = width;
	biHeight    = height;
	biSizeImage = 3*biWidth*biHeight;
	bfSize      = biSizeImage+bfOffBits;
	
	/* ファイルオープン */
	if ((fp = fopen(filename, "wb"))==NULL) {
		printf("Open error!\n");
		exit(1);
	}
	printf("output file : %s\n", filename);

	/* ヘッダー情報 */
	fwrite(&bfType, sizeof(bfType), 1, fp);
	fwrite(&bfSize, sizeof(bfSize), 1, fp);
	fwrite(&bfReserved1, sizeof(bfReserved1), 1, fp);
	fwrite(&bfReserved2, sizeof(bfReserved2), 1, fp);
	fwrite(&bfOffBits, sizeof(bfOffBits), 1, fp);

	fwrite(&biSize, sizeof(biSize), 1, fp);
	fwrite(&biWidth, sizeof(biWidth), 1, fp);
	fwrite(&biHeight, sizeof(biHeight), 1, fp);
	fwrite(&biPlanes, sizeof(biPlanes), 1, fp);
	fwrite(&biBitCount, sizeof(biBitCount), 1, fp);
	fwrite(&biCompression, sizeof(biCompression), 1, fp);
	fwrite(&biSizeImage, sizeof(biSizeImage), 1, fp);
	fwrite(&biXPelsPerMeter, sizeof(biXPelsPerMeter), 1, fp);
	fwrite(&biYPelsPerMeter, sizeof(biYPelsPerMeter), 1, fp);
	fwrite(&biClrUsed, sizeof(biClrUsed), 1, fp);
	fwrite(&biClrImportant, sizeof(biClrImportant), 1, fp);

	/* ビットマップデータ */
	for (i=0; i<biHeight; i++)
	for (j=0; j<biWidth; j++) {
		for (k=0; k<3; k++) {
			fwrite(&Pixel[i][j][2-k], 1, 1, fp);
		}
	}
	
	fclose(fp);
}


/* [0.0 1.0]のRGBデータを[0 255]に変換して配列Pixel[v][u][3]に格納する */
void storePixel(int u, int v, double rgb[])
{
	int i;
	int c;
	
	for (i=0; i<3; i++) {
		c = 255*rgb[i];
		if (c < 0)   c = 0;
		if (c > 255) c = 255;
		Pixel[v][u][i] = (unsigned char)c;
	}
}
