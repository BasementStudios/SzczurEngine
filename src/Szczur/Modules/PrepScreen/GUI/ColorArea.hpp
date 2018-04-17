#pragma once 

#include <vector>

namespace rat
{
    
    class Widget; class ImageWiget;

    class ColorArea
    {
    public:
    private:
        float _padding{5.f};
        float _dim{25.f};
        float _width;

        Widget* _base;
        //std::vector<ImageWidget*> _colors;


    };
    
}