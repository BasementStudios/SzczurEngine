#pragma once

#include <cstdlib>

#include "Szczur/Modules/GUI/Widget.hpp"

namespace rat
{
    class Widget;
    struct StressTester
    {
        int level;
        int branchAmount;
        int posRange;
        int sizeRange;
        void makeBranches(Widget* root)
        {
            if(level == 0) return;
            float pos = float(rand()%posRange+1);
            unsigned int size = (unsigned int)(rand()%sizeRange + 1);
            auto* branch = new Widget;
            root->add(branch);
            branch->setSize(size, size);
            branch->setPosition(pos, pos);
            for(size_t i = 0; i < branchAmount; i++)
            {
                auto branchTester = *this;
                branchTester.level--;
                branchTester.makeBranches(branch);
            }
        }
    };
}