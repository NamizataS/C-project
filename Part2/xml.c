//
// Created by Namizata Sangaré on 11/11/2020.
//

#include "functions.h"

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
    strRemove(string,tag);
    removeChar(string,11);
    if ( *string == '<' ){
        xml = malloc(sizeof(Node));
        removeSpaceandTab(name);
        xml->name = name;
        xml->child = malloc(sizeof(Node));
        xml->child = XMLinList(string,xml->child);
    }
    else{
        if ( strchr(name,'=') != NULL ){
            char *element = extractUntil(name,' ' );
            removeSpaceandTab(element);
            xml->name = malloc( sizeof(char)*(strlen(name)+1));
            xml->name = element;
            strRemove(name,element);
            char *attributeName = extractUntil(name,'=');
            strRemove(name,attributeName);
            removeChar(attributeName,'=');
            char *value = extractUntil(name, ' ' );
            if ( value == NULL ){
                value = name;
                if ( xml->attributes == NULL ){
                    xml->attributes = malloc(sizeof(xmlAttribute));
                    xml->attributes = newXMLAttribute(attributeName,value);
                }
                strRemove(string,tag);
                tag = extractName(string);
                xml->content = extractContent(tag,'<');
                xml->sibling = malloc(sizeof(Node));
                xml->sibling = XMLinList(strRemove(string,tag),xml->sibling);
            } else {
                strRemove(name,value);
                xml->attributes = malloc(sizeof(xmlAttribute));
                xml->attributes = newXMLAttribute(attributeName,value);
                fillXMLAttribute(xml->attributes,name);
                tag = extractName(string);
                xml->content = extractContent(tag,'<');
                xml->sibling = malloc(sizeof(Node));
                xml->sibling = XMLinList(strRemove(string,tag),xml->sibling);
            }
        } else {
            xml->name = malloc(sizeof(char) * (strlen(name)+1));
            xml->name = name;
            string = strRemove(string,tag);
            tag = extractName(string);
            xml->content = extractContent(tag,'<');
            xml->sibling = malloc(sizeof(Node));
            xml->sibling = XMLinList(strRemove(string,tag),xml->sibling);
        }

    }

    return xml;
}

xmlAttribute *newXMLAttribute( char *name, char *content ){
    xmlAttribute *newAttribute = malloc(sizeof(xmlAttribute));
    removeSpaceandTab(name);
    while ( strchr(content,34) != NULL ){
        removeChar( content,34 );
    }
    removeSpaceandTab(content);
    newAttribute->name = name;
    newAttribute->content = content;
    newAttribute->next = NULL;

    return newAttribute;
}

bool fillXMLAttribute( xmlAttribute *attribute, char *string ){
    char *name = extractUntil(string,'=');
    strRemove(string,name);
    removeChar(name,'=');
    char *content = extractUntil(string,' ');
    if ( content == NULL ){
        content = string;
        attributeInXML(attribute,newXMLAttribute(name,content));
        return true;
    } else {
        strRemove(string,content);
        attributeInXML(attribute,newXMLAttribute(name,content));
        return fillXMLAttribute(attribute,string);
    }
}

void attributeInXML( xmlAttribute *list, xmlAttribute *insert ){

    xmlAttribute *current = list;
    while ( current->next != NULL ){
        current = current->next;
    }
    current->next = insert;
}

bool checkXML( Node *xml,char *string ){

    while ( xml && xml->name ){
        char *tag = malloc( sizeof( char ) * (strlen(xml->name) + 1) );
        char *closing = malloc(sizeof(char) * (strlen(xml->name)+1));
        strcpy(tag,xml->name);
        strcpy(closing,"</");
        strcat(closing,tag);
        strcat(closing,">");

        if ( strstr(string,closing) == NULL ){
            return false;
        }
        strRemove(string,closing);
        removeChar(closing,'/');
        if ( xml->attributes != NULL ){
            removeChar(closing,'>');
        }
        if ( strstr(string,closing) == NULL ){
            return false;
        }
        strRemove(string, closing);

        if ( xml->child != NULL ){
            xml = xml->child;
        } else {
            xml = xml->sibling;
        }
    }

    return true;
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