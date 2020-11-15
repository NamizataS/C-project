//
// Created by Namizata Sangaré on 08/11/2020.
//

#include "functions.h"

bool checkXML( char *string ){
    removeChar(string,11);
    //printf("%s\n",string);
    if ( string == NULL ){
        return true;
    }
    if ( *string == '<'){
        char *tag = extractUntil(string,'>');
        if ( strstr(tag,"<?xml version=\"1.0\"") != NULL || strstr(tag,"<?xml version=\"1.1\"") != NULL && strstr(tag,"?>") != NULL ){
            return checkXML( strRemove(string,tag));
        }
        if ( strstr(tag, "<!DOCTYPE") != NULL ){
            return checkXML( strRemove(string,tag) );
        }
        if ( strchr(tag,'=') == NULL ){
            strRemove(string,tag);
            removeChar( tag, '<' );
            char *closing = malloc(sizeof(char) * (strlen(tag) + 1));
            strcpy(closing,"</");
            strcat(closing,tag);
            if ( strstr(string,closing) == NULL ){
                return false;
            } else{
                //printf("%s\n",closing);
                return checkXML( strRemove(string,closing));
            }
        } else {
            strRemove(string,tag);
            tag = extractUntil( tag, ' ' );
            removeChar(tag,' ');
            removeChar(tag,'<');
            char *closing = malloc(sizeof(char)*(strlen(tag)+1));
            strcpy(closing,"</");
            strcat(closing,tag);
            strcat(closing,">");
            if ( strstr(string,closing) == NULL ){
                return false;
            }else{
                return checkXML( strRemove(string,closing));
            }
        }

    }else{
        char *content = extractUntil(string,'<');
        removeChar(content,'<');
        return checkXML( strRemove(string,content));
    }
    return true;
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

    strRemove(string,tag);
    char *name = isolateContent(tag);
    removeChar(string,11);
    strRemove(name, " ");
    removeChar(name,11);
    if ( *string == '<' ){
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
