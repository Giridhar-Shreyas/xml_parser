#include "xml.hpp"

int lineNum;



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

    //i+=1;
    if(line[i] == ' '){
        i++;
    }
    std::string key = "";

    bool run = true;
    while (run)
    {
        key += line[i];

        if (i+1 >= line.length())
        {
            std::cerr << "parse error while reading attributes at line " << lineNum << "\n";
            exit(-1);
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
                if (!rootSet)
                {
                    i++;
                }
                
                while (line[i] == ' ')
                {
                    i++;
                }
                
                std::string value = "";
                bool run = true;
                while (run)
                {
                    value += line[i];

                    if (i+1 >= line.length())
                    {
                        std::cerr << "parse error while reading attributes at line " << lineNum << "\n";
                        exit(-1);
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
                if(!rootSet){
                    i++;
                }
                while (line[i] == ' ')
                {
                    i++;
                }
                std::string value = "";
                bool run = true;
                while (run)
                {
                    value += line[i];

                    if (i+1 >= line.length())
                    {
                        std::cerr << "parse error while reading attributes at line " << lineNum << "\n";
                        exit(-1);
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


bool isOnlyWhitespace(const std::string& str) {
    return std::all_of(str.begin(), str.end(), [](unsigned char c) {
        return std::isspace(c);
    });
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
    lineNum = 0;

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
                    std::cerr << "parse error text before tag at line" << lineNum << "\n";
                    exit(-1);
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
                                    std::cerr << "parsing error at line " << lineNum << "\n";
                                    exit(-1);
                                }
                                i++;
                                
                            }
                            if (isOnlyWhitespace(currParent.top()->innerText))
                            {
                                currParent.top()->hasInnerText = false;
                                currParent.top()->innerText = "";
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
                    std::cerr << "No closing tag found in line: " << lineNum << "\n";
                    exit(-1);
                }
                
                

                if (currParent.empty() && !rootSet && lineNum!=0)
                {   
                    std::cerr << "multiple roots parsing error at line " << lineNum << "\n";
                    exit(-1);
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
                                std::cerr << "parse error while reading opening tag at line" << lineNum << "\n";
                                exit(-1);
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
                        std::cerr << "parsing error at line " << lineNum << "\n";
                        exit(-1);
                    }

                

                    // end of tag started
                    if ((line[i+1] == '/')){

                        // sanity check 
                        if (i+2 > line.length())
                        {
                            std::cerr << "Parsing error while reading closing tag at line number " << lineNum << "\n";
                            exit(-1);
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
                                std::cerr << "parse error while reading closing tag at line number" << lineNum << "\n";
                                exit(-1);
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
                            std::cerr << "opening and closing tags do not match for opening tag = \"" << currParent.top()->tag << "\" and closing tag \"" << stuff << "\" at line " << lineNum <<"\n";
                            exit(-1);
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
                currAttributes.clear();
                
                if (check != std::string::npos){
                    if (!currParent.empty())
                    {
                        // sanity check 
                        if (i+1 > line.length())
                        {
                            std::cout << "Parsing error\n";
                        }
                        // skip over '>'
                        if(line[i+1] != '<') { 
                            i++;
                            currParent.top()->hasInnerText = true;
                            bool run = true;
                            while (run)
                            {
                                currParent.top()->innerText += line[i];
                                if (i+1 >= line.length())
                                {   
                                    std::cerr << "parsing error at line " << lineNum << "\n";
                                    exit(-1);
                                }
                                i++;
                                
                                if (line[i] == '<')
                                {
                                    run = false;
                                }
                                
                            }
                            if (isOnlyWhitespace(currParent.top()->innerText))
                            {
                                currParent.top()->hasInnerText = false;
                                currParent.top()->innerText = "";
                            }
                        }
                        else{
                            i++;
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

    if(!currParent.empty()){
        std::cerr << "Closing tag did not have / for tag name " << currParent.top()->tag << " at line " << lineNum << " Or previous line" << "\n";
        std::cerr << "And/Or the opening and closing names might not match for tag name" << "\n";
        exit(-1);
    }

    return;
}



void xml::parseRoot(){

    std::queue<XMLNode *> nodes;
    std::vector<int> visited;
    nodes.push(root);

    while (!nodes.empty())
    {
        auto node = nodes.front();
        nodes.pop();

        if (std::find(visited.begin(), visited.end(), node->nodeId) == visited.end())
        {
            visited.push_back(node->nodeId);

            
            //std::cout << "tag name: " << node->tag << " \n";

            if (node->hasInnerText)
            {
                //std::cout << "inner text: " << node->innerText << " \n";
            }
            

            int count = 0;
            if(!node ->attributes.empty())
            {
                auto start = node->attributes.begin();

                while(start != node->attributes.end())
                {
                    //std::cout << "attribute tag: " << start->first  << "\n"; 
                    //std::cout << "attribute value: " << start->second  << "\n"; 

                    start++;
                }
                    //std::cout << "\n";
            }

            if (node->hasInnerText && !node->children.empty())
            {
                std::cerr << "The tag " << node->tag << " has both inner tag and text which is not allowed\n";
                exit(-1);
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

void xml::xmlInner(XMLNode * node, int depth, std::ofstream& file){
    for (size_t i = 0; i < depth; i++)
    {
        file   << '\t';
    }

    file   << "<" << node->tag;

    for (auto it = node->attributes.begin(); it != node->attributes.end(); it++)
    {
        file   << " " << it->first << "=\"" << it->second << "\"";
    }

    file   << ">";

    if (node->hasInnerText) {
        file   << node->innerText;
    } else {
        file   << "\n";
    }

    for (auto i = 0 ; i < node->children.size(); i++)
        {
            xmlInner(node->children[i], depth+1, file);
        }

    if (!node->hasInnerText && !node->children.empty()) {
        for (int i = 0; i < depth; ++i) file   << '\t';
    }
    file   << "</" << node->tag << ">\n";
}


void xml::writeToFile(std::string fileName){
    std::ofstream outFile(fileName);

    if (!outFile.is_open()) {
        std::cerr << "Failed to open file: " << fileName << "\n";
        exit(-1);
    }
    xmlInner(root, 0, outFile);

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
    xml::writeToFile("copy.xml");

    
}

xml::~xml()
{
    delete root;
}