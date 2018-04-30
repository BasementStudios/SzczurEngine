#pragma once

#include <functional>

#include <boost/container/flat_map.hpp>

#include <SFML/Graphics.hpp>

namespace rat {
    class TextStruct {
    public:
        using Key_t = size_t;

        using Texts_t = boost::container::flat_map<Key_t, std::pair<std::string, std::string>>;


        TextStruct(size_t id=0u, size_t minorId=0u, size_t voiceStart=0u, size_t voiceEnd=0u);
        ~TextStruct();

        void setId(size_t id);
        size_t getId() const;

        void setMinorId(size_t minorId);
        size_t getMinorId() const;

        void setIdName(const std::string& name);
        const std::string& getIdName() const;

        void setMinorIdName(const std::string& name);
        const std::string& getMinorIdName() const;

        void setVoiceStart(size_t voiceStart);
        size_t getVoiceStart() const;

        void setVoiceEnd(size_t voiceEnd);
        size_t getVoiceEnd() const;

        void setLabel(const std::string& label);
        const std::string& getLabel() const;

        void interpretText(const std::string& text);

        void forEach(std::function<void(Texts_t::iterator)> func);
    private:
        size_t _id;
        std::string _idName;
        size_t _minorId;
        std::string _minorIdName;
        size_t _voiceStart;
        size_t _voiceEnd;
        std::string _label;
        Texts_t _texts;
    };
}