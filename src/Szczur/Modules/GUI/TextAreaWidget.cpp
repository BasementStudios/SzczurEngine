#include "TextAreaWidget.hpp"

#include <iostream>
#include <cassert>
#include <algorithm>

#include "Test.hpp"
#include "Szczur/Modules/Script/Script.hpp"
#include "Szczur/Utility/Convert/Unicode.hpp"

#include "InterfaceWidget.hpp"

namespace rat {
    TextAreaWidget::TextAreaWidget() 
    {
        _texts.emplace_back(sf::Text{});
        _texts.resize(1);
    }

    TextAreaWidget::TextAreaWidget(sf::Vector2u size, sf::Font* font) 
    :
    TextAreaWidget()
    {
        setFont(font);
    }

    void TextAreaWidget::initScript(Script& script) {
        auto object = script.newClass<TextAreaWidget>("TextAreaWidget", "GUI");
        basicScript(object);

        object.setProperty(
            "font",
            [](TextAreaWidget& owner){owner._font;},
            [](TextAreaWidget& owner, sf::Font* font){owner.setFont(font);}
        );

        object.setProperty(
            "text",
            [](TextAreaWidget& owner){return owner.getString();},
            [](TextAreaWidget& owner, const std::string& text){owner.setString(text);}
        );

        object.setProperty(
            "fontSize",
            [](TextAreaWidget& owner){return owner.getCharacterSize();},
            [](TextAreaWidget& owner, size_t size){owner.setCharacterSize(size);}
        );

        object.setProperty(
            "color",
            [](TextAreaWidget& owner){return owner._color;},
            [](TextAreaWidget& owner, sol::table tab){ owner.setColor( sf::Color(tab[1], tab[2], tab[3]) ); }
        );

        /*
        object.setProperty(
            "size",
            [](TextAreaWidget& owner){return owner._size;},
            [](TextAreaWidget& owner, sol::table tab){ owner.setTextSize(sf::Vector2u{tab[1], tab[2]}); }
        );*/
        
        object.init();
    }

    void TextAreaWidget::setString(const std::string& text)
    {
        _str = text;
        _aboutToRecalculate = true;
    }
    const std::string& TextAreaWidget::getString() const
    {
        return _str;
    }

    void TextAreaWidget::setFont(sf::Font* font) {
        _font = font;
        _isFontChanged = true;
        _aboutToRecalculate = true;
    }
    const sf::Font* TextAreaWidget::getFont() const
    {
        return _font;
    }

    void TextAreaWidget::setCharacterSize(size_t size)
    {
        _chSize = size;
        _isChChanged = true;
        _aboutToRecalculate = true;
    }
    size_t TextAreaWidget::getCharacterSize() const
    {
        return _chSize;
    }

    void TextAreaWidget::setCharacterPropSize(float prop)
    {
        _hasChPropSize = true;
        _chPropSize = prop;

        if(_interface) _calcChPropSize();
        else _elementsPropSizeMustBeenCalculated = true;

    }
    void TextAreaWidget::_calcChPropSize()
    {
        assert(_hasChPropSize);
        assert(_interface);

        auto size = _interface->getSizeByPropSize({_chPropSize, _chPropSize});
        setCharacterSize(size_t(std::min(size.x, size.y)));
    }
    float TextAreaWidget::getCharacterPropSize() const
    {
        return _chPropSize;
    }

    void TextAreaWidget::_recalcElementsPropSize()
    {
        if(_hasChPropSize) _calcChPropSize();
    }

    void TextAreaWidget::_setColor(const sf::Color& color) 
    {
        _color = color;
        for(auto& t : _texts) t.setFillColor(color);
    }

    void TextAreaWidget::_draw(sf::RenderTarget& target, sf::RenderStates states) const 
    {
        for(auto& t : _texts) target.draw(t, states);
    }

    sf::String& TextAreaWidget::_wrapText(sf::String& temp) {
        for(size_t i = 0; i < temp.getSize(); ++i)
            if(temp[i] == '\n')
                temp[i] = ' ';
        for(size_t i = _size.x; i < temp.getSize(); i += _size.x) 
        {
            auto x = i;
            while(temp[x] != ' ') {
                if(--x == 0u) {
                    x=i;
                    break;
                }
            }
            if(temp[x] == ' ')
                temp[x] = '\n';
            else
                temp.insert(x, "\n");
        }
        return temp;
    }

    void TextAreaWidget::_wrap()
    {
        if(!_font && !_chSize) return;

        const float width = _minSize.x;
        size_t i = 0;

        sf::String str = getUnicodeString(_str);
        const char endingKey = 4;
        str += endingKey;

        sf::Text text;
        text.setString(str);
        text.setFont(*_font);
        text.setCharacterSize(_chSize);

        size_t begin = 0;
        size_t end;

        float lineWidth = 0.f;
        size_t lineIndex = 0u;

        bool isReadyToBreak = false;

        bool isTooThick = false;
        bool isFirstKey = true;

        while(true)
        {
            bool mustIncrNewBegin = true;
            size_t spaceIndex;
            bool wasSpace = false;
            float fromSpace = 0.f;
            bool isFirstKeyInLine = true;
            
            for( ; ; ++i)
            {
                end = i;

                const char key = str[i];

                if(key == '\n')
                {
                    ++i;
                    wasSpace = false;
                    break;
                }
                else if(key == endingKey)
                {
                    isReadyToBreak = true;
                    break;
                }
                else if(key == ' ')
                {
                    if(isFirstKeyInLine) continue;

                    wasSpace = true;
                    spaceIndex = i;
                    fromSpace = 0.f;
                }

                const float keyWidth = text.findCharacterPos(i + 1).x - text.findCharacterPos(i).x;
                lineWidth += keyWidth;
                fromSpace += keyWidth;

                if(lineWidth > width)
                {
                    if(isFirstKey)
                    {
                        isTooThick = true;
                        break;
                    }

                    if(wasSpace)
                    {
                        end = spaceIndex;
                        if(key == ' ') fromSpace = 0.f;
                    }
                    else 
                    {
                        mustIncrNewBegin = false;
                    }
                    break;
                }

                isFirstKeyInLine = false;
                isFirstKey = false;
            }
            if(end == begin) break;

            auto lineStr = str.substring(begin, end - begin);
        
            if(lineIndex < _texts.size())
            {
                auto& t = _texts[lineIndex];
                t.setString(lineStr);
            }
            else
            {
                sf::Text t;
                t.setString(lineStr);
                t.setFillColor(_color);
                t.setFont(*_font);
                t.setCharacterSize(_chSize);
                _texts.emplace_back(t);
            }
            lineIndex++;


            if(isReadyToBreak) break;

            if(wasSpace) lineWidth = fromSpace;
            else lineWidth = 0.f;

            begin = end;
            if(mustIncrNewBegin) begin++;
        }

        _texts.resize(std::max(size_t(1), lineIndex));

        //_isChChanged = true;
        //_isFontChanged = true;
    }

    void TextAreaWidget::_convertToMultiLines()
    {
        size_t lineIndex = 0;
        const size_t oldSize = _texts.size();
        size_t begin = 0;
        size_t end;
        size_t i = 0;
        auto str = getUnicodeString(_str);
        const char endingKey = 5;
        str += endingKey;
        for(const char& key : str)
        {
            if(key == '\n'|| key == endingKey)
            {
                auto lineStr = str.substring(begin, i - begin);

                if(lineIndex < _texts.size())
                {
                    auto& t = _texts[lineIndex];
                    t.setString(lineStr);
                }
                else
                {
                    sf::Text t;
                    t.setFillColor(_color);
                    t.setFont(*_font);
                    t.setCharacterSize(_chSize);
                    t.setString(lineStr);
                    _texts.emplace_back(t);
                }
                lineIndex++;
                begin = i + 1;
            }
            ++i;
        }
        _texts.resize(lineIndex);

        _updateFont();
        _updateChSize();
        
        _calcTextPos();
    }

    void TextAreaWidget::setAlign(Align align)
    {
        _align = align;
        _isPosChanged = true;
    }
    TextAreaWidget::Align TextAreaWidget::getAlign() const
    {
        return _align;
    }   

    float TextAreaWidget::_getAlignFactor() const
    {
        switch(_align)
        {
            case Align::Left: return 0.f; break;
            case Align::Center: return 0.5f; break;
            case Align::Right: return 1.f; break;
            default: return 0.f; break;
        }
    }
    void TextAreaWidget::_calcTextPos()
    {
        float width = 0.f;
        if(_isMinSizeSet)
        {
            width = _minSize.x;
        }
        else
        {
            for(auto& t : _texts)
            {
                const auto& rect = t.getGlobalBounds();
                if(rect.width > width) width = rect.width;
            }
        }

        float totalHeight = 0.f;
        const auto& drawPos = gui::FamilyTransform::getDrawPosition();

        const float alignFactor = _getAlignFactor();
        for(auto& t : _texts)
        {
            const auto rect = t.getGlobalBounds();
            float lineX = alignFactor * (width - rect.width);
            sf::Vector2f pos(lineX, totalHeight);
            t.setPosition(drawPos + pos);
            totalHeight += rect.height;
        }
    }

    void TextAreaWidget::_calculateSize()
    {   
        if(_font)
        {
            if(_isMinSizeSet)
            {
                _wrap();
            }
            else
            {
                _convertToMultiLines();
            }
        }

        if(_isFontChanged && _font) _updateFont();
        if(_isChChanged) _updateChSize();

        _isPosChanged = true;
    }

    void TextAreaWidget::_updateFont()
    {
        assert(_font);
        for(auto& t : _texts) t.setFont(*_font);
        _isFontChanged = false;
    }
    void TextAreaWidget::_updateChSize()
    {
        for(auto& t : _texts) t.setCharacterSize(_chSize);
        _isChChanged = false;
    }

    sf::Vector2f TextAreaWidget::_getSize() const 
    {
        const auto& drawPos = gui::FamilyTransform::getDrawPosition();
        float height = 0.f;
        float width = 0.f;

        {
            const auto& lastText = _texts.back();
            const auto rect = lastText.getGlobalBounds();
            height = rect.height + rect.top - drawPos.y;
        }

        if(_isMinSizeSet)
        {
            width = _minSize.x;
        }
        else
        {
            for(const auto& t : _texts)
            {
                const auto rect = t.getGlobalBounds();
                if(rect.width > width) width = rect.width;
            }
        }     

        return {width, height};
    }

    void TextAreaWidget::_recalcPos()
    {
        _calcTextPos();
    }

    void TextAreaWidget::_callback(CallbackType type) {
        if(auto it = _luaCallbacks.find(type); it != _luaCallbacks.end())
            std::invoke(it->second, this);
        if(auto it = _callbacks.find(type); it != _callbacks.end())
            std::invoke(it->second, this);
    }
}