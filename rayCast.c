#include "cg.h"
#include "test_load.mdl"
#include "loader.c"
#include "beval.c"
#include "probe.c"
#include "camera.c"
#include "bright.c"
#include "writeBmp.c"

int main(void)
{
    int u, v;
    double S[3];
    int np;
    PointList plist[M1 + 2 * M2];
    double rgb[3];
    char output[256];
    char *envfile = "envsetting.csv";
    char *csgfile = "csgfile.txt";
    char command[256];

    setup_csg(csgfile);

    signPrim();
    surfaceArgs();
    loadEnvSetting(envfile, output, sizeof(output)); // from camera.c 
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
    strcpy(command, "open ");
    strcat(command, output);

#ifdef __APPLE__
    // popen(command, "w");
#endif
#ifdef WIN32
    _popen(command, "w");
#endif

    return 0;
}
