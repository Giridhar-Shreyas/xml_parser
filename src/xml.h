#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#define FALSE 0
#define TRUE 1


struct _XMLNode
{
    char* tag;
    char* inner_text;
    struct _XMLNode* parent;
};
typedef struct _XMLNode XMLNode;


struct _XMLDocument
{
    XMLNode* root;
};
typedef struct _XMLDocument XMLDocument;

int charactersInFile(FILE *file);
void extractFileName(char *fileName, char *commandLine);
void XML_Load(char *fileName, char* fileContent, XMLDocument *doc);

XMLNode *newXMLNode(XMLNode *parent);
void XMLNode_free(XMLNode *node);
char * strdup( const char *str1 );

