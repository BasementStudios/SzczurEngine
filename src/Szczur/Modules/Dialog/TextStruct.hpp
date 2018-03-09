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

        void setVoiceStart(size_t voiceStart);
        size_t getVoiceStart() const;

        void setVoiceEnd(size_t voiceEnd);
        size_t getVoiceEnd() const;

        void interpretText(const std::string& text);

        void forEach(std::function<void(Texts_t::iterator)> func);
    private:
        size_t _id;
        size_t _minorId;
        size_t _voiceStart;
        size_t _voiceEnd;
        Texts_t _texts;
    };
}