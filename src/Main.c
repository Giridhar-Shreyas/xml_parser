#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xml.h"

int main(void){
    //  Initialization
    char *fileName;
    fileName = calloc(1, sizeof(char));
    char commandLine[1000];
    char *fileContent;
    fileContent = calloc(1, sizeof(char));
    XMLDocument *doc = malloc(sizeof(XMLDocument));
    


    //  Get commandline promt
    fprintf(stdout, "Enter the file name\n");
    fgets(commandLine, sizeof(commandLine), stdin);

    //  Loading the file
    extractFileName(fileName, commandLine);
    XML_Load(fileName, fileContent, doc);
    //printf("The root tag: %s\n", doc->root->tag);

    return 0;
}