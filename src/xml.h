#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#define FALSE 0
#define TRUE 1

//
//  Definitions
//
struct XMLNode
{
    char* tag;
    char* inner_text;
    XMLNode* parent;
};
typedef struct XMLNode;


struct XMLDocument
{
    XMLNode* root;
};
typedef struct XMLDocument;

int charactersInFile(FILE *file);
void XML_free(FILE *file);
void extractFileName(char *fileName, char *commandLine);
void XML_Load(char *fileName, FILE *XMLDocument);

XMLNode *newXMLNode(XMLNode *parent);
void XMLNode_free(XMLNode *parent);


