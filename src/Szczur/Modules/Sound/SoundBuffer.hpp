#pragma once

#include <SFML/Audio/SoundBuffer.hpp>

#include "Szczur/Utility/Logger.hpp"

namespace rat
{
    class SoundBuffer : public sf::SoundBuffer
    {

        unsigned int _counter {0};
        std::string _fileName {""};

    public:

        const std::string& getName() const    { return _fileName; }
        void setName(const std::string& name) { _fileName = name; }  

        void increment() { _counter++; }
        void decrement() { _counter--; }

        int getCounter() { return _counter; }

    };
}