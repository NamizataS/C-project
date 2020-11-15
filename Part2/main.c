#include "functions.h"

int main() {
    char *dtdPath = "./../recipe.dtd";
    char *xmlPath = "./../recipe.xml";
    DTD *dtd;
    Node *xml;

    xml = XMLinList( FileinString(xmlPath),xml );
    dtd = DTDinList( FileinString(dtdPath), dtd);

    if ( !attributesInList(dtd,FileinString(dtdPath)) ){
        printf("Les attributs ne sont pas au bon format ou ne correspondent à aucune balise existante\n");
        return EXIT_FAILURE;
    }
    if ( !checkXML( xml,FileinString(xmlPath) ) ){
        printf("The xml file is missing a closing or opening tag\n");
        return EXIT_FAILURE;
    }
    //printDTD(dtd);
    freeDTD(dtd);
    freeXML(xml);

    return 0;
}
