//
// Created by Namizata Sangaré on 11/11/2020.
//

#include "functions.h"

DTD *DTDinList( char *string, DTD *dtd ){

    char *tag = extractName(string);
    if ( strstr(tag,"]>") != NULL ){
        return dtd;
    }

    if ( strstr(tag, "<!DOCTYPE") != NULL ){
        tag = extractUntil(tag,'[');
        return DTDinList(strRemove(string,tag),dtd);
    }

    if ( strstr(tag, "<!ELEMENT") != NULL ){
        strRemove(string,"<!ELEMENT ");
        strRemove(tag,"<!ELEMENT ");
        char *elementName = extractUntil(tag,' ');
        strRemove(elementName, " ");
        removeChar(elementName,11);
        if (dtd == NULL ){
            dtd = malloc(sizeof(DTD));
            dtd->name = elementName;
            dtd->type = ELEMENT;
            dtd->attributes = NULL;
            dtd->child = malloc(sizeof(DTD));
            dtd->child = DTDinList(strRemove(string,tag),dtd->child);
        } else{
            dtd->name = elementName;
            strRemove(string,elementName);
            strRemove(tag,elementName);
            if (strstr(tag,"#PCDATA") != NULL ){
                dtd->type = PCDATA;
            } else{
                dtd->type = ELEMENT;
            }
            dtd->attributes = NULL;
            dtd->sibling = malloc(sizeof(DTD));
            dtd->sibling = DTDinList(strRemove(string,tag),dtd->sibling);
        }
    } else{
        return DTDinList(strRemove(string,tag),dtd);
    }

    return dtd;
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

bool updateStatus( char *status, Values *values ){

    while ( strchr(status,34) != NULL ){
        removeChar(status,34);
    }
    removeSpaceandTab(status);
    while ( values ){
        if ( strcmp(status,values->name) == 0 ){
            values->isDefault = true;
            return true;
        }
        values = values->next;
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

Values *newValues( char *name ){
    Values *newValue = malloc( sizeof(Values) );
    newValue->name = name;
    newValue->next = NULL;

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
