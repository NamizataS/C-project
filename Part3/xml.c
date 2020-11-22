//
// Created by Namizata Sangaré on 16/11/2020.
//

#include "functions.h"

bool checkXML( char *string ){
    removeChar(string,11);
    if ( string == NULL ){
        return true;
    }
    if ( *string == '<'){
        char *tag = extractUntil(string,'>');
        if ( (strstr(tag,"<?xml version=\"1.0\"") != NULL || strstr(tag,"<?xml version=\"1.1\"") != NULL) && strstr(tag,"?>") != NULL ){
            return checkXML( strRemove(string,tag));
        }
        if ( strstr(tag, "<!DOCTYPE") != NULL ){
            return checkXML( strRemove(string,tag) );
        }
        if ( strchr(tag,'=') == NULL ){
            strRemove(string,tag);
            removeChar( tag, '<' );
            char *closing = malloc(sizeof(char) * (strlen(tag) + 1));
            while ( strchr(tag,-1) != NULL ){
                removeChar(tag,-1);
            }
            strcpy(closing,"</");
            strcat(closing,tag);
            if ( strstr(string,closing) == NULL ){
                return false;
            } else{
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
}
Node *XMLinList( Node *xml,char *string ){
    char *tag = extractName( string );
    if ( tag == NULL ){
        return xml;
    }
    if ( strstr(tag,"<?xml") != NULL ){
        return XMLinList( xml, strRemove(string,tag));
    }
    if ( strstr(tag, "<!DOCTYPE") != NULL ){
        return XMLinList( xml,strRemove(string,tag) );
    }

    if ( checkEnd(tag) ){
        strRemove(string,tag);
        strRemove(tag,"</");
        removeChar( tag, '>');
        removeSpaceandTab(tag);
        if ( strcmp(tag,xml->name) == 0){
            return XMLinList(xml,string);
        } else {
            while ( strcmp(tag,xml->name) != 0 ){
                if ( xml->parent != NULL ){
                    xml = xml->parent;
                } else{
                    xml = xml->olderSibling;
                }
            }
            return XMLinList(xml,string);
        }
    }

    strRemove( string, tag );
    removeChar( string, 11);
    if ( *string == '<' ){
        removeChar(tag,'<');
        removeChar(tag,'>');
        if ( strchr(tag,'=') == NULL ){
            if ( xml == NULL ){
                xml = newRoot( tag, NULL );
                xml->child->parent = xml;
                return XMLinList( xml->child,string );
            } else {
                removeSpaceandTab(tag);
                if ( xml->name == NULL ){
                    xml->name = tag;
                    xml->child = malloc(sizeof(Node));
                    xml->child->parent = xml;
                } else{
                    return XMLinList( fillRoot(xml,tag, NULL),string);
                }
            }
        } else{
            char *element = extractUntil(tag,' ');
            strRemove(tag,element);
            removeSpaceandTab(element);
            char *attributeName = extractUntil(tag,'=');
            strRemove(tag,attributeName);
            removeChar(attributeName,'=');
            removeSpaceandTab(attributeName);
            char *value = extractUntil(tag,' ');
            xmlAttribute *attribute;
            if ( value == NULL ){
                value = tag;
                removeSpaceandTab(tag);
                attribute = newXMLAttribute(attributeName,value);
            } else{
                strRemove(tag,value);
                removeSpaceandTab(value);
                attribute = newXMLAttribute(attributeName,value);
                fillXMLAttribute(attribute,tag);
            }
            if ( xml == NULL ){
                xml = newRoot( element, attribute );
                xml->child->parent = xml;
                return XMLinList( xml->child,string );
            } else {
                if ( xml->name == NULL ){
                    xml->name = element;
                    xml->attributes = attribute;
                    xml->child = malloc(sizeof(Node));
                    xml->child->parent = xml;
                    return XMLinList( xml->child,string );
                } else{
                    return XMLinList( fillRoot(xml,element, attribute ),string);;
                }
            }
        }
    } else{
        char *name = extractUntil(tag,'>');
        if ( strchr(tag,'=') == NULL ){
            removeChar(name,'<');
            removeChar(name,'>');
            removeSpaceandTab(name);
            tag = extractName( string );
            char *content = extractUntil( tag, '<' );
            removeChar(content,'<');
            removeSpaceandTab(content);
            if ( xml->name == NULL || xml == NULL ){
                xml->name = name;
                xml->content = content;
                return XMLinList(xml,strRemove(string,tag));

            } else{
                fillSibling( xml, newLeaf(name,content, NULL));
                return XMLinList(xml,strRemove(string,tag));
            }
        } else {
            char *element = extractUntil(name, ' ' );
            strRemove(name,element);
            removeSpaceandTab(element);
            char *attributeName = extractUntil(name,'=');
            strRemove(name,attributeName);
            removeSpaceandTab(attributeName);
            char *value = extractUntil(name, ' ' );
            xmlAttribute *attribute;
            if ( value == NULL ){
                value = name;
                removeSpaceandTab(value);
                attribute = newXMLAttribute(attributeName,value);
            } else {
                strRemove( name, value);
                removeSpaceandTab(value);
                attribute = newXMLAttribute( attributeName,value);
                fillXMLAttribute(attribute,name);
            }
            tag = extractName(string);
            char *content = extractContent(tag,'<');
            if ( xml->name == NULL || xml == NULL ){
                xml->name = element;
                xml->content = content;
                xml->attributes = attribute;
                return XMLinList(xml,strRemove(string,tag));
            } else{
                fillSibling( xml, newLeaf(element,content,attribute));
                return XMLinList(xml,strRemove(string,tag));
            }

        }
    }

    return xml;
}

Node *newRoot( char *name, xmlAttribute *attributes ){
    removeSpaceandTab(name);
    Node *newRoot = malloc(sizeof(Node));
    newRoot->name = name;
    newRoot->parent = NULL;
    newRoot->child = malloc(sizeof(Node));
    newRoot->sibling = NULL;
    newRoot->attributes = attributes;

    return newRoot;
}

Node *newLeaf( char *name, char *content, xmlAttribute *attributes ){
    Node *newLeaf = malloc(sizeof(Node));
    newLeaf->name = name;
    newLeaf->content = content;
    newLeaf->attributes = attributes;
    newLeaf->parent = NULL;
    newLeaf->child = NULL;
    newLeaf->sibling = NULL;

    return newLeaf;

}

Node *fillRoot( Node *xml, char *name, xmlAttribute *attributes ){
    removeSpaceandTab( name );
    Node *current = xml;
    while ( current->sibling != NULL ){
        current = current->child;
    }
    current->sibling = malloc(sizeof(Node));
    current->sibling->name = name;
    current->sibling->attributes = attributes;
    current->sibling->olderSibling = current;
    current->sibling->child = malloc(sizeof(Node));
    current->sibling->child->parent = current->sibling;
    return xml->sibling->child;
}

void fillSibling( Node *sibling, Node *insert ){
    Node *current = sibling;
    while ( current->sibling != NULL ){
        current = current->sibling;
    }
    current->sibling = insert;
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

bool checkStatusXML( status status, char *string, xmlAttribute *attribute ){
    int count = 0;
    while ( attribute ){
        if ( strcmp(attribute->name,string) == 0 ){
            count += 1;
        }
        attribute = attribute->next;
    }

    switch (status) {
        case REQUIRED:{
            if ( count == 1 ){
                return true;
            }
            break;
        }
        case IMPLIED:{
            if ( count == 0 || count == 1 ){
                return true;
            }
            break;
        }

    }
    return false;
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
