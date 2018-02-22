#pragma once

#include <SFML/Graphics.hpp>

namespace rat {
    class TextStruct {
    public:
        TextStruct(size_t id=0u, size_t voiceStart=0u, size_t voiceLength=0u, const std::string& speaker="", const std::string& text="");
        ~TextStruct();

        void setId(size_t id);
        size_t getId() const;

        void setVoiceStart(size_t voiceStart);
        size_t getVoiceStart() const;

        void setVoiceLength(size_t voiceLength);
        size_t getVoiceLength() const;

        void setSpeaker(const std::string& speaker);
        const std::string& getSpeaker() const;

        void setText(const std::string& text);
        const std::string& getText() const;
    private:
        size_t _id;
        size_t _voiceStart;
        size_t _voiceLength;
        std::string _speaker;
        std::string _text;
    };
}