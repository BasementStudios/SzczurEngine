#include "Quest.hpp"

namespace rat
{
    Quest::Quest(std::shared_ptr<FileLoader> loader)
    : _fileLoader(loader)
    {

    }

    Quest::~Quest()
    {

    }

    void Quest::setQuestName(const unsigned int &i)
    {
        _questName = _fileLoader->getStep(i);
    }
    
    void Quest::setQuestDescription(const unsigned int &a,const unsigned int &b)
    {
        _descriptions.push_back(_fileLoader->getDescription(a,b));
    }

    void Quest::nextStep(const unsigned int &i)
    {
        _steps.push_back(_fileLoader->getStep(i));
    }

    std::string Quest::getQuestName()
    {
        return _questName;
    }

    std::vector<std::string>& Quest::getDescription()
    {
        return _descriptions;
    }

    std::vector<std::string> & Quest::getSteps()
    {
        return _steps;
    }
}