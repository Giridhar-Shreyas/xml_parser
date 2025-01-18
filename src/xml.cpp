#include "xml.hpp"


bool xml::loadFile()
{
    std::string filePath;
    //std::cout << "Enter the file name: ";
    //std::cin >> filePath;
    doc.open("test.xml");

    // return val of 1 = file could not be found
    if (doc.fail())
    {
        return false;
    }
    return true;
}

void xml::parse()
{
    int lineNum = 0;

    while (!doc.eof())
    {
        std::string line;
        getline(doc, line);

        // ignore the first line: <?xml version="1.0" encoding="utf-8"?>
        if (line.length() >= 2)
        {
            if ((line[0] == '<') && (line[1] == '?'))
            {
                getline(doc, line);
            }
        }
        
        int i = 0;
        //for (int i = 0; i < line.length(); i++)
        while(i < line.length())
        {   

            if (line[i] ==' ')
            {
                i++;
            }
            else if ((line[i] != ' ') && line[i] != '<' && line[i] != '>')
            {
                if (currParent.empty())
                {
                    std::cout << "parse error text before tag\n";
                    return;
                }
                currParent.top()->hasInnerText = true;

                //check if it has closing tag </...>
                int check = line.find('<', i);
                if (check != std::string::npos)
                {
                    // sanity check
                    if(check+1 < line.length()){
                        if(line[check+1] == '/'){
                            while (line[i] != '<')
                            {
                                currParent.top()->innerText += line[i];
                                if (i+1 >= line.length())
                                {
                                    std::cout << "huh? would i even print?\n";
                                    return;
                                }
                                i++;
                                
                            }
                        }
                    }
                }
                
            }
            
            
           
            else if ((line[i] == '<'))
            {   
                // check if closing tag exists
                int check = line.find('>', i);
                if (check == std::string::npos){
                    std::cout << "No closing tag found in line: " << lineNum << "\n";
                    return;
                }

                if (currParent.empty() && !rootSet && lineNum!=0)
                {   
                    std::cout << "multiple roots parsing error\n";
                    return;
                }
                

                // start of new tag
                // ignore the first line
                // sanity check
                if ((i+1 < line.length()))
                {
                    if (((line[i+1] != '/') && (!currParent.empty()) && (!currParent.top()->hasInnerText)) || (line[i+1] != '/') && (!rootSet))
                    {
                        // move i to point after the <
                        i++;
                        //start recording
                        std::string stuff = "";
                        bool run = true;
                        while (run)
                        {
                            stuff += line[i];

                            if (i+1 >= line.length())
                            {
                                std::cout << "parse error\n";
                                return;
                            }
                            
                            i++;
                            if((line[i] == '>' )|| (line[i] == ' ')){
                                run = false;
                            }
                        }

                        // tag ended and this node has no atributes
                        if (line[i] == '>')
                        {
                            XMLNode* node = new XMLNode;
                            node->tag = stuff;
                            node->nodeId = id;
                            id++;
                            if (!rootSet)
                            {   
                                this->root = node;
                                rootSet = true;
                            }
                            currParent.push(node);

                        }

                        if (line[i] == ' ')
                        {
                            // TODO
                        }  
                    }
                    else if ((line[i+1] != '/') && (!currParent.empty()) && (currParent.top()->hasInnerText))
                    {   
                        std::cout << "parsing error\n";
                        return;
                    }

                

                    // end of tag started
                    if ((line[i+1] == '/')){

                        // sanity check 
                        if (i+2 > line.length())
                        {
                            std::cout << "Parsing error\n";
                        }
                        
                        // move pointer to start read after </
                        i+=2;
                        //start recording
                        std::string stuff = "";
                        while ((line[i] != '>'))
                        {
                            stuff += line[i];
                            if (i+1 >= line.length())
                            {
                                std::cout << "parse error\n";
                                return;
                            }
                            i++;
                        }
                        
                        // tag that was opened has now been closed
                        // so we are done with that node
                        if (currParent.top()->tag == stuff)
                        {
                            
                            XMLNode* child = currParent.top();
                            
                            if(currParent.top() != root){
                                currParent.pop();
                                currParent.top()->children.push_back(child);
                            }
                            else{
                                currParent.pop();
                            }
                            
                            
                        }
                        else{
                            std::cout << "opening and closing tags do not match\n";
                            return;
                        }
                        

                    }
                }
                                
            }

            // only two cases either 
            // end of new tag
            // or start of inner text
            else if (line[i] == '>')
            {
                int check = line.find('<', i);
                
                if (check != std::string::npos){
                    if (!currParent.empty())
                    {
                        // sanity check 
                        if (i+1 > line.length())
                        {
                            std::cout << "Parsing error\n";
                        }
                        // skip over >
                        i++;
                        currParent.top()->hasInnerText = true;
                        bool run = true;
                        while (run)
                        {
                            currParent.top()->innerText += line[i];
                            if (i+1 >= line.length())
                            {   
                                std::cout << "oh no it prints\n";
                                return;
                            }
                            i++;
                            
                            if (line[i] == '<')
                            {
                                run = false;
                            }
                            
                        }
                        //weird hack
                        //i--;
                        
                    }
                    
                }
                else{
                    i++;
                }
                // then it must exist in the next line
                
            }
            else{
                i++;
            }

            
        }
        lineNum++;
    }
    return;
}

void xml::parseRoot(){

    std::stack<XMLNode *> nodes;
    std::vector<int> visited;
    nodes.push(root);

    while (!nodes.empty())
    {
        auto node = nodes.top();
        nodes.pop();

        if (std::find(visited.begin(), visited.end(), node->nodeId) == visited.end())
        {
            visited.push_back(node->nodeId);
            std::cout << "tag name: " << node->tag << " ";
            if (node->hasInnerText)
            {
                std::cout << "inner text: " << node->innerText << " ";
            }
            std::cout << "\n";
        }

        for (auto i = 0 ; i < node->children.size(); i++)
        {
            if (std::find(visited.begin(), visited.end(), node->children[i]->nodeId) == visited.end())
            {
                nodes.push(node->children[i]);
            }
        }
        
        

    }
    
}

xml::xml()
{
    bool success = xml::loadFile();
    if (!success)
    {
        std::cout << "The entered file does not exist\n"; 
        return;
    }
    xml::parse();
    xml::parseRoot();

    
    

    
}

xml::~xml()
{
}