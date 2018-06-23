#include "FileLoader.hpp"
namespace rat
{
    FileLoader::FileLoader()
    {
        std::fstream file("C:/Szczur/SzczurEngine/src/Szczur/Modules/QuestJournal/Steps.txt");
        std::string line;
        while(file.eof() == false)
        {
          
            getline(file,line);
            steps.push_back(line);
        }
        file.close();
        file.open("C:/Szczur/SzczurEngine/src/Szczur/Modules/QuestJournal/Descriptions.txt");
        while(file.eof() == false)
        {
            getline(file,line);
            descriptions.push_back(line);
        }

    }
    FileLoader::~FileLoader()
    {

    }

    std::string FileLoader::getStep(const unsigned int &i)
    {
        return steps[i];
    }

    std::string FileLoader::getDescription(const unsigned int &a,const unsigned int &b)
    {
        std::string text="";
        for(int i=a;i<=b;i++)
        {
            text+=descriptions[i] + "\n";
        }
        return text;
    }
}
