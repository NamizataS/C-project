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

    if ( dtd == NULL ){
        printf("La DTD n'est pas valide\n");
    }
    if ( !attributesInList( dtd, FileinString( dtdPath) ) ){
        printf("Les attributs ne sont pas au bon format ou ne correspondent à aucune balise exitante\n");
        return EXIT_FAILURE;
    }
    if ( !checkAttributesinXML( dtd, xml ) ){
        printf("Les attributs ne sont pas bons\n");
        return EXIT_FAILURE;
    }
    if ( ! checkXMLandDTD(dtd,xml) || !checkDTDandXML(dtd,xml) ){
        printf("Le XML n'est pas conforme à la DTD\n");
    }

    freeXML(xml);
    freeDTD( dtd );
    return EXIT_SUCCESS;
}

