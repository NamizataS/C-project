//
// Created by Namizata Sangaré on 16/11/2020.
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
        //printf("%s\n",string);
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
        if ( position_tag != NULL ){
            int position = position_tag - tag;
            strncpy(content, tag, position);
            return content;
        }

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
        char *pr = string, *pw = string;
        while ( *pr ){
            *pw = *pr++;
            pw += (*pw != c);
        }
        *pw = '\0';
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

void printXML( Node *xml ){
    printf("Parent: ");
    while (xml && xml->name){
        printf("%s\n",xml->name);
        if (xml->parent != NULL ){
            xml = xml->parent;
        } else {
            xml = xml->child;
        }
    }
}
