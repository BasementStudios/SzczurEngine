#include "Listener.hpp"

namespace rat
{

    Listener::Listener()
    {
        initScript();
		LOG_INFO(this, " : Module Music constructed");
    }

    Listener::~Listener()
    {
        LOG_INFO(this, " : Module Music destructed");
    }

    void Listener::initScript()
    {
        Script& script = getModule<Script>();
		auto module = script.newModule("Listener");

		module.set_function("setGlobalVolume", &Listener::setGlobalVolume, this);
		module.set_function("getGlobalVolume", &Listener::getGlobalVolume, this);
		module.set_function("setPosition", &Listener::setPosition, this);
		module.set_function("getPosition", &Listener::getPosition, this);
        module.set_function("setDirection", &Listener::setDirection, this);
		module.set_function("getDirection", &Listener::getDirection, this);
        module.set_function("setUpVector", &Listener::setUpVector, this);
		module.set_function("getUpVector", &Listener::getUpVector, this);
        module.set_function("getOffsetZ", &Listener::getOffsetZ, this);
        module.set_function("setOffsetZ", &Listener::setOffsetZ, this);

    }

    void Listener::setGlobalVolume(float volume)
    {
        sf::Listener::setGlobalVolume(volume);
    }

    float Listener::getGlobalVolume()
    {
        return sf::Listener::getGlobalVolume();
    }

    void Listener::setPosition(float x, float y, float z)
    {
        sf::Listener::setPosition(x, y, z + _offsetZ);
    }

    sf::Vector3f Listener::getPosition()
    {
        return sf::Listener::getPosition();
    }

    void Listener::setDirection(float x, float y, float z)
    {
        sf::Listener::setDirection(x, y, z);    
    }

    sf::Vector3f Listener::getDirection()
    {
        return sf::Listener::getDirection();
    }

    void Listener::setUpVector(float x, float y, float z)
    {
        sf::Listener::setUpVector(x, y, z);
    }

    sf::Vector3f Listener::getUpVector()
    {
        return sf::Listener::getUpVector();
    }

    void Listener::setOffsetZ(float offsetZ)
    {
        _offsetZ = offsetZ;
    }

    float Listener::getOffsetZ() const
    {
        return _offsetZ;
    }

}