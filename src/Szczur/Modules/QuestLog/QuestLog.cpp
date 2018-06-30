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
        LOG_INFO(this, "QuestLog module initializing...");
        init();
        LOG_INFO(this, "QuestLog module initialized");
    }
    void QuestLog::init()
    {
        //_debugCounterTest();

        initScript();


    }


    void QuestLog::update(float dt)
    {
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
    
}

#undef CALL