#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xml.h"


int charactersInFile(FILE *file){
    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    fseek(file, 0, SEEK_SET);
    return size;
}

void XML_free(FILE *file){
    fclose(file);
}

void extractFileName(char *fileName, char *commandLine){
    int size = 0;
    while(commandLine[size] != 'l'){
        size++;
    }
    fileName = realloc(fileName, (size+1) * sizeof(char));
    int i = 0;
    while(i<=size){
        fileName[i] = commandLine[i];
        i++; 
    }
    fileName[i+1] = '\0';
}

void XML_Load(char *fileName, FILE *XMLDocument, char *fileContent){
    XMLDocument = fopen(fileName, "r");
    if(XMLDocument != NULL){
        fprintf(stdout, "The file has been opened.\n");
        int size = charactersInFile(XMLDocument);
        fileContent = realloc(fileContent, (size)*sizeof(char));
        fread(fileContent, size, sizeof(char), XMLDocument);
        fileContent[size]= '\0';
        fclose(XMLDocument);
    }
    else{
        fprintf(stderr, "The file could not be opened\n");
    }
}


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