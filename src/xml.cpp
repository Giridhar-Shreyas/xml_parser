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
        
        
        for (int i = 0; i < line.length(); i++)
        {   

            if (line[i] ==' ')
            {
                continue;
            }
            if ((line[i] != ' ') && line[i] != '<')
            {
                if (currParent.empty())
                {
                    std::cout << "parse error text before tag\n";
                    return;
                }
                currParent.front()->hasInnerText = true;

                //check if it has closing tag </...>
                int check = line.find('<', i);
                if (check != std::string::npos)
                {
                    // sanity check
                    if(check+1 < line.length()){
                        if(line[check+1] == '/'){
                            while (line[i] != '<')
                            {
                                currParent.front()->innerText += line[i];
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
            
            
           
            if ((line[i] == '<'))
            {   
                // check if closing tag exists
                int check = line.find('>', i);
                if (check == std::string::npos){
                    std::cout << "No closing tag found in line: \n";
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
                    if (((line[i+1] != '/') && (!currParent.empty()) && (!currParent.front()->hasInnerText)) || (line[i+1] != '/') && (!rootSet))
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
                    else if ((line[i+1] != '/') && (!currParent.empty()) && (currParent.front()->hasInnerText))
                    {   
                        std::cout << "parsing error\n";
                        return;
                    }

                

                    // end of tag started
                    if ((line[i+1] == '/')){

                        // sanity check 
                        if (i+2 < line.length())
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
                        if (currParent.front()->tag == stuff)
                        {
                            XMLNode* child = currParent.front();
                            currParent.pop();
                            currParent.front()->children.push_back(child);
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
            if (line[i] == '>')
            {
                int check = line.find('<', i);
                
                if (check != std::string::npos){
                    if (!currParent.empty())
                    {
                        bool run = true;
                        while (run)
                        {
                            currParent.front()->innerText += line[i];
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
                        
                        
                    }
                    
                }
                // then it must exist in the next line
                
            }

            
        }
        lineNum++;
    }
    return;
}

void xml::parseRoot(){
    std::cout << root->tag << " " << root->innerText <<"\n";
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