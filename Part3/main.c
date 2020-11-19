#include "functions.h"

int main() {
    char *xmlPath = "./../recipe.xml";

    Node *xml;

    /*if ( !checkXML( FileinString(xmlPath) ) ){
        printf("Il manque une balise ouvrante ou fermante\n");
        return EXIT_FAILURE;
    }*/
    xml = XMLinList( xml, FileinString(xmlPath) );
    //printXML(xml);
    //freeXML(xml);
    return EXIT_SUCCESS;
}

