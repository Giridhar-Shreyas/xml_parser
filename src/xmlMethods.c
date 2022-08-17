char *XML_Load(char *fileName, FILE *XMLDocument, char* commandLine){
    //commandLine is a placeholder for fileName(dynamically allocated)
    char commandLine[1000];
    fprintf(stdout, "Enter the name of the file\n");
    fgets(commandLine, sizeof(commandLine), stdin);

    int size = 0;
    char *fileName = calloc(size, sizeof(char));
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


    FILE *xmlFile;

    xmlFile = fopen(fileName, "r");
    if(xmlFile != NULL){
        fprintf(stdout, "The file has been opened.\n");
    }
    else{
        fprintf(stderr, "The file could not be opened\n");
    }