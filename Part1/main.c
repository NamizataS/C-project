#include "functions.h"

int main() {
    char *xmlPath = "./../recipe.xml";
    char *dtdPath = "./../recipe.dtd";
    Node *xml;
    DTD *dtd;

    xml = XMLinList(FileinString( xmlPath), xml);

    if ( xml == NULL ){
        printf("Le fichier XML est mal formé\n");
        return EXIT_FAILURE;
    } else {
        printf("Le fichier XML est bien formé\n");
    }

    dtd = DTDinList(FileinString( dtdPath),dtd);


    if ( !checkDTD(dtd, FileinString(dtdPath)) ){
        printf("La DTD n'est pas valide\n");
        return EXIT_FAILURE;
    } else {
        printf("La DTD est valide\n");
    }

    if ( checkXMLandDTD(dtd,xml) && checkDTDandXML(dtd,xml) ){
        printf("Le fichier XML est conforme à la DTD\n");
    } else {
        printf("Le fichier XML n'est pas conforme à la DTD\n");
        return EXIT_FAILURE;
    }

    freeXML(xml);
    freeDTD(dtd);

    return EXIT_SUCCESS;
}
