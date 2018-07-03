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
                    return std::string(*(k+(stepID+2)));
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

    std::string FileLoader::getDescription(unsigned int a)
    {
        unsigned int i=0;
        std::string text="";

        while(descriptions[i] != "_"+std::to_string(a))
        {
            i++;
        }
        i++;

        while(descriptions[i] != "---")
        {
            text+=descriptions[i] + "\n";
            i++;
        }
        return text;
    }
}
}
