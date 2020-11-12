//
// Created by Namizata Sangaré on 08/11/2020.
//

#include "functions.h"

bool checkChild( DTD *dtd, char *tag, elementOccur occurrence ){
    while (dtd && dtd->name){
        if ( tag != NULL && strcmp(dtd->name,tag) == 0 ){
            dtd->occurrence = occurrence;
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

bool checkDTD( DTD *dtd, char *string ){
    char *tag = extractName(string);
    if ( strstr(tag,"]") != NULL ){
        return true;
    }

    if ( strstr(tag, "<!DOCTYPE") != NULL ){
        tag = extractUntil(tag,'[');
        return checkDTD(dtd,strRemove(string,tag));
    }

    if ( strstr(tag, "<!ELEMENT") != NULL && strstr(tag,"PCDATA") == NULL ) {
        strRemove(string, "<!ELEMENT ");
        strRemove(tag, "<!ELEMENT ");
        char *elementName = extractUntil(tag, ' ');
        strRemove(elementName," ");
        removeChar(elementName,11);
        if ( strcmp(elementName,dtd->name) == 0){
            strRemove(string,elementName);
            strRemove(tag,elementName);
            if (!checkCharacter(tag,',')){
                elementOccur occurrence = checkOccurrence(tag);
                char *newTag = extractContent(tag,')');
                strRemove(newTag,"(");
                deleteOccurrence(newTag,occurrence);
                strRemove(newTag," ");
                removeChar(newTag,11);
                if ( dtd->child != NULL ){
                    if (checkChild(dtd,newTag,occurrence) ){
                        return checkDTD(dtd,strRemove(string,tag));
                    }
                } else {
                    return false;
                }
            } else{
                char *newTag = extractContent(tag,')');
                strRemove(newTag,"(");
                removeChar(newTag,' ');
                removeChar(newTag,11);
                if ( checkDTDElement(dtd,newTag) ){
                    return checkDTD(dtd,strRemove(string,tag));
                }
            }

        } else{
            return false;
        }
    }
    if(strstr(tag, "<!ELEMENT") != NULL && strstr(tag,"PCDATA") != NULL){
        return checkDTD(dtd,strRemove(string,tag));
    }
    return false;
}

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
            dtd->sibling = malloc(sizeof(DTD));
            dtd->sibling = DTDinList(strRemove(string,tag),dtd->sibling);
        }
    }
    return dtd;
}

bool checkDTDElement( DTD *dtd, char *tag ){
    if (strlen(tag) == 0 ){
        return true;
    }
    if ( strchr(tag,',') == NULL ){
        char *namew = tag;
        char *namer = tag;
        elementOccur occurrence = checkOccurrence(namew);
        deleteOccurrence(namew,occurrence);
        if ( checkChild(dtd,namew,occurrence) ){
            return checkDTDElement(dtd,strRemove(tag,namer));
        }
    }
    char *namew = extractUntil(tag,',');
    char *namer = extractUntil(tag, ',');
    elementOccur occurrence = checkOccurrence(namew);
    deleteOccurrence(namew,occurrence);
    removeChar(namew,',');
    if ( checkChild(dtd,namew,occurrence) ){
        return checkDTDElement(dtd,strRemove(tag,namer));
    }

    return false;
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