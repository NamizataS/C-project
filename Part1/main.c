#include "functions.h"

int main() {
    char *xmlPath = malloc(sizeof(char)*100);
    char *dtdPath = malloc(sizeof(char)*100);

    FILE* xmlFile = NULL;
    FILE *dtdFile = NULL;

    while ( !xmlFile ){
        printf("Veuillez entrer le chemin de votre fichier XML\n");
        scanf("%s",xmlPath);
        xmlFile = openFile(xmlPath);
    }

    while ( !dtdFile ){
        printf("Veuillez entrer le chemin de votre fichier DTD\n");
        scanf("%s",dtdPath);
        dtdFile = openFile(dtdPath);
    }

    if ( !checkXML(FileinString(xmlPath)) ){
        printf("Le fichier XML est mal formé\n");
        exit(EXIT_FAILURE);
    } else {
        printf("Le fichier XML est bien formé\n");
    }

    Node *xml = XMLinList(FileinString( xmlPath), xml);
    DTD *dtd = DTDinList(FileinString( dtdPath),dtd);


    if ( !checkDTD(dtd, FileinString(dtdPath)) ){
        printf("La DTD n'est pas valide\n");
        exit(EXIT_FAILURE);
    } else {
        printf("La DTD est valide\n");
    }

    if ( checkXMLandDTD(dtd,xml) && checkDTDandXML(dtd,xml) ){
        printf("Le fichier XML est conforme à la DTD\n");
    } else {
        printf("Le fichier XML n'est pas conforme à la DTD\n");
        exit(EXIT_FAILURE);
    }

    freeXML(xml);
    freeDTD(dtd);


    return EXIT_SUCCESS;
}
