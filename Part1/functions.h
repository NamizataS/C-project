//
// Created by Namizata Sangaré on 20/10/2020.
//

#ifndef PART1_FUNCTIONS_H
#define PART1_FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <assert.h>


enum contentType {
    PCDATA = 1,
    ELEMENT = 2
};

enum elementOccur{
    NONE = 0,   //aucun quantifieur
    ONCE = 1,   // quantifieur ? (0 ou 1)
    MULT = 2,   // quantifieur * (0 ou plus)
    PLUS = 3    //quantifieur + (1 ou plus)
};

typedef enum contentType contentType;
typedef enum elementOccur elementOccur;

struct Node {                   //structure pour représenter un noeud
    char *name;
    struct Node *child;
    struct Node *sibling;
    char *content;
};
typedef struct Node Node;

struct DTD {
    char *name;
    struct DTD *child;
    struct DTD *sibling;
    contentType type;
    elementOccur occurrence;
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

bool checkCharacter( char *string, char character );
/*
 *
 */

elementOccur checkOccurrence( char *string );
/*
 * check the occurrence of each elements in the DTD
 */

void deleteOccurrence( char *string, elementOccur occurrence );
/*
 * remove the occurrence from the string
 */

bool checkElement(DTD *dtd, char *string);
/*
 * check if the element is in the dtd
 */

bool checkOccurrenceXML( Node *xml, char *string, elementOccur occurrence);
/*
 * check if there is the right amount of an element in the XML
 */

bool checkXMLandDTD( DTD *dtd, Node *xml );
/*
 * to check if the xml respect the structure of the dtd
 */

bool checkDTDandXML( DTD *dtd, Node *xml );
/*
 * to check if the occurrence are ok
 */

//TO DEBUG
void printXML( Node *xml );
void printDTD( DTD *dtd);

//XML.C

bool checkXML( char *string );
/*
 * check if the XML is valid
 */

Node *XMLinList( char *string, Node *xml );

/*
 * put the infos of the XML in a Node(linked list)
 */

void freeXML( Node *xml );
/*
 * to free the allocated memory
 */


//DTD.C

bool checkChild( DTD *dtd, char *tag, elementOccur occurrence );
/*
 * check if the string is in the DTD
 */

bool checkDTD( DTD *dtd, char *string );
/*
 * check if a DTD is valid
 */


DTD *DTDinList( char *string, DTD *dtd );
/*
 * put the infos of the DTD in a linked list
 */

bool checkDTDElement( DTD *dtd, char *tag );

/*
 * check if all the child are in the DTD
 */

void freeDTD( DTD *dtd );
/*
 * to free the allocated memory
 */


#endif //PART1_FUNCTIONS_H
