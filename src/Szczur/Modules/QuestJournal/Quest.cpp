#include "Quest.hpp"

namespace rat
{
namespace journal
{
    Quest::Quest(std::shared_ptr<FileLoader> loader,unsigned int questID)
    : _fileLoader(loader),
    _questID(questID)
    {

    }

    Quest::~Quest()
    {

    }

    void Quest::setQuestName(unsigned int i)
    {
        _questName = _fileLoader->getQuestName(i);
    }
    
    void Quest::addQuestDescription(unsigned int descriptionID)
    {
        _descriptions.push_back(_fileLoader->getDescription(_questID, descriptionID));
    }

    void Quest::nextStep(unsigned int stepID)
    {
        std::string step = _fileLoader->getStep(_questID,stepID);

        while(step[0]!= ' ')
        {
            step.erase(0,1);
        }
        while(step[0] == ' ')
        {
            step.erase(0,1);
        }   
        _steps.push_back(step);
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

    void Quest::setQuestName(const std::string& name)
    {
        _questName = name;
    }

    unsigned int Quest::getID()
    {
        return _questID;
    }

    void Quest::formatName()
    {
        while(_questName[0]!= ' ')
        {
            _questName.erase(0,1);
        }
        while(_questName[0] == ' ')
        {
            _questName.erase(0,1);
        }   
    }

    void Quest::addQuestSaveDescription(std::vector<std::string> ref)
    {
        _descriptions = ref;
    }

    void Quest::addSaveSteps(std::vector<std::string> ref)
    {
        _steps = ref;
    }
    void Quest::setID(unsigned int ID)
    {
        _questID = ID;
    }

    void Quest::setFollowing(bool n)
    {
        _isFollowing = n;
    }

    bool Quest::isFollowing()
    {
        return _isFollowing;
    }

    std::string Quest::getLastStep()
    {
        std::vector<std::string>::iterator it = _steps.end()-1;
        return *it;
    }
}
}