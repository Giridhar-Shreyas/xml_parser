#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xml.h"
#define FALSE 0
#define TRUE 1

int charactersInFile(FILE *file){
    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    fseek(file, 0, SEEK_SET);
    return size;
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

void XML_Load(char *fileName, char *fileContent, XMLDocument *doc){
    FILE *file;
    file = fopen(fileName, "r");
    if(file != NULL){
        fprintf(stdout, "The file has been opened.\n");
        int size = charactersInFile(file);
        fileContent = realloc(fileContent, (size)*sizeof(char));
        fread(fileContent, size, sizeof(char), file);
        fileContent[size]= '\0';
        fclose(file);

        doc->root = newXMLNode(NULL);

        XMLNode* curr_node = NULL;

        char lex[256];
        int lexi = 0;
        int i = 0;
        while(fileContent[i] != '\0'){
            if(fileContent[i] == '<'){
                lex[lexi] = '\0';
                if(lexi > 0 ){
                    if(!curr_node){
                        fprintf(stderr, "Text outside document");
                        break;
                    }
                    curr_node->inner_text = strdup(lex);
                    lexi = 0;
                }
                
                if(fileContent[i+1] == '/'){
                    i += 2;
                    while(fileContent[i] != '>'){
                        lex[lexi++] = fileContent[i++];
                    }
                    lex[lexi] = '\0';

                    if(!curr_node){
                        fprintf(stderr, "Already at the root");
                        break;
                    }
                    if(strcmp(curr_node->tag, lex)){
                        fprintf(stderr, "Missmatched tags (%s != %s)", curr_node->tag, lex);
                        break;
                        
                    }
                    curr_node = curr_node->parent;
                    i++;
                    continue;
                }
                if(!curr_node){
                    curr_node = doc->root;
                }
                else{
                    curr_node = newXMLNode(curr_node);
                }
                i++;
                while(fileContent[i] != '>'){
                    lex[lexi++] = fileContent[i++];
                }
                lex[lexi] = '\0';
                curr_node->tag = strdup(lex);

                lexi = 0;
                i++;
                continue;
            }
            else{
                lex[lexi++] = fileContent[i++];
            }
        }
    }
    else{
        fprintf(stderr, "The file could not be opened\n");
    }
}

XMLNode *newXMLNode(XMLNode *parent){
    XMLNode* node = malloc(sizeof(XMLNode));
    node->tag = NULL;
    node->inner_text = NULL;
    node->parent = parent;
    return node;
}
void XMLNode_free(XMLNode *node){
    if(node->tag != NULL){
        free(node->tag);
    }
    if(node->inner_text != NULL){
        free(node->inner_text);
    }
}