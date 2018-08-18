#include "ComponentRegistry.hpp"

namespace rat::wrd
{

void ComponentRegistry::assign(const Entity& entity, HashedID hid) const
{
    _call(_assign, entity, hid);
}

void ComponentRegistry::remove(const Entity& entity, HashedID hid) const
{
    _call(_remove, entity, hid);
}

void ComponentRegistry::_call(const FunctorMap_t& _map, const Entity& entity, HashedID hid) const
{
    const auto it = _map.find(hid.hash);

    if (it != _map.end())
    {
        std::invoke(it->second, entity.getRegistry(), entity.getID());
    }
}

}
