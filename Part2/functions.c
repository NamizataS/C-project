//
// Created by Namizata Sangaré on 11/11/2020.
//

#include "functions.h"

FILE *openFile( char *filename ){
    FILE *pf;
    pf = fopen(filename, "r+b" ); //Ouverture du fichier
    if ( !pf ){
        perror(filename); // Si le fichier n'a pas pu être ouvert print cette phrase
        exit(EXIT_FAILURE);
    }
    return pf;
}

void closeFile( FILE *pf ){
    fclose(pf);
}

int lengthOfString( char *filename ){
    FILE *pf = openFile(filename);
    int count = 0;
    int c;
    for (c = getc(pf); c != EOF ; c = getc(pf)) {
        count = count + 1;
    }
    closeFile(pf);
    return count;
}

void strip( char *string ){
    char *tmpString = string;
    while ( *string != '\0' ){
        if ( *string != '\n' ){
            *tmpString++ = *string++;
        } else {
            ++(*string);
        }
    }
    *tmpString = '\0';
}

void removeExtraWhiteSpaces( char *string ) {
    char *tmpString = string;
    while ( *string != '\0'){
        if ( *string == ' ' && ( *(string + 1) == ' ' || *(string - 1) == ' ' ) ){
            string++;
        } else {
            *tmpString++ = *string++;
        }
    }
    *tmpString = '\0';
}

char *extractName( char *string ){
    if ( string != NULL ){
        char *position_ptr = strchr(string, '>');
        if ( position_ptr != NULL ){
            int position = position_ptr - string;
            char *name = malloc( sizeof(char) * position );
            strncpy(name, string, (position+1));
            name[position + 1] = '\0';
            return name;
        }
    }

    return NULL;
}

char *strRemove(char *string, char *subString) {
    if ( subString != NULL ){
        size_t len = strlen(subString);
        if ( len > 0) {
            char *tmp = string;
            if ( (tmp = strstr(tmp,subString)) != NULL ){
                memmove(tmp, tmp + len, strlen( tmp +len) + 1);
            }
        }
        return string;
    }
    return NULL;
}


char *FileinString( char *filename ){
    //open the file and stock the content in buffer
    FILE *pf = openFile( filename );
    int length = lengthOfString(filename) + 2;
    char *buffer = malloc( sizeof(char) *  length);
    fread( buffer, sizeof(char), length, pf);
    closeFile(pf);
    //removes new lines and extra white spaces from the XML content
    strip(buffer);
    removeExtraWhiteSpaces(buffer);
    return buffer;
}


char *isolateContent( char *tag ){
    size_t len = strlen(tag) + 1;
    char *newTag = (char*)malloc(len);
    memmove(newTag,tag,len);
    newTag = strRemove( newTag, ">");
    newTag = strRemove( newTag, "<");

    return newTag;
}

char *extractContent( char *tag, char c ){
    if ( tag != NULL ){
        size_t len = strlen(tag);
        char *content = malloc( sizeof(char) * len );
        char *position_tag = strchr(tag, c);
        int position = position_tag - tag;
        strncpy(content, tag, position);
        return content;
    }
    return NULL;
}

bool checkEnd( char *tag ){
    if ( strstr(tag,"</") != NULL ){
        return true;
    }
    return false;
}

void removeChar( char *string, char c ){
    if (string != NULL && strchr(string,c) != NULL ){
        char *read = string, *write = string;
        while ( *read ){
            *write = *read++;
            write += (*write != c);
        }
        *write = '\0';
    }
}

char *extractUntil( char *string, char point ){
    if ( string != NULL ){
        size_t len = strlen(string);
        char *doctype = malloc( sizeof(char)*len);
        char *positionDoc = strchr(string,point);
        if ( positionDoc != NULL ){
            int position = positionDoc - string;
            strncpy(doctype,string,position+1);
            return doctype;
        }
    }
    return NULL;
}

void removeSpaceandTab(char *string){
    removeChar( string, 32 );
    removeChar( string, 11 );
}

bool checkCharacter( char *string, char character ){
    if ( string != NULL && strchr(string, character ) != NULL ){
        return true;
    }
    return false;
}

elementOccur checkOccurrence( char *string ){
    if ( checkCharacter(string, '?') ){
        return ONCE;
    }
    if ( checkCharacter(string, '*') ){
        return MULT;
    }
    if ( checkCharacter(string, '+') ){
        return PLUS;
    }
    return NONE;
}

void deleteOccurrence( char *string, elementOccur occurrence ){
    if ( occurrence == ONCE ){
        strRemove(string, "?");
    }
    if ( occurrence == MULT ){
        strRemove(string,"*");
    }
    if ( occurrence == PLUS ){
        strRemove(string,"+");
    }
}

bool checkOccurrenceXML( Node *xml, char *string, elementOccur occurrence){
    int count = 0;
    while (xml && xml->name){
        if ( strcmp(xml->name,string) == 0){
            count += 1;
        }
        if (xml->child != NULL ){
            xml = xml->child;
        } else{
            xml = xml->sibling;
        }
    }
    switch (occurrence) {
        case NONE:{
            if ( count == 1 ){
                return true;
            }
            break;
        }
        case ONCE:{
            if ( count == 0 || count == 1 ){
                return true;
            }
            break;
        }
        case MULT:{
            if ( count >= 0 ){
                return true;
            }
            break;
        }
        case PLUS:{
            if (count >= 1){
                return true;
            }
            break;
        }
    }
    return false;
}

bool checkElement(DTD *dtd, char *string){

    while (dtd){

        if ( dtd->name != NULL && string != NULL && strcmp(dtd->name,string) == 0 ){
            return true;
        }
        if ( dtd->child != NULL ){
            dtd = dtd->child;
        } else {
            dtd = dtd->sibling;
        }
    }
    return false;
}

bool checkXMLandDTD( DTD *dtd, Node *xml ){

    while ( xml && xml->name){
        if ( !checkElement(dtd,xml->name) ){
            return false;
        }
        if ( xml->child != NULL ){
            xml = xml->child;
        } else{
            xml = xml->sibling;
        }
    }

    return true;
}

bool checkDTDandXML( DTD *dtd, Node *xml ){

    while ( dtd && dtd->name ){
        if ( !checkOccurrenceXML(xml,dtd->name,dtd->occurrence) ){
            return false;
        }
        if (dtd->child != NULL ){
            dtd = dtd->child;
        } else{
            dtd = dtd->sibling;
        }
    }
    return true;
}

bool checkAttributesinXML( DTD *dtd, Node *xml ){

    while (dtd && dtd->name){
        if ( dtd->attributes != NULL ){
            if(!checkAttributes(dtd->name,dtd->attributes,xml)){
                return false;
            }
        }

        if ( dtd->child != NULL ){
            dtd = dtd->child;
        } else {
            dtd = dtd->sibling;
        }
    }
    return true;
}

bool checkAttributes( char *element, Attributes *attributes, Node *xml ){
    while ( xml && xml->name ){
        if ( strcmp(xml->name,element) == 0 ){
            if ( xml->attributes != NULL ){
                while ( attributes ){
                    if ( attributes->status == VALUE ){
                        if ( !checkValuesinXML(attributes->name,attributes->values,xml->attributes)){
                            return false;
                        }
                    } else if ( !checkStatusXML(attributes->status,attributes->name,xml->attributes)){
                        return false;
                    }
                    attributes = attributes->next;
                }
            } else{
                return false;
            }
        }
        if ( xml->child != NULL ){
            xml = xml->child;
        }else{
            xml = xml->sibling;
        }
    }
    return true;
}

bool checkValuesinXML( char *name, Values *value, xmlAttribute *attributes ){
    while ( attributes ){
        if ( strcmp(attributes->name,name) == 0 ){
            if(!checkValue( value, attributes->content )){
                return false;
            }
        }
        attributes = attributes->next;
    }
    return true;
}

bool checkValue( Values *value, char *name ){
    while ( value ){
        if ( strcmp(value->name,name) == 0 ){
            return true;
        }
        value = value->next;
    }
    return false;
}

bool checkAttributesinDTD( DTD *dtd, Node *xml ){

    while ( xml && xml->name ){
        if ( xml->attributes != NULL ){
            if ( !checkXMLAttributes( xml->name,xml->attributes,dtd) ){
                return false;
            }
        }
        if ( xml->child != NULL ){
            xml = xml->child;
        } else{
            xml = xml->sibling;
        }
    }
    return true;
}

bool checkXMLAttributes( char *element, xmlAttribute *attributes, DTD *dtd ){

    while ( dtd && dtd->name ){
        if ( strcmp(element, dtd->name) == 0 ){
            if ( dtd->attributes != NULL ){
                while ( attributes ){
                    if ( !checkifInDTD( attributes->name,dtd->attributes) ){
                        return false;
                    }
                    attributes = attributes->next;
                }
            }else{
                return false;
            }
        }
        if ( dtd->child != NULL ){
            dtd = dtd->child;
        } else {
            dtd = dtd->sibling;
        }
    }
    return true;
}

bool checkifInDTD( char *attribute, Attributes *dtdAttribute ){
    while ( dtdAttribute ){
        if ( strcmp(attribute,dtdAttribute->name) == 0 ){
            return true;
        }
        dtdAttribute = dtdAttribute->next;
    }
    return false;
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
