#include "xml.hpp"




void xml::atributesInner(const std::string line, int& i, bool& success){
    /*
    already checked that there exists a closing tag '>'
    currently line[i] == ' ' after reading the tag
    now we need to read the attributes 
    checks:
        the attribute must have '='
        followed by " or '
            similarly must have a closing " or '
        
     */
    int check = line.find('=', i);
    char opening = ' ';
    if(check == std::string::npos){
        success = false;
        return;
    }
    // else continue
    else{
        int check1 = line.find('\'', i);
        int check2 = line.find('"', i);

        if(check1 == std::string::npos && check2 == std::string::npos){
            success = false;
            return;
        }
        // the = is outside of "" or ''
        else if (check1 != std::string::npos && check2 == std::string::npos)
        {
            if (check > check1)
            {
                success = false;
                return;
            }
            else{
                opening = '\'';
            }
            
        }
        else if (check1 == std::string::npos && check2 != std::string::npos){
            if (check > check2)
            {
                success = false;
                return;
            }
            else{
                opening = '"';
            }

        }
        else{
            int checkmax = std::max(check1, check2);
            if (check > checkmax)
            {
                success = false;
                return;
            }
            if (checkmax == check1)
            {
                opening = '\'';
            }
            else{
                opening = '"';
            }
        }
    }

    //move the pointer to after the space

    i+=1;
    std::string key = "";

    bool run = true;
    while (run)
    {
        key += line[i];

        if (i+1 >= line.length())
        {
            std::cout << "parse error\n";
            return;
        }
        
        i++;
        if(line[i] == '='){
            run = false;
        }
    }
    

    bool done = false;
    bool outOfString = false;
    
    if ((i+1 < line.length()))
    {
        if (line[i+1] =='"')
        {
           check = line.find('"', i);
            if (check != std::string::npos){
                // all is good read the value for key = key
                i++;
                i++;
                std::string value = "";
                bool run = true;
                while (run)
                {
                    value += line[i];

                    if (i+1 >= line.length())
                    {
                        std::cout << "parse error\n";
                        return;
                    }
                    
                    i++;
                    if(line[i] == '"'){
                        run = false;
                    }
                }
                currAttributes.push_back(std::make_tuple(key, value));
                    
                if (i+1 < line.length())
                {
                    i++;
                    if (line[i] == ' ')
                    {
                        if (i+1 < line.length())
                        {
                            i++;
                            atributesInner(line, i, success);
                        }
                        else{
                            success = false;
                            return;
                        }
                        
                    }
                    else{
                        // we are done no more attributes
                        success = true;
                        return;
                    }
                       
                    
                }
                else{
                    success = false;
                    return;
                }

            }
            else{
                success = false;
                return;
            }

        }
        else if (line[i+1] == '\'')
        {
            check = line.find('\'', i);
            if (check != std::string::npos){
                // all is good read the value for key = key
                i++;
                i++;
                std::string value = "";
                bool run = true;
                while (run)
                {
                    value += line[i];

                    if (i+1 >= line.length())
                    {
                        std::cout << "parse error\n";
                        return;
                    }
                    
                    i++;
                    if(line[i] == '\''){
                        run = false;
                    }
                }
                currAttributes.push_back(std::make_tuple(key, value));
                    
                if (i+1 < line.length())
                {
                    i++;
                    if (line[i] == ' ')
                    {
                        if (i+1 < line.length())
                        {
                            i++;
                            atributesInner(line, i, success);
                        }
                        else{
                            success = false;
                            return;
                        }
                        
                    }
                    else{
                        // we are done no more attributes
                        success = true;
                        return;
                    }
                       
                    
                }
                else{
                    success = false;
                    return;
                }
                
                 
            }
            else{
                success = false;
                return;
            }

        }
        else{
            success = false;
            return;
        }
    
    }
    else
    {
        success = false;
        return;
    }
    
    

}






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
                        // move i to point after the '<'
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
                        

                        if (line[i] == ' ')
                        {
                            bool success = false;
                            atributesInner(line, i, success);
                        }
                        
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
                            if (!currAttributes.empty())
                            {
                                for (size_t i = 0; i < currAttributes.size(); i++)
                                {
                                    node->attributes.insert(std::pair<std::string, std::string>(std::get<0>(currAttributes[i]), std::get<1>(currAttributes[i])));
                                }
                                
                            }
                            
                            currParent.push(node);

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
                            }                // then it must exist in the next line

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
                        // skip over '>'
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

            int count = 0;
            if(!node ->attributes.empty())
            {
                auto start = node->attributes.begin();

                while(start != node->attributes.end())
                {
                    std::cout << "attribute tag: " << start->first  << "\n"; 
                    std::cout << "attribute value: " << start->second  << "\n"; 

                    start++;
                }
                    std::cout << "\n";
            }
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