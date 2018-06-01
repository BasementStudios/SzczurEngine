#include "ArmatureComponent.hpp"

#include <experimental/filesystem>

#include "Szczur/Modules/DragonBones/SF3DFactory.hpp"

#include "../Entity.hpp"
#include "../Scene.hpp"
#include "../ScenesManager.hpp"

#include "Szczur/Modules/Script/Script.hpp"
#include "Szczur/Utility/Convert/Windows1250.hpp"

namespace rat
{

    ArmatureComponent::ArmatureComponent(Entity* parent) :
        Component(parent, fnv1a_64("ArmatureComponent"), "ArmatureComponent", Component::Drawable)
    {

    }

    ArmatureComponent::~ArmatureComponent()
    {
        if(_armature)
            delete _armature;
    }

    void ArmatureComponent::setArmatureDisplayData(ArmatureDisplayData* armatureDisplayData, bool deleteOld)
    {
        _armatureDisplayData = armatureDisplayData;
        if(armatureDisplayData) {
            auto dbFactory = dragonBones::SF3DFactory::get();

            if(_armature && deleteOld)
				delete _armature;

            _armature = dbFactory->buildArmatureDisplay(armatureDisplayData->getName());
        }
        else if(_armature) {
			if (deleteOld)
				delete _armature;

            _armature = nullptr;
        }
    }

    ArmatureDisplayData* ArmatureComponent::getArmatureDisplayData() {
        return _armatureDisplayData;
    }

    const ArmatureDisplayData* ArmatureComponent::getArmatureDisplayData() const {
        return _armatureDisplayData;
    }

    dragonBones::SF3DArmatureDisplay* ArmatureComponent::getArmature() {
        return _armature;
    }

    std::unique_ptr<Component> ArmatureComponent::copy(Entity* newParent) const {
        auto ptr = std::make_unique<ArmatureComponent>(*this);
		
		// prevent deleting
		ptr->_armature = nullptr;
		ptr->setArmatureDisplayData(this->_armatureDisplayData);

        ptr->setEntity(newParent);

        return ptr;
    }

    void* ArmatureComponent::getFeature(Component::Feature_e feature) {
        if (feature == Component::Drawable) return static_cast<sf3d::Drawable*>(this);

        return nullptr;
    }

    const void* ArmatureComponent::getFeature(Component::Feature_e feature) const {
        if (feature == Component::Drawable) return static_cast<const sf3d::Drawable*>(this);

        return nullptr;
    }
    void ArmatureComponent::loadFromConfig(Json& config) {
        Component::loadFromConfig(config);
		auto& armatureDisplayDataHolder = getEntity()->getScene()->getScenes()->getArmatureDisplayDataHolder();
		auto name = mapUtf8ToWindows1250(config["armatureDisplayData"].get<std::string>());
		if(name != "") {
			bool found{false};
			for(auto& it : armatureDisplayDataHolder) {
				if(name == it.getName()) {
					setArmatureDisplayData(&it);
					found = true;
				}
			}
			if(!found) {
				try {
					setArmatureDisplayData(&(armatureDisplayDataHolder.emplace_back("Assets/Armatures/"+name)));
				}
				catch(const std::exception& exc) {

				}
			}
		}
    }

    void ArmatureComponent::saveToConfig(Json& config) const {
        Component::saveToConfig(config);
		config["armatureDisplayData"] = _armatureDisplayData ? mapWindows1250ToUtf8(_armatureDisplayData->getName()) : "";
    }

    void ArmatureComponent::draw(sf3d::RenderTarget& target, sf3d::RenderStates states) const
    {
        if(_armature) {
            states.transform *= getEntity()->getTransform();
            _armature->draw(target, states);
        }
    }

	void ArmatureComponent::playAnim(const std::string& animationName, int playTimes)
	{
		if (_armature)
		{
			_armature->getAnimation()->play(animationName, playTimes);
		}
	}

	void ArmatureComponent::fadeIn(const std::string& animationName, float fadeInTime, int playTimes)
	{
		if (_armature)
		{
			_armature->getAnimation()->fadeIn(animationName, fadeInTime, playTimes);
		}
	}

	void ArmatureComponent::setFlipX(bool flipX)
	{
		if (_armature)
		{
			_armature->getArmature()->setFlipX(flipX);
		}
	}

	void ArmatureComponent::setSpeed(float speed)
	{
		if (_armature)
		{
			_armature->getAnimation()->timeScale = speed;
		}
	}

	void ArmatureComponent::initScript(Script& script)
	{
		auto object = script.newClass<ArmatureComponent>("ArmatureComponent", "World");
		object.set("play", &ArmatureComponent::playAnim);
		object.set("fadeIn", &ArmatureComponent::fadeIn);
		object.set("setFlipX", &ArmatureComponent::setFlipX);
		object.set("setSpeed", &ArmatureComponent::setSpeed);
		object.init();

	}

	void ArmatureComponent::renderHeader(ScenesManager& scenes, Entity* object) {
		if(ImGui::CollapsingHeader("Armature##armature_component")) {

            // Armature data holder
            auto& armatures = scenes.getArmatureDisplayDataHolder();

            // Load armature button
            if(ImGui::Button("Load armature...##armature_component")) {

            	// Path to direcotry with armature files
                std::string directory = scenes.getRelativePathFromExplorer("Select armature file", ".\\Assets");
                directory = std::experimental::filesystem::path(directory).parent_path().string();

                // Load armature form directory
                if(directory != "") {
                    try {

                        auto armature = std::find_if(armatures.begin(), armatures.end(), [directory] (auto& armature) { return armature.getFolderPath() == directory; });

                        ArmatureDisplayData* it = nullptr;
                        if (armature == armatures.end())
                        {
                            // Create new
                            it = &armatures.emplace_back(directory);
                        }
                        else
                        {
                            // Use exisiting
                            it = &(*armature);
                        }

                        setArmatureDisplayData(it);
                    }
                    catch(const std::exception& exc) {
                        setArmatureDisplayData(nullptr, false);

                        LOG_EXCEPTION(exc);
                    }
                }
            }

            // Show path to armature directory
            ImGui::Text("Path:");
            ImGui::SameLine();
            ImGui::Text(getArmatureDisplayData() ? mapWindows1250ToUtf8(getArmatureDisplayData()->getName()).c_str() : "None");

			// Select animation button
			if (auto* arm = getArmature(); arm && arm->getAnimation())
			{
				if (ImGui::BeginCombo("Animation", arm->getAnimation()->getLastAnimationName().c_str()))
				{
					for (auto& anim : arm->getArmature()->getAnimation()->getAnimationNames())
					{
						bool isSelected = (arm->getAnimation()->getLastAnimationName() == anim);

						if (ImGui::Selectable(anim.c_str(), isSelected))
							arm->getAnimation()->play(anim);
						else
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				if (arm->getAnimation()->isPlaying())
				{
					ImGui::SameLine();
					if (ImGui::Button("Stop##armature_component"))
					{
						arm->getAnimation()->stop(arm->getAnimation()->getLastAnimationName());
						arm->getAnimation()->reset();
					}
				}

				ImGui::DragFloat("Animation speed##armature_component", &arm->getAnimation()->timeScale, 0.01f);
			}
        }
	}

}
