//
// Created by Namizata Sangaré on 11/11/2020.
//

#ifndef PART2_FUNCTIONS_H
#define PART2_FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <assert.h>

enum contentType {
    PCDATA = 1,
    ELEMENT = 2,
    CDATA = 3,
    ID = 4
};

enum elementOccur{
    NONE = 0,   //aucun quantifieur
    ONCE = 1,   // quantifieur ? (0 ou 1)
    MULT = 2,   // quantifieur * (0 ou plus)
    PLUS = 3    //quantifieur + (1 ou plus)
};

enum status{
    REQUIRED = 0,
    IMPLIED = 1,
    FIXED = 2
};

typedef enum contentType contentType;
typedef enum elementOccur elementOccur;
typedef enum status status;

struct Node {                   //structure pour représenter un noeud
    char *name;
    struct Node *child;
    struct Node *sibling;
    char *content;
};
typedef struct Node Node;

struct Values{
    char *name;
    bool isDefault;
    struct Values *next;
};

typedef struct Values Values;

struct Attributes{
    char *name;
    status status;
    Values *values;
    contentType type;
    struct Attributes *next;
};
typedef struct Attributes Attributes;

struct DTD {
    char *name;
    struct DTD *child;
    struct DTD *sibling;
    contentType type;
    elementOccur occurrence;
    Attributes *attributes;
};

typedef struct DTD DTD;


//FUNCTIONS.C
FILE *openFile( char *filename );
/*
 * function to open a file
 */

void closeFile ( FILE *pf);
/*
 * function to close a file
 */

int lengthOfString( char *filename );
/*
 * count how many character there is in the file
 */

void strip( char *string );
/*
 * delete all the new lines and tab from a string
 */

void removeExtraWhiteSpaces( char *string );
/*
 * remove the extra white spaces and leaves only one when needed
 */

char *extractName( char *string );
/*
 * function to extract a tag with the bracket
 */

char *strRemove(char *string, char *subString);
/*
 * remove a substring from a string
 */


char *FileinString( char *filename );
/*
 * open the XML file and put the content in a string
 */

char *isolateContent( char *tag );
/*
 * delete the brackets
 */

char *extractContent( char *tag, char c );
/*
 * extract the content without the closing tag
 */

bool checkEnd( char *tag );
/*
 * check if the tag is a closing one
 */

void removeChar( char *string, char c );
/*
 * remove a given char from the string
 */

char *extractUntil( char *string, char point );
/*
 * extract a part of a string until a certain character
 */

void removeSpaceandTab(char *string);
/*
 * function to remove VT and space
 */

void printDTD( DTD *dtd);


//DTD.C

DTD *DTDinList( char *string, DTD *dtd );
/*
 * put the infos of the DTD in a linked list
 */

bool attributesInList( DTD *dtd, char *string );
/*
 * to add the attributes in the list. Return true if it was successful and false if it isn't
 */

bool insertAttribute( DTD *dtd, char *element, char *name, char *type, char *status);
/*
 * check if the element is really in the dtd and insert the attribute and return true if it is
 * function used when it isn't an enumerated attribute
 */

bool insertEnumeratedAttribute( DTD *dtd, char *element, char *name, char *values, char *status);
/*
 * check if the element is in the dtd and insert the attribute and return true
 * function used when it is an enumerated attribute
 */

contentType getType( char *type );
/*
 * transform the type from char to the enum type created earlier
 */

status getStatus( char *status );
/*
 * transform the status from char to the the enum type created earlier
 */

void freeDTD( DTD *dtd );
/*
 * to free the allocated memory
 */


#endif //PART2_FUNCTIONS_H
