#include <SFML/Graphics.hpp>

#include <Szczur/Modules/GUI/GUI.hpp>
#include <Szczur/Modules/GUI/TextWidget.hpp>
#include "Szczur/Modules/GUI/ScrollAreaWidget.hpp"
#include "Szczur/Modules/GUI/ListWidget.hpp"

#include "Quest.hpp"

namespace rat
{
    class TextWidget; class ScrollAreaWidget;class ListWidget;
    class StepsManager
    {
        public:
            StepsManager(sf::Font *font,InterfaceWidget* interface);
            ~StepsManager();

            void refresh();
            void setScrollTextures(sf::Texture *t1,sf::Texture *t2,sf::Texture *t3);

            void setQuest(std::shared_ptr<Quest> quest);
        private:
            std::shared_ptr<std::vector<std::string> > _steps;
            sf::Font* _font;
            ScrollAreaWidget _scroller;
            std::shared_ptr<Quest> _quest;
            ListWidget _list;
            
    };
}