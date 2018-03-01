#include "TextStruct.hpp"

namespace rat {
    TextStruct::TextStruct(size_t id, size_t voiceStart, size_t voiceEnd, const std::string& speaker, const std::string& text) :
    _id(id),
    _voiceStart(voiceStart),
    _voiceEnd(voiceEnd),
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

    void TextStruct::setMinorId(size_t minorId) {
        _minorId = minorId;
    }
    
    size_t TextStruct::getMinorId() const {
        return _minorId;
    }

    void TextStruct::setVoiceStart(size_t voiceStart) {
        _voiceStart = voiceStart;
    }
    
    size_t TextStruct::getVoiceStart() const {
        return _voiceStart;
    }

    void TextStruct::setVoiceEnd(size_t voiceEnd) {
        _voiceEnd = voiceEnd;
    }
    
    size_t TextStruct::getVoiceEnd() const {
        return _voiceEnd;
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