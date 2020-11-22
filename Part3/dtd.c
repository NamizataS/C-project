//
// Created by Namizata Sangaré on 19/11/2020.
//

#include "functions.h"

DTD *DTDinList( DTD *dtd, char *string ){
    char *tag = extractName( string );
    if ( strstr( tag, "]>" ) != NULL ){
        return dtd;
    }

    if ( strstr( tag, "<!DOCTYPE" ) != NULL ){
        tag = extractUntil( tag, '[');
        return DTDinList( dtd, strRemove( string,tag ) );
    }

    if ( strstr(tag, "<!ELEMENT" ) != NULL ){
        strRemove( string, tag );
        strRemove( tag, "<!ELEMENT ");
        char *element = extractUntil( tag, ' ' );
        strRemove( tag, element );
        removeSpaceandTab( element );
        if ( dtd == NULL ){
            dtd = malloc(sizeof(DTD));
            dtd->name = element;
            if ( strstr(tag, "#PCDATA") != NULL ){
                dtd->type = PCDATA;
                return DTDinList( dtd, string);
            } else {
                dtd->type = ELEMENT;
                dtd->child = malloc( sizeof(DTD) );
                dtd->child->parent = dtd;
            }
            if ( strchr(tag, ',') != NULL ){
                char *child = extractUntil(tag,')');
                fillDTDRoot(dtd, child, element );
                return DTDinList( dtd, string );
            }
        } else {
            if ( searchThroughDTD( dtd,element ) ){
                char *child = extractUntil( tag, ')' );
                fillDTDRoot( dtd, child, element );
                return DTDinList(dtd,string);
            }
        }
    }

    if ( strstr(tag, "<!ATTLIST") != NULL ){
        return DTDinList( dtd, strRemove( string, tag ) );
    }
    return NULL;
}

DTD *newChild( char *name, elementOccur occurrence ){
    DTD *newDTD = malloc( sizeof(DTD) );
    newDTD->name = name;
    newDTD->occurrence = occurrence;

    return newDTD;
}

void fillDTDRoot( DTD *dtd, char *string, char *element ){
    removeChar(string,'(');
    removeChar(string,')');
    while ( dtd ){
        if ( dtd->parent != NULL ){
            if ( strcmp(element,dtd->parent->name) == 0 ){
                if ( strcmp(string, "#PCDATA") == 0 ){
                    dtd->parent->type = PCDATA;
                    dtd->parent->child = NULL;
                    break;
                } else {
                    fillSiblingDTD( dtd, string );
                }
            }
        }
        dtd = dtd->child;
    }
}

bool fillSiblingDTD( DTD *dtd, char *string ){
    char *child = extractUntil( string, ',' );
    if ( child == NULL ){
        child = string;
        elementOccur occurrence = checkOccurrence( child );
        deleteOccurrence( child, occurrence );
        if ( dtd->name == NULL ){
            dtd->name = child;
            dtd->occurrence = occurrence;
            dtd->child = malloc(sizeof(DTD));
            dtd->child->parent = dtd;
            return true;
        } else{
            dtdinRoot( dtd, newChild( child,occurrence) );
            return true;
        }
    } else{
        strRemove(string,child);
        removeChar(child, ',');
        removeSpaceandTab(child);
        elementOccur occurrence = checkOccurrence( child );
        deleteOccurrence( child, occurrence );
        if ( dtd->name == NULL ){
            dtd->name = child;
            dtd->occurrence = occurrence;
            dtd->child = malloc(sizeof(DTD));
            dtd->child->parent = dtd;
            return fillSiblingDTD( dtd, string );
        } else {
            dtdinRoot( dtd, newChild( child, occurrence ) );
            return fillSiblingDTD(dtd,string);
        }
    }
}

void dtdinRoot( DTD *dtd, DTD *insert ){
    DTD *current = dtd;
    while ( current->sibling != NULL ){
        current = current->sibling;
    }
    current->sibling = insert;
    current->sibling->child = malloc( sizeof(DTD) );
    current->sibling->child->parent = current->sibling;
}

bool searchThroughDTD( DTD *dtd, char *element ){
    while (dtd && dtd->name){
        if ( strcmp( dtd->name, element ) == 0 ){
            return true;
        }
        if ( searchThroughDTD( dtd->sibling,element) ){
            return true;
        } else {
            dtd = dtd->child;
        }
    }
    return false;
}

bool attributesInList( DTD *dtd, char *string ){

    char *tag = extractName(string);
    if ( strstr(tag,"]>") != NULL ){
        return true;
    }

    if ( strstr(tag,"<!DOCTYPE") != NULL ){
        tag = extractUntil(tag,'[');
        return attributesInList( dtd,strRemove(string,tag) );
    }

    if ( strstr(tag,"!ELEMENT") != NULL ){
        return attributesInList( dtd, strRemove(string,tag) );
    }

    if ( strstr( tag, "!ATTLIST" ) != NULL ){

        strRemove(string,"<!ATTLIST ");
        strRemove(tag, "<!ATTLIST ");
        strRemove(string,tag);
        char *element = extractUntil( tag, ' ' );
        strRemove(tag,element);
        char *name = extractUntil(tag, ' ' );
        strRemove(tag,name);
        char *type = extractUntil(tag,' ');
        if ( strchr(type, '|') == NULL ){
            strRemove(tag,type);
            char *status = extractUntil(tag, ' ' );
            if ( status == NULL ){
                status = tag;
                removeChar(status,'>');
                if ( attributeisValid( dtd, element,newAttribute(name,type,status) ) ){
                    return attributesInList( dtd, string );
                }
            } else{
                status = extractUntil(tag,11);
                strRemove(tag,status);
                if ( attributeisValid(dtd,element,newAttribute(name,type,status) ) ){
                    if ( fillAttribute(dtd,element,tag) ){
                        return attributesInList(dtd,string);
                    }
                }
            }
        } else {
            type = extractUntil(tag,')');
            Values *value = malloc( sizeof(Values));
            char *status = extractUntil(tag,' ' );
            if ( strlen(status) == 1 ){
                status = tag;
                removeChar(status,'>');
                if ( attributeisValid(dtd,element,newEnumeratedAttribute(name,getValues(type,value)))){
                    return attributesInList(dtd,string);
                }
            }
        }
    }

    return false;
}

bool attributeisValid( DTD *dtd, char *element, Attributes *newAttribute ){

    removeSpaceandTab(element);

    while ( dtd && dtd->name ){
        if ( strcmp(dtd->name,element) == 0 ){
            if ( dtd->attributes == NULL ){
                dtd->attributes = newAttribute;
                return true;
            }
            Attributes *current = dtd->attributes;
            while ( current->next != NULL ){
                current = current->next;
            }
            current->next = newAttribute;

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

Attributes *newAttribute( char *name, char *type, char *status ){

    Attributes *attribute = malloc(sizeof(Attributes));
    removeSpaceandTab(name);
    removeSpaceandTab(type);
    removeSpaceandTab(status);
    attribute->name = name;
    attribute->type = getType(type);
    attribute->status = getStatus(status);
    attribute->next = NULL;

    return attribute;
}

Attributes *newEnumeratedAttribute( char *name, Values *values ){
    struct Attributes *attribute = malloc(sizeof(Attributes));
    removeSpaceandTab( name );
    attribute->name = name;
    attribute->values = values;

    return attribute;
}

bool fillAttribute( DTD *dtd, char *element, char *string ){
    char *name = extractUntil( string, ' ' );
    strRemove(string,name);
    char *type = extractUntil( string, ' ' );
    if ( strchr(type,'|') == NULL ){
        strRemove(string,type);
        char *status = extractUntil( string, ' ' );
        if ( status == NULL ){
            status = string;
            removeChar(status,'>');
            if ( attributeisValid(dtd,element,newAttribute(name,type,status) ) ){
                return true;
            }
        } else {
            status = extractUntil(string,11);
            strRemove(string,status);
            if ( attributeisValid(dtd,element,newAttribute(name,type,status) ) ){
                return fillAttribute(dtd,element,string);
            }
        }
    } else {
        type = extractUntil(string, ')');
        strRemove(string,type);
        char *status = extractUntil( string, ' ' );
        Values *value = malloc(sizeof(Values));
        if ( strlen(status) == 1 ){
            status = string;
            removeChar(status,'>');
            if ( attributeisValid( dtd,element,newEnumeratedAttribute(name,getValues(type,value) ) ) ){
                return true;
            }
        } else {
            status = extractUntil(string,11);
            strRemove(string,status);
            if ( attributeisValid( dtd,element,newEnumeratedAttribute(name,getValues(type,value) ) ) ){
                return fillAttribute( dtd,element,string );
            }
        }
    }

    return false;
}

Values *getValues( char *values, Values *newValue ){

    if ( strlen(values) == 0 ){
        return newValue;
    }
    char *name = malloc(sizeof(char)*strlen(values));
    if ( strchr(values,'|') != NULL ){
        name = extractUntil(values,'|');
        strRemove(values,name);
        removeChar(name,'|');
        if ( strchr(name, '(') != NULL ){
            removeChar(name, '(' );
        }
    } else {
        name = extractUntil(values,')');
        strRemove(values,name);
        removeChar(name,')');
    }
    removeSpaceandTab(name);
    newValue->name = name;
    newValue->next = malloc(sizeof(Values));
    newValue->next = getValues(values,newValue->next);

    return newValue;
}

contentType getType( char *type ){

    if ( strcmp(type,"CDATA") == 0 ){
        return CDATA;
    }
    if ( strcmp( type, "ID") == 0 ){
        return ID;
    }
    return 5;
}

status getStatus( char *status ){
    if ( strcmp(status,"#REQUIRED") == 0 ){
        return REQUIRED;
    }
    if ( strcmp( status,"#IMPLIED") == 0 ){
        return IMPLIED;
    }
    if ( strstr(status, "#FIXED") != NULL ){
        return FIXED;
    }
    return 3;
}

void freeDTD( DTD *dtd ){
    if (dtd->child != NULL ){
        freeDTD(dtd->child);
    }
    if (dtd->sibling != NULL ){
        freeDTD( dtd->sibling);
    }
    free(dtd);
}
