#include "cg.h"
#include "cone.mdl"
#include "beval.c"
#include "probe.c"
#include "camera.c"
#include "bright.c"
#include "writeBmp.c"

void main(void)
{
    int u, v;
    double S[3];
    int np;
    PointList plist[M1 + 2 * M2];
    double rgb[3];
    char output[256];
    char *setting = "envsetting.csv";
    char command[256];

    signPrim();
    surfaceArgs();
    loadEnvSetting(setting, output, sizeof(output)); // from camera.c 
    camera();

    for (v=0; v<HS; v++) {
        printf("%d ",v);
        for (u=0; u<WS; u++) {
            screen2world( u, v, S );
            probe( C, S, &np, plist );
            if (np <= 0) {  
                storePixel(u, v, Dark);
            } else {
                bright(np, plist, rgb);
                storePixel(u, v, rgb);
            }
        }
    }
	
    bmpout(output, WS, HS);
    strcpy(command, "mspaint ");
    strcat(command, output);
    _popen(command, "w");
}
