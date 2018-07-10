#pragma once

#include "Widget.hpp"

#include "Szczur/Modules/GUITest/NinePatch.hpp"

namespace rat
{
    class Script;
    class WindowWidget : public Widget
    {
    public:
        WindowWidget();
        void setTexture(sf::Texture* texture, int padding);
        void setTexture(sf::Texture* texture, int paddingWidth, int paddingHeight);
        void setScale(const sf::Vector2f& scale);
        void setScale(float x, float y);

        void setPatchAmount(const sf::Vector2u& amount);
        void setPatchAmount(unsigned int horizontalAmount, unsigned int verticalAmount);

        void setMainPatchPropSize(const sf::Vector2f& propSize);

    protected:
        virtual void _draw(sf::RenderTarget& target, sf::RenderStates states) const override;
		virtual sf::Vector2f _getSize() const override;
		virtual void _calculateSize() override;
        virtual void _setColor(const sf::Color& color) override;
        virtual void _recalcPos() override;
        virtual void _recalcElementsPropSize() override;
    private:
        NinePatch _ninePatch;
        sf::Vector2f _scale{1.f, 1.f};

        sf::Vector2f _minWinSize;
        sf::Vector2u _patchesAmount;
        bool _isPathesAmountSet{false};

        bool _isMainPatchPropSizeSet{false};
        sf::Vector2f _mainPathPropSize;
        void _calcMainPatchSize();

        void _calcPatchesAmount();

    public:
        static void initScript(Script& script);
    };
}