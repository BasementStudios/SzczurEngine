#pragma once

#include <set>
#include <functional>

#include "PPGroup.hpp"

namespace rat
{
class PPContainer
{

public:

    void add(const PPGroup& addon)
    {
        auto found = ppCont.find(addon);
        if(found == ppCont.end()) ppCont.insert(addon);
        else found->add(addon);
    }

    void remove(const PPGroup& sub)
    {
        assert(ppCont.count(sub) == 1);
        auto found = ppCont.find(sub);
        if(found->getAmount() > sub.getAmount()) found->remove(sub);
        else ppCont.erase(found);
    }

    size_t getAmount(const std::string type, size_t power)
    {
        auto found = ppCont.find(PPGroup(type, power));
        if(found == ppCont.end()) return 0u;
        return found->getAmount();
    }

private:
    std::set<PPGroup, std::greater<PPGroup>> ppCont;

};
}