//
// Created by Namizata Sangaré on 08/11/2020.
//

#include "functions.h"

bool checkXML( Node *xml, char *root ){
    int count = 0;
    while (xml){
        if ( xml->name != NULL && strcmp(xml->name, root ) == 0 ){
            count += 1;
        }
        if ( xml->child != NULL ){
            xml = xml->child;
        } else {
            xml = xml->sibling;
        }
    }
    return count > 1 ? false : true;
}

Node *XMLinList( char *string, Node *xml ){

    char *tag = extractName( string );
    if ( tag == NULL ){
        return xml;
    }

    if ( checkEnd(tag) ){
        return XMLinList( strRemove(string,tag),xml);
    }
    if ( strstr(tag,"<?xml") != NULL ){
        if (strstr(tag,"<?xml version=\"1.0\"") != NULL || strstr(tag,"<?xml version=\"1.1\"") != NULL ){
            return XMLinList( strRemove(string, tag), xml );
        } else {
            return NULL;
        }
    }
    if ( strstr(tag, "<!DOCTYPE") != NULL ){
        return XMLinList( strRemove(string,tag), xml );
    }

    char *name = isolateContent(tag);
    strRemove(name, " ");
    removeChar(name,11);
    if ( xml == NULL ){
        xml = malloc(sizeof(Node));
        xml->name = name;
        xml->child = malloc(sizeof(Node));
        xml->child = XMLinList(strRemove(string,tag),xml->child);
    } else{
        xml->name = malloc( sizeof(char)*(strlen(name)+1));
        xml->name = name;
        string = strRemove(string,tag);
        tag = extractName(string);
        xml->content = extractContent(tag,'<');
        xml->sibling = malloc(sizeof(Node));
        xml->sibling = XMLinList(strRemove(string,tag),xml->sibling);
    }

    return xml;
}

void freeXML( Node *xml ){
    if (xml->child != NULL ){
        freeXML(xml->child);
    }
    if (xml->sibling != NULL ){
        freeXML(xml->sibling);
    }
    free(xml);
}
