#include <stdio.h>
#include <map>
#include <vector>
#include <iostream>
#include <fstream>  
#include <cctype>
#include <queue>

class xml
{
private:

    struct XMLNode
    {
        std::string tag;
        std::map<std::string, std::string> attributes;
        std::vector<XMLNode*> children;
        std::string innerText = "";
        bool hasInnerText = false;
    };

    std::ifstream doc;
    bool rootSet = false;
    bool unexpectedNl = false;
    char expectedChar;
    XMLNode *root=nullptr;
    XMLNode *currNode;
    std::queue<XMLNode*> currParent;
    
    bool loadFile();
    void parse();
    void parseRoot();

   
public:
    xml();
    ~xml();
};


