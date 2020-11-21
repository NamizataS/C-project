#include "functions.h"

int main() {
    char *xmlPath = "./../recipe.xml";
    char *dtdPath = "./../recipe.dtd";

    Node *xml;
    DTD *dtd;

    /*if ( !checkXML( FileinString(xmlPath) ) ){
        printf("Il manque une balise ouvrante ou fermante\n");
        return EXIT_FAILURE;
    }*/
    xml = XMLinList( xml, FileinString(xmlPath) );
    dtd = DTDinList( dtd, FileinString(dtdPath) );

    if ( !attributesInList( dtd, FileinString( dtdPath) ) ){
        printf("aie\n");
        return EXIT_FAILURE;
    }
    //printXML(xml);
    freeXML(xml);
    return EXIT_SUCCESS;
}

