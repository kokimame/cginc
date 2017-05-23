#include "cg.h"
#include "loader.c"
#include "beval.c"
#include "probe.c"
#include "camera.c"
#include "tracing.c"
#include "writeBmp.c"

int main(void)
{
	int i;
	int u, v; // Used for position of pixels at the screen
    double S[3];	// Position of Pixel in World system
    int np;		// Stores the number of intersection between two points
    PointList plist[M1 + 2 * M2]; // List of intersected points
    double rgb[3];		// RGB for each pixel at the screen
    char output[256];	// Output image name
    char *envfile = "envsetting.csv";
    char command[256];

	loadEnvSetting(envfile, output, sizeof(output)); // from camera.c 

    signPrim();
    surfaceArgs();
    camera();

    for (v = 0; v<HS; v++) {
        printf("%d ", v);
        for (u = 0; u<WS; u++) {
            screen2world(u, v, S);
            probe(C, S, &np, plist);
            if (np <= 0) {
                storePixel(u, v, Dark);
            }
            else {
				bright(np, plist, rgb);
                storePixel(u, v, rgb);
            }
        }
    }

    bmpout(output, WS, HS);
    strcpy(command, "mspaint ");
    strcat(command, output);
    _popen(command, "w");

    return 0;
}
