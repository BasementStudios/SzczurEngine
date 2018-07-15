#pragma once

namespace rat 
{

class ScenesManager;

class ArmatureDisplayDataManager 
{
public:
    ArmatureDisplayDataManager(ScenesManager& scenes);

    void render(bool& ifRender);

private:
    ScenesManager& _scenes;
};

}
