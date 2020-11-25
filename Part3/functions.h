//
// Created by Namizata Sangaré on 16/11/2020.
//

#ifndef PART3_FUNCTIONS_H
#define PART3_FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
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
    VALUE = 2
};

typedef enum contentType contentType;
typedef enum elementOccur elementOccur;
typedef enum status status;

struct Node {
    char *name;
    struct xmlAttribute *attributes;
    struct Node *parent;
    struct Node *child;
    struct Node *olderSibling;
    struct Node *sibling;
    char *content;
};
typedef struct Node Node;

struct xmlAttribute{
    char *name;
    char *content;
    struct xmlAttribute *next;
};

typedef struct xmlAttribute xmlAttribute;

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
    struct DTD *parent;
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

char *removeSub( char *string, char *subString );

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

bool checkAttributesinXML( DTD *dtd, Node *xml );
/*
 * check if the attributes of the DTD are in the XML
 */

bool checkAttributes( char *element, Attributes *attributes, Node *xml );
/*
 * check the nature of the attributes and it's in the xml
 */

bool checkXMLandDTD( DTD *dtd, Node *xml );
/*
 * to check if the xml respect the structure of the dtd
 */

bool checkElement( DTD *dtd, Node *xml );
/*
 * check if the element is in the dtd
 */

bool checkSibling( char *string, DTD *dtd );
/*
 *
 */

bool checkDTDandXML( DTD *dtd, Node *xml );
/*
 * to check if the occurrence are ok
 */

bool checkOccurrenceXML( Node *xml, char *string, elementOccur occurrence);
/*
 * check if there is the right amount of an element in the XML
 */

bool checkAttributesinDTD( DTD *dtd, Node *xml );
/*
 * check if the attributes in the XML are really in the dtd
 */

bool checkXMLAttributes( char *element, xmlAttribute *attributes, DTD *dtd );
/*
 * used for the functions checkAttributesinDTD
 */

bool checkifInDTD( char *attribute, Attributes *dtdAttribute );
/*
 * check if the attribute is in the list of attributes of the DTD
 */


//XML.C
bool checkXML( char *string );
/*
 * check if there is an open or close tag for each element
 */

Node *XMLinList( Node *xml, char *string );

/*
 * put the infos of the XML in a Node(linked list)
 */

Node *newRoot( char *name, xmlAttribute *attributes );
/*
 * to create a new root
 */

Node *newLeaf( char *name, char *content, xmlAttribute *attributes );
/*
 * to create a new leaf
 */

Node *fillRoot( Node *xml, char *name, xmlAttribute *attributes );
/*
 * to fill a root
 */

void fillSibling( Node *sibling, Node *insert );
/*
 * to fill a leaf/sibling
 */

xmlAttribute *newXMLAttribute( char *name, char *content );
/*
 * create a new XML attribute
 */

bool fillXMLAttribute( xmlAttribute *attribute, char *string );
/*
 * fill the XML attributes if there is multiple attributes
 */

void attributeInXML( xmlAttribute *list, xmlAttribute *insert );
/*
 * put the attributes in the XML if it already as elements in it
 */

bool checkStatusXML( status status, char *string, xmlAttribute *attribute );
/*
 * check if the status of an attribute is ok
 */

void freeXML( Node *xml );
/*
 * to free the allocated memory
 */

//DTD.C

DTD *DTDinList( DTD *dtd, char *string );
/*
 * put the infos of the DTD in a linked list
 */

void fillDTDRoot( DTD *dtd, char *string, char *element );
/*
 *
 */

DTD *newChild( char *name, elementOccur occurrence );
/*
 * to create a new DTD child
 */

bool fillSiblingDTD( DTD *dtd, char *string );
/*
 *
 */

void dtdinRoot( DTD *dtd, DTD *insert );
/*
 * insert a new DTD after a sibling
 */

bool searchThroughDTD( DTD *dtd, char *element );
/*
 * search through the DTD to see if the element is already in the DTD
 */

bool attributesInList( DTD *dtd, char *string );
/*
 * to add the attributes in the list. Return true if it was successful and false if it isn't
 */

bool attributeisValid( DTD *dtd, char *element, Attributes *newAttribute );
/*
 * check if the element is really in the dtd and insert the attribute and return true if it is
 * function used when it isn't an enumerated attribute
 */

Attributes *newAttribute( char *name, char *type, char *status );
/*
 * create a new attribute
 */

Attributes *newEnumeratedAttribute( char *name, Values *values );
/*
 * create a new enumerated attribute
 */

bool fillAttribute( DTD *dtd, char *element, char *string );
/*
 * fill a new attribute if the attributes for the same element are in the same block
 */

Values *getValues( char *values, Values *newValues );
/*
 * create a new linked list values
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

#endif //PART3_FUNCTIONS_H