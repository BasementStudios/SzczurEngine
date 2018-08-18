#include "ComponentRegistry.hpp"

namespace rat::wrd
{

bool ComponentRegistry::assignComponent(const Entity& entity, HashedID hid) const
{
    return _call(_assignComponent, entity, hid);
}

bool ComponentRegistry::hasComponent(const Entity& entity, HashedID hid) const
{
    return _call(_hasComponent, entity, hid);
}

bool ComponentRegistry::removeComponent(const Entity& entity, HashedID hid) const
{
    return _call(_removeComponent, entity, hid);
}

bool ComponentRegistry::assignTag(const Entity& entity, HashedID hid) const
{
    return _call(_assignTag, entity, hid);
}

bool ComponentRegistry::hasTag(const Entity& entity, HashedID hid) const
{
    return _call(_hasTag, entity, hid);
}

}
