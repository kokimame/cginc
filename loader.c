/*
If use #include "cg.h" here, it causes error related to
redefinition; different basic types or multiple initialization
of variables defined in cg.h.
*/

void loadEnvSetting(char *filename, char *output, int o_size);
void loadCsg(FILE *fp);
void loadPrimitives();
int parseExpr(char *expr);
void showTheTree();
void setModelData(double *data, int size);
void setColorInfo(double *data);
void loadCone();
void loadRect();
void loadCyli();
void loadSphe();

void loadEnvSetting(char *filename, char *output, int o_size)
{
	int i;
	FILE *fp;

	if ((fp = fopen(filename, "r")) == NULL) {
		printf("File open error: %s", filename);
		exit(1);
	}

	// Load setting for CSG Models.
	loadCsg(fp);
	printf("Loaded CSG\n");
	// Setting for the environment such as light source, camera etc
	fgets(output, o_size, fp);
	strtok(output, "\n");

	fscanf(fp, "%d", &NLight);
	printf("Number of Lights : %d\n", NLight);
	for (i = 0; i < NLight; i++) {
		fscanf(fp, "%lf,%lf,%lf", &Light[i].x, &Light[i].y, &Light[i].z);
		fscanf(fp, "%lf,%lf,%lf", &Light[i].r, &Light[i].g, &Light[i].b);
		printf("Position of Light%d(%2.2lf,%2.2lf,%2.2lf)\n", i + 1, Light[i].x, Light[i].y, Light[i].z);
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


void loadCsg(FILE *fp)
{
	PDB[0].type = "S";
	ST[0].op = 'S';

	char buffer[256];
	char bufexp[256];
	char *pch;

	while (fgets(buffer, sizeof(buffer), fp)) {
		printf("%s", buffer);
		strcpy(bufexp, buffer);
		strtok(buffer, "\n");
		pch = strtok(buffer, ",");
		if (isdigit(pch[0])) {
			NPri += 1;
			loadPrimitives();
		}
		else if (pch[0] == '(') {
			parseExpr(bufexp);
			showTheTree();
			break;
		}
	}
}
void showTheTree()
{
	int j;

	for (j = 1; j <= NTree; j++) {
		char op = ST[j].op;
		if (op == '*' || op == '+' || op == '-')
			printf("%d: %c, l:%d, r:%d\n", j, op, ST[j].left, ST[j].right);
		else
			printf("%d: %c, l:%d\n", j, op, ST[j].left);
	}
}

int parseExpr(char *expr)
{
	static int i;
	int NCurrent = ++NTree; // Store the current number at Tree

	while (expr[i] == ' ' || expr[i] == '(' || expr[i] == ')') { i++; };
	if (expr[i] == '\n') {
		printf("Error: Read the end of expr\n"); exit(1);
	}

	if (expr[i] == '+' || expr[i] == '-' || expr[i] == '*') {
		ST[NCurrent].op = expr[i];
		i++;
		ST[NCurrent].left = parseExpr(expr);
		i++;
		ST[NCurrent].right = parseExpr(expr);
		return NCurrent;
	}
	else if (isdigit(expr[i]) != 0) {
		ST[NCurrent].op = 'E';
		ST[NCurrent].left = expr[i] - '0';
		return NCurrent;
	}
}

void loadPrimitives()
{
	char *pch = strtok(NULL, ","); // Set next character -> PDB.type
	if (!strcmp(pch, "cone")) loadCone();
	else if (!strcmp(pch, "rect")) loadRect();
	else if (!strcmp(pch, "cyli")) loadCyli();
	else if (!strcmp(pch, "sphe")) loadSphe();
	else {
		printf("Error: unknown primitive type %s\n", pch);
		exit(1);
	}
}

void setModelData(double *data, int size)
{
	char *pch;
	int cnt = 0;
	long ret;

	pch = strtok(NULL, ",<");
	while (pch != NULL && *pch != '\n') {
		ret = strtol(pch, NULL, 10);
		data[cnt] = (double)ret;
		cnt += 1;
		pch = strtok(NULL, ">,<");
	}
	// for (i = 0; i < cnt; i++) printf("%.2f ", data[i]);
}

void setColorInfo(double *data)
{
	PDB[NPri].rgb[0] = data[0];
	PDB[NPri].rgb[1] = data[1];
	PDB[NPri].rgb[2] = data[2];
	PDB[NPri].kd = data[3];
	PDB[NPri].ks = data[4];
	PDB[NPri].n = (int)data[5];
}

void loadCone()
{
	int i;
	double data[32];

	setModelData(data, sizeof(data));
	PDB[NPri].type = "cone";
	setColorInfo(data);   // Set color quality to data, using it up to #5
	// Cone needs 5 variables to define its shape
	for (i = 6; i <= 10; i++) PDB[NPri].data[i - 6] = data[i];
}

void loadRect()
{
	int i;
	double data[32];

	setModelData(data, sizeof(data));
	PDB[NPri].type = "rect";
	setColorInfo(data);
	// Rectangle needs 6 variables to define its shape
	for (i = 6; i <= 11; i++) PDB[NPri].data[i - 6] = data[i];
}

void loadSphe()
{
	int i;
	double data[32];
	
	setModelData(data, sizeof(data));
	PDB[NPri].type = "sphe";
	setColorInfo(data);
	// Sphere needs 4 varibles to define its shape
	for (i = 6; i <= 9; i++) PDB[NPri].data[i - 6] = data[i];
}

void loadCyli()
{
	int i;
	double data[32];

	setModelData(data, sizeof(data));
	PDB[NPri].type = "cyli";
	setColorInfo(data);
	// Cylinder needs 5 variables to define its shape
	for (i = 6; i <= 10; i++) PDB[NPri].data[i - 6] = data[i];
}
