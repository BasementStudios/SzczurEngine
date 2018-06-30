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
        std::cout << timer << '\n';

        if(timer > 3.f)
        {
            timer = 0.f;
            toStep = true;
        }
        if(toStep)
        {
            auto* q = getQuest("Nemo");
            switch(count)
            {
                case 0: activateQuest("Nemo"); break;
                case 1: q->getNode("SharkChoice")->nextStep("Kill"); break;
                case 2: q->getNode("Kill")->nextStep(); break;
                case 3: q->getNode("FindSth")->nextStep(); break;
                default: break;
            }
            std::cout << "count: ", count, '\n'; 
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
        loadFromJson(j);
    }
    void QuestLog::save(const std::string& path) const
    {
        std::ofstream out(path.c_str(), std::ios::trunc);

        if(!out)
        {
            LOG_ERROR("Cannot open file \"", path, "\"...");
            return;
        }

        out << std::setw(4) << getJson();
    }

    json QuestLog::getJson() const
    {
        json j;

        for(auto& [name, quest] : _quests)
        {
            j[name] = quest->getJson();
        }

        return j;
    }
    void QuestLog::loadFromJson(nlohmann::json& j)
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
            quest->loadFromJson(i.value());
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
        module.set_function("getReqs", &QuestLog::getReqs, this);

        script.initClasses<Requirements, Quest, QuestNode>();


        //script.scriptFile("quests/testowyQuest.lua");
        
    }

    void QuestLog::activateQuest(const std::string& name)
    {
        if(auto found = _quests.find(name); found == _quests.end())
        {
            LOG_ERROR("Cannot activate quest \"", name, "\"");
            assert(false);
        }
        else
        {
            auto& quest = found->second;
            quest->start();

            auto journal = getModule<QuestJournal>();
            journal.addQuest(1);
        }
    }

    void QuestLog::addQuestTitle(const std::string& questName, int index)
    {
        std::cout << "jeb\n";
        auto journal = getModule<QuestJournal>();
        journal.moveIterator(questName);

        journal.addStep((unsigned int)(index));
    }
    void QuestLog::addQuestDescription(const std::string& questName, int index)
    {
        std::cout << "jebDesc\n";
        auto journal = getModule<QuestJournal>();
        journal.moveIterator(questName);
        std::cout << "jebDesc\n";

        journal.addDescription((unsigned int)(index));
        std::cout << "jebDesc\n";

    }

    void QuestLog::testQuest()
    {
        auto* nemo =  addQuest("Nemo");

        auto* root = nemo->getRoot();
        root->addDescription(5);

        auto* sharkChoice = root->addStep("SharkChoice");
        sharkChoice->addTitle(0);

        auto* kill = sharkChoice->addStep("Kill");
        kill->addTitle(4);
        kill->addDescription(6);

        auto* findSth = kill->addStep("FindSth");
        findSth->addTitle(2);
        findSth->addDescription(7);


        auto* goAround = sharkChoice->addStep("GoAround");
        goAround->addTitle(3);
        goAround->addDescription(8);



    }
    
}

#undef CALL