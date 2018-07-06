#pragma once
#include <string>
#include <vector>
#include <memory>

#include "FileLoader.hpp"

namespace rat
{
namespace journal
{
    class Quest
    {
        public:
            Quest(std::shared_ptr<FileLoader> loader, unsigned int questID);
            ~Quest();
            void setQuestName(unsigned int i);
            void setQuestName(const std::string& name);

            void addQuestDescription(unsigned int descriptionID);
            void addQuestSaveDescription(std::vector<std::string> vec);

            void nextStep(unsigned int stepID);
            void addSaveSteps(std::vector<std::string> vec);

            unsigned int getID();
            void setID(unsigned int ID);
            std::string& getQuestName();

            std::vector<std::string> & getDescription();

            std::vector<std::string> & getSteps();

            void formatName();

        private:

            std::string _questName;
            std::vector<std::string> _steps;
            std::vector<std::string> _descriptions;

            std::shared_ptr<FileLoader> _fileLoader;
            unsigned int _questID;
    };
}
}