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

        ~XMLNode() {
        for (auto child : children)
            delete child;
        }
    };

    std::ifstream doc;
    bool rootSet = false;
    XMLNode *root=nullptr;
    int id = 0;



    bool loadFile();
    void parse();
    void parseRoot();
    void atributesInner(const std::string line, int& i, bool& success);

   
public:
    std::stack<XMLNode*> currParent;
    std::vector<std::tuple<std::string, std::string>> currAttributes;
    xml();
    ~xml();
};


