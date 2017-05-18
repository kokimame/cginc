/*
    If use #include "cg.h" here, it causes error related to
	redefinition; different basic types or multiple initialization
	of variables defined in cg.h.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
	char *type;
	double rgb[3];
	double kd;
	double ks;
	int n;
	double data[20];
} Primitive;

typedef struct {
	char op;
	int left;
	int right;
} CSG;

int NPri = 0;
Primitive PDB[100];
// CSG ST[100];

void load_primitives();
void load_expression();
void load_cone();
void load_rect();
void load_cyli();
void load_sphe();
void parse_expr(char *expr);

int main()
{
	PDB[0].type = "S";
	// ST[0].op = 'S';

    FILE *fp;
    char buffer[256];
	char buf2[256];
    char *pch;
	char *filename = "csgfile.txt";

    if((fp = fopen(filename, "r")) == NULL){
        printf("File open error: %s\n", filename);
        exit(1);
    }

    while(fgets(buffer, sizeof(buffer), fp)){
        printf("%s", buffer);
		strcpy(buf2, buffer);
		strtok(buffer, "\n");
		pch = strtok(buffer, ",");
		if (isdigit(pch[0])) {
			NPri += 1;
			load_primitives();
		}
		else if (pch[0] == '(') {
			printf("expression: %s\n", buf2);
			parse_expr(buf2, 0);
		}
    }

    fclose(fp);
}

void parse_expr(char *expr, s)
{
	int i = s;
	
	for (; expr[i] != ')'; i++) {
		if (expr[i] == '(') continue;
		else if (isdigit(expr[i])) printf("%c", expr[i]);
		else if (expr[i] == '+' || expr[i] == '-' || expr[i] == '*') {
		}
		else if (expr[i] == ',') continue;
	}

	printf("\n");
} 

void load_primitives()
{
	char *pch = strtok(NULL, ","); // Set next character -> PDB.type
    if(!strcmp(pch, "cone")) load_cone();
    else if(!strcmp(pch, "rect")) load_rect();
    else if(!strcmp(pch, "cyli")) load_cyli();
    else if(!strcmp(pch, "sphe")) load_sphe();
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
    while(pch != NULL && *pch != '\n'){
		ret = strtol(pch, NULL, 10);
		data[cnt] = (double)ret;
		cnt += 1;
        pch = strtok(NULL, ">,<");
    }
	for (i = 0; i < cnt; i++) printf("%.2f ", data[i]);

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
void load_expression()
{

}
