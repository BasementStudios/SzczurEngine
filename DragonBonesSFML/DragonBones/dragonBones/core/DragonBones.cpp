#include "DragonBones.h"
#include "../armature/Armature.h"
#include "../animation/WorldClock.h"
#include "../animation/Animation.h"
#include "../events/EventObject.h"
#include "../events/IEventDispatcher.h"

DRAGONBONES_NAMESPACE_BEGIN

bool DragonBones::yDown = true;
bool DragonBones::debug = false;
bool DragonBones::debugDraw = false;
bool DragonBones::webAssembly = false;
const std::string DragonBones::version = "5.5.1";

DragonBones::DragonBones(IEventDispatcher* eventManager) :
    _events(),
    _objects(),
    _clock(nullptr),
    _eventManager(eventManager)
{
    _clock = new WorldClock();
    _eventManager = eventManager;
}

DragonBones::~DragonBones()
{
    if (_clock != nullptr)
    {
        delete _clock;
    }

    _clock = nullptr;
    _eventManager = nullptr;
}
void DragonBones::advanceTime(float passedTime)
{
    if (!_objects.empty())
    {
        for (const auto object : _objects)
        {
            object->returnToPool();
        }

        _objects.clear();
    }

    _clock->advanceTime(passedTime);

    if (!_events.empty()) 
    {
        for (std::size_t i = 0; i < _events.size(); ++i)
        {
            const auto eventObject = _events[i];
            const auto armature = eventObject->armature;
            if (armature->armatureData != nullptr)
            {
                armature->getProxy()->_dispatchEvent(eventObject->type, eventObject);
                if (eventObject->type == EventObject::SOUND_EVENT)
                {
                    _eventManager->_dispatchEvent(eventObject->type, eventObject);
                }
            }

            bufferObject(eventObject);
        }

        _events.clear();
    }
}

void DragonBones::bufferEvent(EventObject* value)
{
    _events.push_back(value);
}

void DragonBones::bufferObject(BaseObject* object)
{
    _objects.push_back(object);
}

WorldClock* DragonBones::getClock()
{
    return _clock;
}


DRAGONBONES_NAMESPACE_END
