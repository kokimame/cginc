/* 
   24�r�b�g-�r�b�g�}�b�v�f�[�^(BMP�t�@�C��)��
   �ǂݍ��� unsigned char�^�z��Ɋi�[����D
  [����] �摜�̉����͂S�̔{���ł��邱�ƁI
*/
void readBmp(
		char *filename, /* BMP�t�@�C���� */
		unsigned char image[Y_SIZE][X_SIZE][3],
		                /* 24�r�b�gRGB�摜�z�� */
		DWORD *width, 	/* �摜�� */
		DWORD *height	/* �摜���� */
		    )
{
	FILE *fp;
	int i, j, k;
	WORD dum;
	DWORD ddum;
	
	/* �t�@�C���I�[�v�� */
	if ((fp = fopen(filename, "rb"))==NULL) {
		printf("readBmp: Open error!\n");
		exit(1);
	}
	printf("input file : %s\n", filename);

	/* �w�b�_�[��� */
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

	/* �r�b�g�}�b�v�f�[�^ */
	for (i=0; i<*height; i++)
	for (j=0; j<*width; j++) {
		for (k=0; k<3; k++) {
			fread(&image[i][j][2-k], 1, 1, fp);
		}
	}
	
	fclose(fp);
}
