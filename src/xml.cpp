#include "xml.hpp"


bool xml::loadFile()
{
    std::string filePath;
    std::cout << "Enter the file name: ";
    std::cin >> filePath;
    doc.open(filePath);

    // return val of 1 = file could not be found
    if (doc.fail())
    {
        return false;
    }
    return true;
}

void xml::parse()
{
    std::string line;
    while (getline(doc, line))
    {
    /*
    int realStart = 0;
        while (isspace(line[realStart]))
        {
            realStart++;
        }
        // there is some text outside of the tag identifiers 
        // TODO
        if (line[realStart] != '<')
        {
            std::cout << "Text out side of opening tag";
            return;
        }
         */
        for (int i = 0; i < line.length(); i++)
        {   
           
            if ((line[i] == '<'))
            {   
                // start of new tag
                // ignore the first line
                if ((line[i+1] != '?') && (line[i+1] != '/'))
                {
                    // move i to point after the <
                    i++;
                    //start recording
                    std::string stuff = "";
                    while ((line[i] != '>' )|| (line[i] != ' '))
                    {
                        stuff += line[i];

                        if (i+1 == line.length())
                        {
                            std::cout << "parse error\n";
                            return;
                        }
                        
                        i++;
                    }

                    // tag ended and this node has no atributes
                    if (line[i] == '>')
                    {
                        XMLNode node;
                        node.tag = stuff;
                        if (!rootSet)
                        {   
                            root = &node;
                            rootSet = true;
                        }
                        currParent.push(&node);

                    }

                    if (line[i] == ' ')
                    {
                        // TODO
                    }  
                }

                // end of tag started
                if ((line[i+1] == '/')){

                    // move pointer to start read after </
                    i+=2;
                    //start recording
                    std::string stuff = "";
                    while ((line[i] != '>' ))
                    {
                        stuff += line[i];
                        if (i+1 == line.length())
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
                    

                }
                                
            }

            // only two cases either 
            // start of new tag
            // or start of inner text
            if (line[i] == '>')
            {
                
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
}

xml::~xml()
{
}