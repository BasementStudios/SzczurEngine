#include "QuestLog.hpp"

#include <vector>
#include <fstream>

#include "Szczur/Utility/Logger.hpp"

#include "Szczur/Modules/GUI/ImageWidget.hpp"
#include "GUI/QuestInfoBar/QuestTitle.hpp"

#define CALL Widget::CallbackType::onRelease

using namespace nlohmann;

namespace rat
{
    QuestLog::QuestLog()
    {
        LOG_INFO(this, "QuestLog initing...");
        init();
        LOG_INFO(this, "QuestLog inited.");
    }
    void QuestLog::init()
    {
        //_debugCounterTest();

        initScript();

        _quest = std::make_unique<Quest>(*this, "Q0");
        auto* quest = _quest.get();
        auto* node = _quest->getRoot();
        _widget = getModule<GUI>().addInterface();
        _widget->setSize(40.f, 50.f);
        _widget->setPosition(100, 100);

        auto* saveButton = new Widget;
        _widget->add(saveButton);
        saveButton->setCallback(CALL, [=](auto){
            save("quests/quests.json");
        });
        saveButton->setSize(100, 100);
        saveButton->setPosition(900.f, 0.f);

        auto loadButton = new Widget;
        _widget->add(loadButton);
        loadButton->setSize(100, 100);
        loadButton->setPosition(1000, 0);
        loadButton->setCallback(CALL, [=](auto){
            load("quests/quests.json");
        });
        
        

        auto& gui = getModule<GUI>();
        gui.addAsset<sf::Texture>("Assets/Quest/Left.png");
        gui.addAsset<sf::Texture>("Assets/Quest/LeftOpen.png");
        gui.addAsset<sf::Texture>("Assets/Quest/Right.png");
        gui.addAsset<sf::Texture>("Assets/Quest/RightOpen.png");
        gui.addAsset<sf::Texture>("Assets/Quest/Shield.png");
        gui.addAsset<sf::Texture>("Assets/Quest/Broken Shield.png");
        gui.addAsset<sf::Texture>("Assets/Quest/Ring.png");
        gui.addAsset<sf::Texture>("Assets/Quest/Sword.png");
        gui.addAsset<sf::Texture>("Assets/Quest/None.png");
        gui.addAsset<sf::Texture>("Assets/Quest/BetterShield.png");
        gui.addAsset<sf::Texture>("Assets/Quest/Exit.png");
        gui.addAsset<sf::Texture>("Assets/Quest/DeadKnight.png");
        gui.addAsset<sf::Texture>("Assets/Quest/Knight.png");
        gui.addAsset<sf::Texture>("Assets/Quest/Wizard.png");
        gui.addAsset<sf::Texture>("Assets/Quest/DeadWizard.png");
        gui.addAsset<sf::Texture>("Assets/Quest/Pied.png");
        gui.addAsset<sf::Font>("Assets/Fonts/Base.ttf");

        _title = std::make_unique<QuestGUI>();
        _title->setParent(_widget);
        _title->setFont(gui.getAsset<sf::Font>("Assets/Fonts/Base.ttf"));

        
        std::vector<ImageWidget*> w;

        for(int i = 0; i < 3; i++)
        {
            w.emplace_back(new ImageWidget);
            _widget->add(w[i]);
            w[i]->setSize(100, 100);
            w[i]->setPosition(100 + i *200, 100);
            
        }
        
        /*
        auto* choice = node->addBrancher("Doors");
        choice->setTitle("Wejdz do ktorychs z drzwi");
        //auto* n1 = choice->addStep("Left");
        auto* tarcze = choice->addStep("Shields");
        auto* n2 = choice->addStep("Right");
        choice->_onActivate = [=, &gui](){
            w[0]->setTexture(gui.getAsset<sf::Texture>("Assets/Quest/LeftOpen.png"));
            w[0]->setCallback(CALL, [=](Widget*){
                choice->nextStep(0);
            });
            w[1]->setTexture(gui.getAsset<sf::Texture>("Assets/Quest/RightOpen.png"));
            w[1]->setCallback(CALL, [=](Widget*){
                choice->nextStep(1);  
            });
        };

 

        n2->_onActivate = [n2](){
            n2->nextStep();
        };
        n2->setBlockedCallback([w, &gui](){
            w[1]->setCallback(CALL, [](Widget*){});
            w[1]->setTexture(gui.getAsset<sf::Texture>("Assets/Quest/None.png"));
            std::cout << "Prawe drzwi sie zamknely\n";
        });



        //n1->setTitle("Zniszcz wszystkie tarcze\n");
        //auto t = n1->addStep();
        
        tarcze->setTitle("Zniszcz trzy tarcze");

        tarcze->_onFinished = [](){
            std::cout << "Zniszczyles wszystkie tarcze\n";
        };

        
        std::vector<QuestNode*> t;

        for(int i = 0; i < 3; i++)
        {
            t.emplace_back(tarcze->addSubNode("Shield" + std::to_string(i+1)));
            auto* tar = t[i];
            std::string dopisek;
            if(i == 0) dopisek = "lewa";
            else if(i == 1) dopisek = "srodkowa";
            else if(i == 2) dopisek = "prawa";

            tar->setTitle("Zniszcz " + dopisek + " tarcze");

            auto* wid = w[i];
            tar->_onActivate = [tar, wid, &gui](){
                
                wid->setTexture(gui.getAsset<sf::Texture>("Assets/Quest/Shield.png"));
                wid->setCallback(CALL, [tar, wid, &gui](Widget*){
                    tar->nextStep();
                });
            };

            tar->_onFinished = [tar, wid, dopisek, &gui](){
                wid->setTexture(gui.getAsset<sf::Texture>("Assets/Quest/Broken Shield.png"));
                wid->setCallback(CALL, [=, &gui](Widget*){
                    tar->setTitle(dopisek + " tarcza zniszczona jej");
                    std::cout << "Ta " + dopisek + " tarcze jest zniszczona...\n";
                });
            };
        }

        auto* shieldLoot = tarcze->addStep("ShieldLoot");
        shieldLoot->addCounter("ShieldAmount", 3);
        
        shieldLoot->_onActivate = [w, shieldLoot, &gui](){
            for(auto* wid : w)
            {
                wid->setCallback(CALL, [](Widget* owner){});
                wid->setTexture(gui.getAsset<sf::Texture>("Assets/Quest/None.png"));
            }
            w[1]->setTexture(gui.getAsset<sf::Texture>("Assets/Quest/BetterShield.png"));
            w[1]->setCallback(CALL, [shieldLoot](Widget*){
                shieldLoot->advanceCounter("ShieldAmount");
                std::cout << shieldLoot->getValueFromCounter("ShieldAmount") << "\n";
            });
        };
        shieldLoot->setTitle("Podnies lepsza tarcze");
        shieldLoot->_onFinished = [](){
            std::cout << "Podniesiono lepsza tarcze\n";
        };
        
        auto* exitt = shieldLoot->addStep("Exit");
        exitt->setTitle("Opusc krypte");
        exitt->_onActivate = [=, &gui](){
            w[1]->setTexture(gui.getAsset<sf::Texture>("Assets/Quest/Exit.png"));
            w[1]->setCallback(CALL, [=](auto*){
                if(quest->isReqSuited("IsRingTaken"))
                {
                    std::cout << "Masz przeklety pierscien, nie mozesz wyjsc\n";
                }
                else if(quest->getValueFromCounter("TestCounter") == 13)
                {
                    std::cout << "Podniosles miecz, right?\n";
                }
                else
                {
                    std::cout << "Before Next\n";
                    exitt->nextStep();
                    std::cout << "After Next\n";
                }
            });
        };
        exitt->_onFinished = [](){
            std::cout << "Opusciles krypte\n";
        };

        _quest->addReq("IsRingTaken");

        _quest->addCounter("TestCounter", 23);
        
        auto* pied = n2->addBrancher();
        auto* wizard  = pied->addStep();
        auto* knight  = pied->addStep();
        pied->setTitle("Wez miecz lub pierscien");
        pied->_onActivate = [=, &gui](){
            w[2]->setTexture(gui.getAsset<sf::Texture>("Assets/Quest/Sword.png"));
            w[2]->setCallback(CALL, [=](auto*){
                pied->nextStep(1);
            });

            w[0]->setTexture(gui.getAsset<sf::Texture>("Assets/Quest/Ring.png"));
            w[0]->setCallback(CALL, [=](auto*){
                pied->nextStep(0);
                quest->suitReq("IsRingTaken");
            });
        };
        pied->_onFinished = [=, &gui](){
            w[0]->setTexture(gui.getAsset<sf::Texture>("Assets/Quest/None.png"));
            w[0]->setCallback(CALL, [](auto*){});
            w[2]->setTexture(gui.getAsset<sf::Texture>("Assets/Quest/None.png"));
            w[2]->setCallback(CALL, [](auto*){});
        };

        wizard->setTitle("Pokonaj czarodzieja");
        wizard->_onActivate = [w, &gui, wizard](){
            w[1]->setTexture(gui.getAsset<sf::Texture>("Assets/Quest/Wizard.png"));
            w[1]->setCallback(CALL, [wizard, w, &gui](auto*){
                w[1]->setTexture(gui.getAsset<sf::Texture>("Assets/Quest/DeadWizard.png"));
                w[1]->setCallback(CALL, [wizard](auto*){
                    wizard->nextStep();
                });
            });
        };
        wizard->_onFinished = [](){
            std::cout << "Pokonales Wizarda\n";
        };
        wizard->addNode(exitt);

        knight->setTitle("Pokonaj rycerza");
        knight->_onActivate = [=, &gui](){
            w[1]->setTexture(gui.getAsset<sf::Texture>("Assets/Quest/Knight.png"));
            w[1]->setCallback(CALL, [=, &gui](auto*){
                w[1]->setTexture(gui.getAsset<sf::Texture>("Assets/Quest/DeadKnight.png"));
                w[1]->setCallback(CALL, [=](auto*){
                    quest->advanceCounter("TestCounter", 13);
                    knight->nextStep();
                });
            });
        };
        knight->_onFinished = [](){
            std::cout << "Pokonales knighta\n";
        };
        knight->addNode(exitt);

        node->start();
        node->nextStep();
        */

    }

    void QuestLog::addQuest(std::string name)
    {
        new Quest(*this, name);
    }

    void QuestLog::update(float dt)
    {
    }
    void QuestLog::render()
    {
        
    }
    
    void _resetWidget(Widget* w)
    {
        w->setCallback(CALL, [](Widget*){});
    }

    QuestGUI* QuestLog::getGUI()
    {
        return _title.get();
    }

    void QuestLog::addQuestFrom(std::string name, Quest_t quest)
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
        quest->setName(name);
        _quests.emplace(name, std::move(quest));
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
        
    }
    
}

#undef CALL