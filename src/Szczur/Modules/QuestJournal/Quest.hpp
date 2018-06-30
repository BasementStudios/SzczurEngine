#pragma once
#include <string>
#include <vector>
#include <memory>

#include "FileLoader.hpp"

namespace rat
{
    class Quest
    {
        public:
            Quest(std::shared_ptr<FileLoader> loader);
            ~Quest();
            void setQuestName(const unsigned int &i);

            void addQuestDescription(const unsigned int &descriptionNumber);

            void nextStep(const unsigned int &i);

            std::string getQuestName();

            std::vector<std::string> & getDescription();

            std::vector<std::string> & getSteps();

        private:
            std::string _questName;
            std::vector<std::string> _steps;
            std::vector<std::string> _descriptions;

            std::shared_ptr<FileLoader> _fileLoader;
    };
}