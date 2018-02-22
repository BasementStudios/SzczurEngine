#include "TextStruct.hpp"

namespace rat {
    TextStruct::TextStruct(size_t id, size_t voiceStart, size_t voiceLength, const std::string& speaker, const std::string& text) :
    _id(id),
    _voiceStart(voiceStart),
    _voiceLength(voiceLength),
    _speaker(speaker),
    _text(text)
    {

    }

    TextStruct::~TextStruct() {

    }


    void TextStruct::setId(size_t id) {
        _id = id;
    }
    
    size_t TextStruct::getId() const {
        return _id;
    }

    void TextStruct::setVoiceStart(size_t voiceStart) {
        _voiceStart = voiceStart;
    }
    
    size_t TextStruct::getVoiceStart() const {
        return _voiceStart;
    }

    void TextStruct::setVoiceLength(size_t voiceLength) {
        _voiceLength = voiceLength;
    }
    
    size_t TextStruct::getVoiceLength() const {
        return _voiceLength;
    }

    void TextStruct::setSpeaker(const std::string& speaker) {
        _speaker = speaker;
    }
    
    const std::string& TextStruct::getSpeaker() const {
        return _speaker;
    }

    void TextStruct::setText(const std::string& text) {
        _text = text;
    }
    
    const std::string& TextStruct::getText() const {
        return _text;
    }
}