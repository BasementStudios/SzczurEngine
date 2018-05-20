#pragma once

#include <memory>

#include "Szczur/Utility/SFML3D/Drawable.hpp"
#include "Szczur/Modules/DragonBones/SF3DArmatureDisplay.hpp"

#include "Szczur/Modules/World/Component.hpp"
#include "Szczur/Modules/World/Data/ArmatureDisplayData.hpp"

namespace rat
{
	class Script;

	class ArmatureComponent : public Component, public sf3d::Drawable
    {
    private:
        dragonBones::SF3DArmatureDisplay* _armature = nullptr;
        ArmatureDisplayData* _armatureDisplayData = nullptr;

    public:

        ArmatureComponent(Entity* parent);

        ~ArmatureComponent();

        void setArmatureDisplayData(ArmatureDisplayData* armatureDisplayData);

        ArmatureDisplayData* getArmatureDisplayData();

        const ArmatureDisplayData* getArmatureDisplayData() const;

        dragonBones::SF3DArmatureDisplay* getArmature();

        virtual std::unique_ptr<Component> copy(Entity* newParent) const;

        virtual void* getFeature(Component::Feature_e feature) override;

        virtual const void* getFeature(Component::Feature_e feature) const override;

        virtual void loadFromConfig(const Json& config);
        virtual void saveToConfig(Json& config) const;

        void draw(sf3d::RenderTarget& target, sf3d::RenderStates states) const override;

		// Scripts
		void playAnim(const std::string& animationName, int playTimes = -1);
		void fadeIn(const std::string& animationName, float fadeInTime = -1.f, int playTimes = -1);
		void setFlipX(bool flipX);
		void setSpeed(float speed);

		static void initScript(Script& script);
    };

}
