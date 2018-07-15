#include "FileLoader.hpp"

namespace rat
{
namespace journal
{

    FileLoader::FileLoader()
    {
        std::fstream file("Assets/QuestJournal/Steps.txt");
        std::string line;
        while(file.eof() == false)
        {
          
            getline(file,line);
            steps.push_back(line);
        }
        file.close();
        file.open("Assets/QuestJournal/Descriptions.txt");
        while(file.eof() == false)
        {
            getline(file,line);
            descriptions.push_back(line);
        }

    }
    FileLoader::~FileLoader()
    {

    }

    std::string FileLoader::getStep(unsigned int questID, unsigned int stepID)
    {
        std::string look = "_" + std::to_string(questID);
        std::string base;
        int g;
        for(auto k = steps.begin(); k!= steps.end();k++)
        {
            base = "";
            if(k->operator[](0)=='_')
            {
                g = 0;
                while(k->operator[](g) != ' ')
                {
                    base+=k->operator[](g);
                    g++;
                }
                if(base == look)
                {
                    base = "";
                    std::string n = *(k+(stepID+2));
                    while(n.find(std::string("\\n")) != std::string::npos)
                    {

                        size_t pos = n.find("\\n");
                        for(size_t p = 0; p <pos;p++)
                        {
                            base+=n[0];
                            n.erase(0,1);
                        }
                        base += "\n";
                        n.erase(0,2);
                    }
                    if(base.empty()) 
                        return n;
                    else
                       return base+n;
                    
                }
            }
        }
        return " ";
    }

    std::string FileLoader::getQuestName(unsigned int questID)
    {
        std::string look = "_" + std::to_string(questID);
        std::string base;
        int g;
        for(auto k = steps.begin(); k!= steps.end();k++)
        {
            base = "";
            if(k->operator[](0)=='_')
            {
                g = 0;
                while(k->operator[](g) != ' ')
                {
                    base+=k->operator[](g);
                    g++;
                }
                if(base == look)
                {
                    return std::string(*k);
                }
            }
        }
        return " ";
    }

    std::string FileLoader::getDescription(unsigned int questID, unsigned int descriptionID)
    {
        unsigned int i=0;
        std::string text="";
        std::string base;
        int g;
        std::vector<std::string>::iterator k;
        for(k = descriptions.begin(); k!= descriptions.end(); k++)
        {
            if(k->operator[](0) == '_')
            {
                g=0;
                base = "";
                while(k->operator[](g) != ' ')
                {
                    base+=k->operator[](g);
                    g++;
                }
                if("_"+ std::to_string(questID) == base)
                {
                    break;
                }
            }
        }
        while(*k != std::to_string(descriptionID))
            k++;
        k++;
        while(*k!= "---")
        {
            text+=*k;
            //text += "\n";
            k++;
        }
        return text;
    }
}
}
