#pragma once

#include <SFML/Graphics.hpp>

namespace rat {
    class TextStruct {
    public:
        TextStruct(size_t id=0u, size_t voiceStart=0u, size_t voiceEnd=0u, const std::string& speaker="", const std::string& text="");
        ~TextStruct();

        void setId(size_t id);
        size_t getId() const;

        void setMinorId(size_t minorId);
        size_t getMinorId() const;

        void setVoiceStart(size_t voiceStart);
        size_t getVoiceStart() const;

        void setVoiceEnd(size_t voiceEnd);
        size_t getVoiceEnd() const;

        void setSpeaker(const std::string& speaker);
        const std::string& getSpeaker() const;

        void setText(const std::string& text);
        const std::string& getText() const;
    private:
        size_t _id;
        size_t _minorId;
        size_t _voiceStart;
        size_t _voiceEnd;
        std::string _speaker;
        std::string _text;
    };
}