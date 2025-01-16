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
        std::string innerText = "none";
    };

    std::ifstream doc;
    bool rootSet = false;
    XMLNode *root;
    XMLNode *currNode;
    std::queue<XMLNode*> currParent;
    
    bool loadFile();
    void parse();

   
public:
    xml();
    ~xml();
};


