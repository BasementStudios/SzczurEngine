#include "QuestLog.hpp"

#include <vector>

#include "Szczur/Utility/Logger.hpp"

#include "Szczur/Modules/GUI/ImageWidget.hpp"
#include "GUI/QuestInfoBar/QuestTitle.hpp"

#define CALL Widget::CallbackType::onRelease

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
        _quest = std::make_unique<Quest>(*this);
        auto* quest = _quest.get();
        auto* node = _quest->getRoot();
        _widget = getModule<GUI>().addInterface();
        _widget->setSize(40.f, 50.f);
        _widget->setPosition(100, 100);
        
        

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
        //_title->setSize(200, 20);
        _title->setTitle("Wez mydlo");
        auto* sub = _title->addSubtitle("Hash");
        sub->setReq("Great", 2, 3);
        _title->resetSubtitles();
        /*
        auto* subT = _title->getSubReq(0);
        subT->setReq("Zebrane mydla", 0, 3);
        auto* subT2 = _title->getSubReq(0);
        subT2->setReq("Wziales prysznic", false);
        */
        
        std::vector<ImageWidget*> w;

        for(int i = 0; i < 3; i++)
        {
            w.emplace_back(new ImageWidget);
            _widget->add(w[i]);
            w[i]->setSize(100, 100);
            w[i]->setPosition(100 + i *200, 100);
            
        }
        auto* choice = node->addBrancher();
        choice->setTitle("Wejdz do ktorychs z drzwi");
        auto* n1 = choice->addStep();
        auto* n2 = choice->addStep();
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

        n1->_onActivate = [n1](){
            n1->nextStep();
        };

        n1->setBlockedCallback([w, &gui](){
            w[0]->setCallback(CALL, [](Widget*){});
            w[0]->setTexture(gui.getAsset<sf::Texture>("Assets/Quest/None.png"));            
            std::cout << "Lewe drzwi sie zamknely\n";
        });

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
        auto* tarcze = n1->addStep();
        tarcze->setTitle("Zniszcz trzy tarcze");

        tarcze->_onFinished = [](){
            std::cout << "Zniszczyles wszystkie tarcze\n";
        };

        
        std::vector<QuestNode*> t;

        for(int i = 0; i < 3; i++)
        {
            t.emplace_back(tarcze->addSubNode());
            auto* tar = t[i];
            std::string dopisek;
            if(i == 0) dopisek = "lewa";
            else if(i == 1) dopisek = "srodkowa";
            else if(i == 2) dopisek = "prawa";

            tar->setTitle("Zniszcz " + dopisek + " tarcze");

            auto* wid = w[i];
            tar->_onActivate = [tar, wid, &gui](){
                
                std::cout << "eiribv\n";
                wid->setTexture(gui.getAsset<sf::Texture>("Assets/Quest/Shield.png"));
                wid->setCallback(CALL, [tar, wid, &gui](Widget*){
                    tar->nextStep();
                });
            };

            tar->_onFinished = [tar, wid, dopisek, &gui](){
                wid->setTexture(gui.getAsset<sf::Texture>("Assets/Quest/Broken Shield.png"));
                wid->setCallback(CALL, [tar, wid, dopisek, &gui](Widget*){
                    std::cout << "Ta " + dopisek + " tarcze jest zniszczona...\n";
                });
            };
        }

        auto* shieldLoot = tarcze->addStep();
        shieldLoot->addCounter("Shields", 3);
        
        shieldLoot->_onActivate = [w, shieldLoot, &gui](){
            for(auto* wid : w)
            {
                wid->setCallback(CALL, [](Widget* owner){});
                wid->setTexture(gui.getAsset<sf::Texture>("Assets/Quest/None.png"));
            }
            w[1]->setTexture(gui.getAsset<sf::Texture>("Assets/Quest/BetterShield.png"));
            w[1]->setCallback(CALL, [shieldLoot](Widget*){
                shieldLoot->advanceCounter("Shields");
            });
        };
        shieldLoot->setTitle("Podnies lepsza tarcze");
        shieldLoot->_onFinished = [](){
            std::cout << "Podniesiono lepsza tarcze\n";
        };
        
        auto* exitt = shieldLoot->addStep();
        exitt->setTitle("Opusc krypte");
        exitt->_onActivate = [=, &gui](){
            w[1]->setTexture(gui.getAsset<sf::Texture>("Assets/Quest/Exit.png"));
            w[1]->setCallback(CALL, [=](auto*){
                if(quest->isReqSuited("IsRingTaken"))
                {
                    std::cout << "Masz przeklety pierscien, nie mozesz wyjsc\n";
                }
                else
                {
                    exitt->nextStep();
                }
            });
        };
        exitt->_onFinished = [](){
            std::cout << "Opusciles krypte\n";
        };

        _quest->addReq("IsRingTaken");
        
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
        knight->_onActivate = [w, &gui, knight](){
            w[1]->setTexture(gui.getAsset<sf::Texture>("Assets/Quest/Knight.png"));
            w[1]->setCallback(CALL, [knight, w, &gui](auto*){
                w[1]->setTexture(gui.getAsset<sf::Texture>("Assets/Quest/DeadKnight.png"));
                w[1]->setCallback(CALL, [knight](auto*){
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
    
}

#undef CALL