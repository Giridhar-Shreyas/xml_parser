#include <stdio.h>
#include <map>
#include <vector>
#include <iostream>
#include <fstream>  
#include <cctype>
#include <queue>
#include <stack>
#include <algorithm>

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
        int nodeId;
    };

    std::ifstream doc;
    bool rootSet = false;
    bool unexpectedNl = false;
    char expectedChar;
    XMLNode *root=nullptr;
    XMLNode *currNode;
    
    int id = 0;
    bool loadFile();
    void parse();
    void parseRoot();

   
public:
    std::stack<XMLNode*> currParent;
    xml();
    ~xml();
};


