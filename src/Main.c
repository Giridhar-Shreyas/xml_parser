#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xml.h"

int main(void){
    //  Initialization
    char *fileName;
    fileName = calloc(1, sizeof(char));
    FILE *XMLDocument;
    char commandLine[1000];
    char *fileContent;
    fileContent = calloc(1, sizeof(char));

    //  Get commandline promt
    fprintf(stdout, "Enter the file name\n");
    fgets(commandLine, sizeof(commandLine), stdin);

    //  Loading the file
    extractFileName(fileName, commandLine);
    XML_Load(fileName, XMLDocument, fileContent);

    return 0;
}