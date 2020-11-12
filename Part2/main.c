#include "functions.h"

int main() {
    char *dtdPath = "./../recipe.dtd";
    DTD *dtd;

    dtd = DTDinList( FileinString(dtdPath), dtd);

    if ( !attributesInList(dtd,FileinString(dtdPath)) ){
        printf("Les attributs ne sont pas au bon format ou ne correspondent à aucune balise existante\n");
        return EXIT_FAILURE;
    }

    //printDTD(dtd);
    freeDTD(dtd);

    return 0;
}
