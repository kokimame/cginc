/*
If use #include "cg.h" here, it causes error related to
redefinition; different basic types or multiple initialization
of variables defined in cg.h.
*/

void load_csg(char *filename);
void load_primitives();
int parse_expr(char *expr);
void show_the_tree();
void load_expression();
void load_cone();
void load_rect();
void load_cyli();
void load_sphe();

void load_csg(char *filename)
{
	PDB[0].type = "S";
	ST[0].op = 'S';

	FILE *fp;
	char buffer[256];
	char bufexp[256];
	char *pch;

	if ((fp = fopen(filename, "r")) == NULL) {
		printf("File open error: %s\n", filename);
		exit(1);
	}

	while (fgets(buffer, sizeof(buffer), fp)) {
		printf("%s", buffer);
		strcpy(bufexp, buffer);
		strtok(buffer, "\n");
		pch = strtok(buffer, ",");
		if (isdigit(pch[0])) {
			NPri += 1;
			load_primitives();
		}
		else if (pch[0] == '(') {
			parse_expr(bufexp);
			show_the_tree();
		}
	}
	fclose(fp);
}
void show_the_tree()
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

int parse_expr(char *expr)
{
	static int i;
	int NCurrent = ++NTree; // Store the current number at Tree

	while (expr[i] == ' ' || expr[i] == '(' || expr[i] == ')') { i++; };
	if (expr[i] == '\n') return -1;

	if (expr[i] == '+' || expr[i] == '-' || expr[i] == '*') {
		ST[NCurrent].op = expr[i];
		i++;
		ST[NCurrent].left = parse_expr(expr);
		i++;
		ST[NCurrent].right = parse_expr(expr);
		return NCurrent;
	}
	else if (isdigit(expr[i]) != 0) {
		ST[NCurrent].op = 'E';
		ST[NCurrent].left = expr[i] - '0';
		return NCurrent;
	}
}

void load_primitives()
{
	char *pch = strtok(NULL, ","); // Set next character -> PDB.type
	if (!strcmp(pch, "cone")) load_cone();
	else if (!strcmp(pch, "rect")) load_rect();
	else if (!strcmp(pch, "cyli")) load_cyli();
	else if (!strcmp(pch, "sphe")) load_sphe();
	else {
		printf("Error: unknown primitive type %s\n", pch);
		exit(1);
	}

}

void load_cone()
{
	char *pch;
	double data[11];
	int cnt = 0, i;
	long ret;

	pch = strtok(NULL, ",<");
	while (pch != NULL && *pch != '\n') {
		ret = strtol(pch, NULL, 10);
		data[cnt] = (double)ret;
		cnt += 1;
		pch = strtok(NULL, ">,<");
	}
	// for (i = 0; i < cnt; i++) printf("%.2f ", data[i]);

	PDB[NPri].type = "cone";
	PDB[NPri].rgb[0] = data[0];
	PDB[NPri].rgb[1] = data[1];
	PDB[NPri].rgb[2] = data[2];
	PDB[NPri].kd = data[3];
	PDB[NPri].ks = data[4];
	PDB[NPri].n = (int)data[5];
	for (i = 6; i <= 10; i++) PDB[NPri].data[i - 6] = data[i];

}

void load_rect()
{
}

void load_sphe()
{
}

void load_cyli()
{
}