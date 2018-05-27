#include "QuestLog.hpp"

#include "Szczur/Utility/Logger.hpp"

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
        _quest = std::make_unique<Quest>();
        auto* node = _quest->getRoot();
        _widget = getModule<GUI>().addInterface();
        _widget->setSize(40.f, 50.f);
        _widget->setPosition(100, 100);

        auto* w1 = new Widget;
        _widget->add(w1);
        w1->setSize(100, 100);
        w1->setPosition(300, 100);

        auto* w2 = new Widget;
        _widget->add(w2);
        w2->setSize(100, 100);
        w2->setPosition(500, 100);

        auto* w3 = new Widget;
        _widget->add(w3);
        w3->setSize(100, 100);
        w3->setPosition(100, 100);

        auto* n1 = node->addStep();
        auto* n2 = node->addStep();

        node->setTitle("Wejdz do Ktorychs ze dzrwi");
        node->setActivateCallback([w1, w2, n1, n2](){
            w1->setCallback(CALL, [n1, w1](Widget*){
                n1->start();
            });
            w2->setCallback(CALL, [n2, w2](Widget*){
                n2->start();
            });
        });

        n1->setTitle("Wejdz w lewe");
        n1->_onFinished = [](){
            std::cout << "Wszedles w lewe drzwi\n";
        };
        n1->_onActivate = [w1, n1](){
            w1->setCallback(CALL, [n1](Widget*){
                n1->nextStep();
            });
        };

        n2->setTitle("Wejdz w prawe");
        n2->_onFinished = [](){
            std::cout << "Wszedles w prawe drzwi\n";
        };
        n2->_onActivate = [w2, n2](){
            w2->setCallback(CALL, [n2](Widget*){
                n2->nextStep();
            });
        };
        n1->setBlockedCallback([this, w1](){
            w1->setCallback(CALL, [](Widget*){});
            std::cout << "Lewe drzwi sie zamknely\n";
        });
        n2->setBlockedCallback([this, w2](){
            w2->setCallback(CALL, [](Widget*){});
            std::cout << "Prawe drzwi sie zamknely\n";
        });

        //n1->setTitle("Zniszcz wszystkie tarcze\n");
        auto t = n1->addSubNode();
        t->setActivateCallback([w1, t](){
            w1->setCallback(CALL, [t](Widget*){
                t->nextStep();
            });
        });
        t->setFinishedCallback([w1](){
            w1->setCallback(CALL, [](Widget*){
                std::cout << "Ta tarcza jest juz zniszczona\n";
            });
        });

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
    
}