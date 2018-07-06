#include "QuestLog.hpp"

#include <vector>
#include <fstream>

#include "Szczur/Utility/Logger.hpp"

//#define CALL Widget::CallbackType::onRelease

using namespace nlohmann;

namespace rat
{
    QuestLog::QuestLog()
    {
        LOG_INFO("QuestLog module initializing...");
        init();
        LOG_INFO("QuestLog module initialized");
    }
    void QuestLog::init()
    {
        //_debugCounterTest();

        initScript();
        testQuest();

    }


    void QuestLog::update(float dt)
    {
        timer += dt;
        //std::cout << timer << '\n';

        if(timer > 2.f)
        {
            timer = 0.f;
            toStep = true;
        }
        if(toStep)
        {
            /*
            auto* q = getQuest("Nemo");
            switch(count)
            {
                case 0: activateQuest("Nemo"); break;
                case 1: q->getNode("SharkChoice")->nextStep("Kill"); break;
                case 2: q->getNode("Kill")->nextStep(); break;
                case 3: q->getNode("FindSth")->nextStep(); break;
                default: break;
            }
            //std::cout << "count: " << count << '\n';
            */
            count++;
            toStep = false;
        }
    }
    void QuestLog::render()
    {
        
    }
    
    Quest* QuestLog::addQuest(std::string name)
    {
        bool needDefaultName = false;
        if(name == "") needDefaultName = true;
        if(getQuest(name) != nullptr)
        {
            needDefaultName = true;
            LOG_INFO("Quest with name \"", name, "\" already exist, default name is set.");
        }

        if(needDefaultName)
        {
            name = "Q" + std::to_string(_quests.size());
        }

        auto quest = std::make_unique<Quest>(*this, name);

        auto* questPtr = quest.get();
        
        _quests.emplace(name, std::move(quest));

        return questPtr;
    }

    Quest* QuestLog::getQuest(const std::string name)
    {
        auto found = _quests.find(name);
        if(found == _quests.end())
        {
            //LOG_ERROR("Cannot get quest \"", name, "\", nullptr returned.");
            return nullptr;
        }

        return found->second.get();
    }

    void QuestLog::load(const std::string& path)
    {
        std::ifstream in(path.c_str());

        if(!in)
        {
            LOG_ERROR("Cannot open file \"", path, "\"...");
            return;
        }

        json j;
        in >> j;
        loadFromSaveJson(j);
    }
    void QuestLog::save(const std::string& path) const
    {
        std::ofstream out(path.c_str(), std::ios::trunc);

        if(!out)
        {
            LOG_ERROR("Cannot open file \"", path, "\"...");
            return;
        }

        out << std::setw(4) << getSaveJson();
    }

    json QuestLog::getSaveJson() const
    {
        json j;

        for(auto& [name, quest] : _quests)
        {
            j[name] = quest->getSaveJson();
        }

        return j;
    }
    void QuestLog::loadFromSaveJson(nlohmann::json& j)
    {
        LOG_INFO("Loading QuestLog...");
        for(auto i = j.begin(); i != j.end(); i++)
        {
            const std::string name = i.key();
            auto quest = getQuest(name);
            if(!quest)
            {
                LOG_ERROR("Quest \"", name, "\" hasn't been created. Maybe it has been removed...?");
                continue;
            }
            quest->loadFromSaveJson(i.value());
        }
        LOG_INFO("Loaded QuestLog.");
    }

    Requirements& QuestLog::getReqs()
    {
        return _reqs;
    } 

    void QuestLog::initScript() 
    {
        
        auto& script = getModule<Script>();

        auto module = script.newModule("QuestLog");

        
        // Main
        module.set_function("load", &QuestLog::load, this);
        
        module.set_function("save", &QuestLog::save, this);
        module.set_function("addQuest", &QuestLog::addQuest, this);
        module.set_function("getQuest", &QuestLog::getQuest, this);
        module.set_function("startQuest", &QuestLog::activateQuest, this);
        module.set_function("getReqs", &QuestLog::getReqs, this);

        script.initClasses<Requirements, Quest, QuestNode>();


        script.scriptFile("Assets/Quest/testowyQuest.lua");
        
    }

    void QuestLog::activateQuest(const std::string& name)
    {
        if(auto found = _quests.find(name); found == _quests.end())
        {
            LOG_ERROR("Cannot find quest \"", name, "\" to activate");
            assert(false);
        }
        else
        {
            auto& quest = found->second;
            auto& journal = getModule<QuestJournal>();
            //std::cout << "Quest with name index: " <<  << " activated\n\n";
            journal.addQuest(quest->getNameIndex());
            quest->start();

        }
    }

    void QuestLog::finishQuest(const std::string& name)
    {
        if(auto found = _quests.find(name); found == _quests.end())
        {
            LOG_ERROR("Cannot find quest \"", name, "\" to finish");
            assert(false);
        }
        else
        {
            auto& quest = found->second;
            auto& journal = getModule<QuestJournal>();
            //std::cout << "Quest with name index: " << quest->getNameIndex() << " finished\n\n";

            //quest->start();

            journal.finishQuest(quest->getNameIndex());
        }
    }

    void QuestLog::addQuestTitle(int questIndex, int index)
    {
        auto& journal = getModule<QuestJournal>();
        journal.moveIterator(questIndex);
        journal.addStep((unsigned int)(index));
        //std::cout << "Step: " << index << " from quest " << questName << " added\n\n";
    }
    void QuestLog::addQuestDescription(int questIndex, int index)
    {
        auto& journal = getModule<QuestJournal>();
        journal.moveIterator(questIndex);

        journal.addDescription((unsigned int)(index));

        //std::cout << "Desc: " << index << " from quest " << questName << " added\n\n";
    }

    void QuestLog::testQuest()
    {
        /*
        auto* nemo = addQuest("Nemo");
        nemo->setNameIndex(0);

        auto* root = nemo->getRoot();
        root->addDescription(0);

        auto* sharkChoice = root->addStep("SharkChoice");
        sharkChoice->addTitle(0);

        auto* kill = sharkChoice->addStep("Kill");
        kill->addTitle(4);
        kill->addDescription(1);

        auto* findSth = kill->addStep("FindSth");
        findSth->addTitle(1);
        findSth->addDescription(2);


        auto* goAround = sharkChoice->addStep("GoAround");
        goAround->addTitle(31);
        goAround->addDescription(31);

*/

    }
    
}

#undef CALL