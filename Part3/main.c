#include "functions.h"

int main() {

    char *xmlPath = malloc(sizeof(char)*100);
    char *dtdPath = malloc(sizeof(char)*100);

    FILE* xmlFile = NULL;
    FILE *dtdFile = NULL;

    Node *xml;
    DTD *dtd;

    while ( !xmlFile ){
        printf("Rentrez le chemin de votre fichier XML\n");
        scanf("%s",xmlPath);
        xmlFile = openFile(xmlPath);
    }

    while ( !dtdFile ){
        printf("Rentrez le chemin de votre fichier DTD\n");
        scanf("%s",dtdPath);
        dtdFile = openFile(dtdPath);
    }

    if ( !checkXML( FileinString(xmlPath) ) ){
        printf("Il manque une balise ouvrante ou fermante\n");
        return EXIT_FAILURE;
    }

    xml = XMLinList( xml, FileinString(xmlPath) );
    dtd = DTDinList( dtd, FileinString(dtdPath) );

    if ( dtd == NULL ){
        printf("La DTD n'est pas valide\n");
    }

    if ( !attributesInList( dtd, FileinString( dtdPath) ) ){
        printf("Les attributs ne sont pas au bon format ou ne correspondent à aucune balise exitante\n");
        return EXIT_FAILURE;
    }

    if ( !checkAttributesinXML( dtd, xml ) || !checkAttributesinDTD( dtd, xml )){
        printf("Les attributs ne sont pas bons\n");
        return EXIT_FAILURE;
    }

    if ( ! checkXMLandDTD(dtd,xml) || !checkDTDandXML(dtd,xml) ){
        printf("Le XML n'est pas conforme à la DTD\n");
    }

    printf("Le fichier XML est conforme à la DTD\n");
    freeXML(xml);
    freeDTD( dtd );
    return EXIT_SUCCESS;
}

