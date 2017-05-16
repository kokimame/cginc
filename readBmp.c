/* 
   24ビット-ビットマップデータ(BMPファイル)を
   読み込み unsigned char型配列に格納する．
  [注意] 画像の横幅は４の倍数であること！
*/
void readBmp(
		char *filename, /* BMPファイル名 */
		unsigned char image[Y_SIZE][X_SIZE][3],
		                /* 24ビットRGB画像配列 */
		DWORD *width, 	/* 画像幅 */
		DWORD *height	/* 画像高さ */
		    )
{
	FILE *fp;
	int i, j, k;
	WORD dum;
	DWORD ddum;
	
	/* ファイルオープン */
	if ((fp = fopen(filename, "rb"))==NULL) {
		printf("readBmp: Open error!\n");
		exit(1);
	}
	printf("input file : %s\n", filename);

	/* ヘッダー情報 */
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

	/* ビットマップデータ */
	for (i=0; i<*height; i++)
	for (j=0; j<*width; j++) {
		for (k=0; k<3; k++) {
			fread(&image[i][j][2-k], 1, 1, fp);
		}
	}
	
	fclose(fp);
}
