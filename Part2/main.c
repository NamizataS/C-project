#include "functions.h"

int main() {
    char *xmlPath = malloc(sizeof(char)*100);
    char *dtdPath = malloc(sizeof(char)*100);
    DTD *dtd;
    Node *xml;

    FILE* xmlFile = NULL;
    FILE *dtdFile = NULL;

    while ( !xmlFile ){
        printf("Veuillez entrer le chemin de votre fichier XML\n");
        scanf("%s",xmlPath);
        if ( strstr(xmlPath,".xml") != NULL ){
            xmlFile = openFile(xmlPath);
        } else{
            xmlFile = NULL;
        }
    }

    while ( !dtdFile ){
        printf("Veuillez entrer le chemin de votre fichier DTD\n");
        scanf("%s",dtdPath);
        if ( strstr( dtdPath, ".dtd") != NULL ){
            dtdFile = openFile(dtdPath);
        }else{
            dtdFile = NULL;
        }
    }

    if ( !checkXML(FileinString(xmlPath))){
        printf("Il manque une balise fermante ou ouvrante\n");
        return EXIT_FAILURE;
    }
    xml = XMLinList( FileinString(xmlPath),xml );
    dtd = DTDinList( FileinString(dtdPath), dtd);

    if ( !attributesInList(dtd,FileinString(dtdPath)) ){
        printf("Les attributs ne sont pas au bon format ou ne correspondent à aucune balise existante\n");
        return EXIT_FAILURE;
    }

    if ( !checkDTD( dtd,FileinString(dtdPath) ) ){
        printf("La DTD n'est pas valide\n");
    }

    if ( !checkAttributesinXML( dtd, xml ) || !checkAttributesinDTD( dtd, xml )){
        printf("Les attributs ne sont pas bons\n");
        return EXIT_FAILURE;
    }

    if ( !checkXMLandDTD(dtd,xml) || !checkDTDandXML(dtd,xml) ){
        printf("Le XML n'est pas conforme à la DTD\n");
        return EXIT_FAILURE;
    }

    printf("Le XML est conforme à la DTD\n");
    freeDTD(dtd);
    freeXML(xml);

    return EXIT_SUCCESS;
}
