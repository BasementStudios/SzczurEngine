#include "Quest.hpp"

namespace rat
{
namespace journal
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
    
    void Quest::addQuestDescription(const unsigned int &descriptionNumber)
    {
        _descriptions.push_back(_fileLoader->getDescription(descriptionNumber));
    }

    void Quest::nextStep(const unsigned int &i)
    {
        _steps.push_back(_fileLoader->getStep(i));
    }

    std::string& Quest::getQuestName()
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
}