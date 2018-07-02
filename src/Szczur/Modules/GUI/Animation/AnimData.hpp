#pragma once

#include <functional>
#include <glm/gtc/constants.hpp>
#include <cmath>

namespace rat
{
namespace gui
{
    enum class Easing
    {
        Linear,
        
        EaseInQuad,
        EaseOutQuad,
        EaseInOutQuad,

        EaseInCubic,
        EaseOutCubic,
        EaseInOutCubic,

        EaseInQuart,
        EaseOutQuart,
        EaseInOutQuart,

        EaseInQuint,
        EaseOutQuint,
        EaseInOutQuint,

        EaseOutElastic,

        EaseInBounce,
        EaseOutBounce,
        EaseInOutBounce,

        EaseInExpo,
        EaseOutExpo,
        EaseInOutExpo
    };
    struct EasingFuncs
    {
        static std::function<float(float)> get(Easing type)
        {
            
            switch (type)
            {
                case Easing::Linear: return [](float t){ return t; };
                    break;
                case Easing::EaseInQuad: return [](float t){ return t * t; };
                    break;
                case Easing::EaseOutQuad: return [](float t){ return t * (2.f - t); };
                    break;
                case Easing::EaseInOutQuad: return [](float t){ return t < 0.5f ? (2.f * t * t) : (-1.f + (4.f - 2.f * t ) * t); };
                    break;
                case Easing::EaseInCubic: return [](float t){ return t * t * t; };
                    break;
                case Easing::EaseOutCubic: return [](float t){ t -= 1.f; return t * t * t + 1.f; };
                    break;
                case Easing::EaseInOutCubic: return [](float t){ return t < 0.5f ? (4.f * t * t * t) : (t - 1.f)*(2.f * t - 2.f)*(2.f * t - 2.f) + 1.f; };
                    break;
                case Easing::EaseInQuart: return [](float t){ return t * t * t *t; };
                    break;
                case Easing::EaseOutQuart: return [](float t){ t -= 1.f; return 1.f - t * t * t * t; };
                    break;
                case Easing::EaseInOutQuart: return [](float t){ return t < 0.5f ? (8.f * t * t * t * t) : 1.f - 8.f * (--t) * t * t * t; };
                    break;
                case Easing::EaseInQuint: return [](float t){ return t * t * t * t * t; };
                    break;
                case Easing::EaseOutQuint: return [](float t){ return 1.f + (--t) * t * t * t * t; };
                    break;
                case Easing::EaseInOutQuint: return [](float t){ return t < 0.5f ? 16.f * t * t * t * t * t : 1.f + 16.f * (--t) * t * t * t * t; };
                    break;
                case Easing::EaseOutElastic: return [](float t){ float p = 0.3f; return pow(2,-10.f * t) * sin((t - p / 4.f)*(2.f * glm::pi<float>()) / p) + 1.f; };
                    break;
                
                case Easing::EaseInBounce: return [](float t){ return pow( 2, 6.f * (t - 1.f) ) * abs( sin( t * glm::pi<float>() * 3.5f )); };
                    break;
                case Easing::EaseOutBounce: return [](float t){ return 1.f - pow( 2, -6.f * t ) * abs( cos( t * glm::pi<float>() * 3.5f ) ); };
                    break;
                case Easing::EaseInOutBounce: return [](float t){ return t < 0.5f ? 8 * pow( 2, 8.f * (t - 1.f) ) * abs( sin( t * glm::pi<float>() * 7.f ) ) : 1.f - 8.f * pow( 2, -8.f * t ) * abs( sin( t * glm::pi<float>() * 7.f ) ); };
                    break;
                
                case Easing::EaseInExpo: return [](float t){ return (pow(2, 8.f * t ) - 1.f) / 255.f; };
                    break;
                case Easing::EaseOutExpo: return [](float t){ return 1.f - pow(2, -8.f * t ); };
                    break;
                case Easing::EaseInOutExpo: return [](float t){ return t < 0.5f ? ((pow(2, 16.f * t ) - 1.f) / 510.f) : (1.f - 0.5f * pow( 2, -16.f * (t - 0.5f) )); };
                    break;

                default:
                return [](float){return 1.f; };
                    break;
            }
        }
    };

    struct AnimData
    {
        float inTime{0.f};
        Easing easing{Easing::Linear};
        std::function<void()> onFinishCallback{[](){}};

        AnimData() = default;
        AnimData(float inTime, Easing easing = Easing::Linear, std::function<void()> onFinish = [](){}) : inTime(inTime), easing(easing), onFinishCallback(onFinish) {}
        //AnimData(float inTime, std::function<void()> onFinish = [](){}, Easing easing = Easing::Linear) : inTime(inTime), easing(easing), onFinishCallback(onFinish) {}
    };
}
}