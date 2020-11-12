#include "functions.h"

int main() {
    char *dtdPath = "./../recipe.dtd";
    DTD *dtd;

    dtd = DTDinList( FileinString(dtdPath), dtd);

    attributesInList(dtd,FileinString(dtdPath));
    if ( dtd->attributes == NULL ){
        printf("Becca is a clown\n");
    }
    //printDTD(dtd);
    freeDTD(dtd);

    return 0;
}
