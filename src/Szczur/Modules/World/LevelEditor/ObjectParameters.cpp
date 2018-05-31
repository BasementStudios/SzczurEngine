#include "ObjectParameters.hpp"

#include <experimental/filesystem>

#include <ImGui/imgui.h>
#include <glm/glm.hpp>

#include "../ScenesManager.hpp"
#include "../Entity.hpp"
#include "../Scene.hpp"
#include "../Components/TraceComponent.hpp"
#include "../Components/SpriteComponent.hpp"
#include "../Components/ArmatureComponent.hpp"
#include "../Components/ScriptableComponent.hpp"
#include "../Components/InteractableComponent.hpp"
#include "../Components/TriggerComponent.hpp"

#include "Szczur/Modules/Trace/Trace.hpp"
#include "Szczur/Modules/Trace/Timeline.hpp"
#include "Szczur/Modules/Trace/Actions/AnimAction.hpp"
#include "Szczur/Modules/Trace/Actions/MoveAction.hpp"
#include "Szczur/Modules/Trace/Actions/WaitAction.hpp"

#include "Szczur/Utility/Convert/Windows1250.hpp"

namespace rat {
    ObjectParameters::ObjectParameters(ScenesManager& scenes) :
    _scenes{scenes} {

    }

    void ObjectParameters::render() {
        auto* scene = _scenes.getCurrentScene();
        if(scene->anySelected) {
            if(ImGui::Begin("Object Parameters", &scene->anySelected)) {
                Entity* focusedObject = scene->getEntity(scene->focusedObject);

                // Change components
                if(ImGui::Button("Change components...")) {
                    ImGui::OpenPopup("Change components...##modal");
                    ImGui::SetNextWindowSize(ImVec2(300,300));
                }
                _renderComponentsManager();

                if(focusedObject) {
                    if(ImGui::CollapsingHeader("Base", ImGuiTreeNodeFlags_DefaultOpen)) {
                        glm::vec3 position = focusedObject->getPosition();
                        glm::vec3 origin = focusedObject->getOrigin();
                        origin.y = -origin.y;
                        glm::vec3 rotation = focusedObject->getRotation();
                        glm::vec3 scale = focusedObject->getScale();
                        char name[255];
                        std::strcpy(&name[0], focusedObject->getName().c_str());
                        ImGui::InputText("", name, 255);
                        focusedObject->setName(name);
                        ImGui::DragFloat3("Position", reinterpret_cast<float*>(&position));
                        ImGui::DragFloat3("Origin", reinterpret_cast<float*>(&origin));
                        ImGui::DragFloat3("Rotation", reinterpret_cast<float*>(&rotation));
                        static bool lockRatio{false};
                        ImGui::DragFloat2("", reinterpret_cast<float*>(&scale), 0.01f);
                        ImGui::SameLine(0.f, 0.f);
                        ImGui::Checkbox("Scale##CheckboxLockRatio", &lockRatio);
                        //ImGui::Checkbox("Locked", &(_focusedObject->locked));
                        focusedObject->setPosition(position);
                        focusedObject->setOrigin(origin);
                        focusedObject->setRotation(rotation);
                        if(lockRatio == false) {
                            focusedObject->setScale(scale);
                        }
                        else {
                            float offset = (scale.x / focusedObject->getScale().x) + (scale.y / focusedObject->getScale().y) - 1;
                            focusedObject->scale( {offset, offset, 1.f} );
                        }
                    }

                    if(auto* object = focusedObject->getComponentAs<SpriteComponent>(); object != nullptr) {
                        if(ImGui::CollapsingHeader("Sprite Component")) {

                            // Load texture button
                            auto& spriteDisplayDataHolder = _scenes.getCurrentScene()->getSpriteDisplayDataHolder();
                            if(ImGui::Button("Load texture...##sprite_component")) {
                                std::string file = _scenes.getRelativePathFromExplorer("Select texture", ".\\Assets");
                                std::cout<<file<<std::endl;
                                if(file != "") {
                                    try {
                                        auto& it = spriteDisplayDataHolder.emplace_back(file);
                                        object->setSpriteDisplayData(&it);
                                    }
                                    catch(const std::exception& exc) {
                                        object->setSpriteDisplayData(nullptr);

                                        LOG_EXCEPTION(exc);
                                    }
                                }
                            }
                            ImGui::Text("Path:");
                            ImGui::SameLine();
                            ImGui::Text(object->getSpriteDisplayData() ? mapWindows1250ToUtf8(object->getSpriteDisplayData()->getName()).c_str() : "None");
                        }
                    }

                    if(auto* object = focusedObject->getComponentAs<CameraComponent>(); object != nullptr) {
                        if(ImGui::CollapsingHeader("Camera Component")) {
                            float velocity = object->getVelocity();
                            bool locked = object->getLock();
                            bool stickToPlayer = object->getStickToPlayer();
                            ImGui::DragFloat("Velocity", &velocity);
                            ImGui::Checkbox("Locked", &locked);
                            ImGui::Checkbox("Stick To Player", &stickToPlayer);

                            object->setVelocity(velocity);
                            object->setLock(locked);
                            object->setStickToPlayer(stickToPlayer);

                        }
                    }


                    if(auto* object = focusedObject->getComponentAs<ArmatureComponent>(); object != nullptr) {
                        if(ImGui::CollapsingHeader("Armature Component")) {

                            //Load armature button
                            auto& armatureDisplayDataHolder = _scenes.getArmatureDisplayDataHolder();
                            if(ImGui::Button("Load armature...##armature_component")) {
                                // std::string directory = _getRelativeDirectoryPathFromExplorer("Select armature folder", "Assets");
                                std::string directory = _scenes.getRelativePathFromExplorer("Select armature file", ".\\Assets");
                                directory = std::experimental::filesystem::path(directory).parent_path().string();
                                if(directory != "") {
                                    try {

                                        auto item = std::find_if(armatureDisplayDataHolder.begin(), armatureDisplayDataHolder.end(), [directory] (auto& item) { return item.getFolderPath() == directory; });

                                        ArmatureDisplayData* it = nullptr;

                                        if (item == armatureDisplayDataHolder.end())
                                        {
                                            // create new
                                            it = &armatureDisplayDataHolder.emplace_back(directory);
                                        }
                                        else
                                        {
                                            // use exisiting
                                            it = &(*item);
                                        }

                                        object->setArmatureDisplayData(it);
                                    }
                                    catch(const std::exception& exc) {
                                        object->setArmatureDisplayData(nullptr, false);

                                        LOG_EXCEPTION(exc);
                                    }
                                }
                            }
                            ImGui::Text("Path:");
                            ImGui::SameLine();
                            ImGui::Text(object->getArmatureDisplayData() ? mapWindows1250ToUtf8(object->getArmatureDisplayData()->getName()).c_str() : "None");

                            // Select animation button
                            if(auto* arm = object->getArmature(); arm && arm->getAnimation()) {
                                ImGui::Button("Select animation##armature_component");

                                ImGui::SetNextWindowSize({300.f, 200.f});
                                if(ImGui::BeginPopupContextItem(NULL, 0)) {
                                    auto names = arm->getAnimation()->getAnimationNames();
                                    for(auto& it : names) {
                                        ImGui::PushID(it.c_str());
                                        if(ImGui::Selectable(it.c_str())) {
                                            arm->getAnimation()->play(it);
                                        }
                                        ImGui::PopID();
                                    }
                                    ImGui::EndPopup();
                                }
                                if(arm->getAnimation()->isPlaying()) {
                                    ImGui::SameLine();
                                    if(ImGui::Button("Stop##armature_component")) {
                                        arm->getAnimation()->play(arm->getAnimation()->getLastAnimationName());
                                        arm->getAnimation()->stop(arm->getAnimation()->getLastAnimationName());
                                    }
                                }

                                ImGui::DragFloat("Animation speed##armature_component", &arm->getAnimation()->timeScale, 0.01f);
                            }


                        }
                    }
                    if(auto* object = focusedObject->getComponentAs<ScriptableComponent>(); object != nullptr) {
                        if(ImGui::CollapsingHeader("Scriptable Component")) {
                            static bool onceType = false;
                            if(ImGui::Button("Load")) {
                                std::string file = _scenes.getRelativePathFromExplorer("Select script file", ".\\Assets");

                                if(file != "") {
                                    try {
                                        if(onceType) {
                                            object->loadAnyScript(file);
                                        }
                                        else {
                                            object->loadScript(file);
                                        }
                                    }
                                    catch(const std::exception& exc) {
                                        LOG_EXCEPTION(exc);
                                    }
                                }
                            }
                            if(object->getFilePath()!="") {
                                ImGui::SameLine();
                                if(ImGui::Button("Reload##scriptable_component")) {
                                    try {
                                        object->reloadScript();
                                    }
                                    catch (const std::exception& exc)
                                    {
                                        LOG_EXCEPTION(exc);
                                    }
                                }
                                ImGui::SameLine();
                                if(ImGui::Button("Remove##scriptable_component")) {
                                    object->loadScript("");
                                }
                            }
                            ImGui::Text("Path:");
                            ImGui::SameLine();
                            ImGui::Text(object->getFilePath()!="" ? mapWindows1250ToUtf8(object->getFilePath()).c_str() : "None");

                            ImGui::Checkbox("Once type##scriptable_component", &onceType);
                        }
                    }

                    if(auto* object = focusedObject->getComponentAs<InteractableComponent>(); object != nullptr) {
                        if(ImGui::CollapsingHeader("Interactable Component")) {
                            float distance = object->getDistance();
                            float height = object->getHeight();
                            ImGui::DragFloat("Distance", &distance);
                            ImGui::DragFloat("Height", &height);
                            object->setDistance(distance);
                            object->setHeight(height);

                        }
                    }
                    if (auto* object = focusedObject->getComponentAs<TriggerComponent>(); object != nullptr)
                    {
                        if (ImGui::CollapsingHeader("Trigger Component"))
                        {
                            float radius = object->getRadius();
                            if (ImGui::BeginCombo("Types", TriggerComponent::enumToString(object->type).c_str()))
                            {
                                if (ImGui::Selectable(TriggerComponent::enumToString(TriggerComponent::None).c_str(), object->type == TriggerComponent::None))
                                {
                                    object->type = TriggerComponent::None;
                                }
                                if (
                                    ImGui::Selectable(TriggerComponent::enumToString(
                                        TriggerComponent::ChangeScene).c_str(),
                                        object->type == TriggerComponent::ChangeScene)
                                    )
                                {
                                    object->type = TriggerComponent::ChangeScene;
                                    object->sceneId = 0u;
                                    object->entranceId = 0u;
                                }
                                ImGui::EndCombo();
                            }
                            ImGui::DragFloat("Radius", &radius);
                            object->setRadius(radius);
                            if (object->type == TriggerComponent::ChangeScene)
                            {
                                auto* temp = _scenes.getScene(object->sceneId);
                                if (ImGui::BeginCombo("Scene", temp ? temp->getName().c_str() : "None"))
                                {
                                    for (auto& it : _scenes.getScenes())
                                    {
                                        if (ImGui::Selectable(it->getName().c_str(), object->sceneId == it->getID()))
                                        {
                                            object->sceneId = it->getID();
                                        }
                                    }

                                    ImGui::EndCombo();
                                }
                                if (object->sceneId)
                                {
                                    auto& holder = _scenes.getScene(object->sceneId)->getEntrances();
                                    std::string name = "None";
                                    for (auto& it : holder)
                                    {
                                        if (it.ID == object->entranceId)
                                        {
                                            name = it.name;
                                            break;
                                        }
                                    }
                                    if (ImGui::BeginCombo("Entrance", name.c_str()))
                                    {
                                        for (auto& it : holder)
                                        {
                                            if (ImGui::Selectable(it.name.c_str(), it.ID == object->entranceId))
                                            {
                                                object->entranceId = it.ID;
                                            }
                                        }

                                        ImGui::EndCombo();
                                    }
                                }
                            }
                        }
                    }

                    if (auto* object = focusedObject->getComponentAs<TraceComponent>(); object != nullptr)
                    {
                        if (ImGui::CollapsingHeader("Trace Component"))
                        {
                            if (ImGui::TreeNode("Traces"))
                            {
                                for (auto& timeline : object->getTrace()->getTimelines())
                                {
                                    std::string id = std::to_string(reinterpret_cast<uintptr_t>(timeline.get()));

                                    if (ImGui::TreeNode(("Timeline " + std::to_string(timeline->getId())).c_str()))
                                    {
                                        ImGui::SameLine();

                                        if (ImGui::SmallButton(("Use##" + id).c_str()))
                                        {
                                            object->getTrace()->setCurrentTimeline(timeline.get());
                                        }

										ImGui::SameLine();

										if (ImGui::SmallButton(("Delete##" + id).c_str()))
										{
											object->getTrace()->removeTimeline(timeline.get());
											ImGui::TreePop();
											break;
										}

                                        ImGui::Checkbox("Loop", &timeline->Loop);
                                        ImGui::Checkbox("Show lines in editor", &timeline->ShowLines);

                                        ImGui::Spacing();

										auto& actions = timeline->getActions();

										for (int i = 0; i < actions.size(); i++)
                                        {
											auto& action = actions[i];

                                            std::string actionId = std::to_string(reinterpret_cast<uintptr_t>(action.get()));

											if (ImGui::SmallButton(("U##" + actionId).c_str()))
											{
												if (i > 0)
												{
													std::swap(actions[i], actions[i - 1]);
												}
											}

											ImGui::SameLine();

											if (ImGui::SmallButton(("D##" + actionId).c_str()))
											{
												if (i < actions.size() - 1)
												{
													std::swap(actions[i], actions[i + 1]);
												}
											}

											ImGui::SameLine();
											
											if (ImGui::SmallButton(("X##" + actionId).c_str()))
											{
												timeline->removeAction(action.get());
												break;
											}

											ImGui::SameLine();

                                            switch (action->getType())
                                            {
                                                case Action::Move:
                                                {
                                                    auto moveAction = static_cast<MoveAction*>(action.get());

                                                    if (ImGui::TreeNode(("Move##" + actionId).c_str()))
                                                    {
                                                        ImGui::Checkbox("Use current position as start position", &moveAction->UseCurrentPosition);

														if (!moveAction->UseCurrentPosition)
														{
															if (ImGui::Button(("C##Start" + actionId).c_str()))
															{
																moveAction->Start = focusedObject->getPosition();
															}

															if (ImGui::IsItemHovered())
															{
																ImGui::BeginTooltip();
																ImGui::Text("Set current position");
																ImGui::EndTooltip();
															}

															ImGui::SameLine();

															ImGui::DragFloat3(("Start##" + actionId).c_str(), reinterpret_cast<float*>(&moveAction->Start));
														}

														ImGui::Spacing();
														ImGui::Checkbox(("Relative to Start##" + actionId).c_str(), &moveAction->EndRelativeToStart);

														if (ImGui::Button(("C##End" + actionId).c_str()))
														{
															moveAction->End = focusedObject->getPosition();
														}

														if (ImGui::IsItemHovered())
														{
															ImGui::BeginTooltip();
															ImGui::Text("Set current position");
															ImGui::EndTooltip();
														}

														ImGui::SameLine();

                                                        ImGui::DragFloat3(("End##" + actionId).c_str(), reinterpret_cast<float*>(&moveAction->End));
														ImGui::Spacing();

														ImGui::DragFloat(("Speed##" + actionId).c_str(), &moveAction->Speed, 0.01f, 0.f, 50.f);
                                                        ImGui::Checkbox(("Teleport##" + actionId).c_str(), &moveAction->Teleport);

                                                        ImGui::TreePop();
                                                    }
                                                } break;
                                                case Action::Anim:
                                                {
                                                    auto animAction = static_cast<AnimAction*>(action.get());

                                                    if (ImGui::TreeNode(("Anim##" + actionId).c_str()))
                                                    {
                                                        static char animName[64] = { 0 };
                                                        
														if (ImGui::BeginCombo("Animation", animAction->AnimationName.c_str()))
														{
															if (auto arm = focusedObject->getComponentAs<ArmatureComponent>())
															{
																for (auto& anim : arm->getArmature()->getAnimation()->getAnimationNames())
																{
																	bool isSelected = (animAction->AnimationName == anim);

																	if (ImGui::Selectable(anim.c_str(), isSelected))
																		animAction->AnimationName = anim;
																	else
																		ImGui::SetItemDefaultFocus();

																}
															}


															ImGui::EndCombo();
														}

                                                        ImGui::DragFloat(("Fade in time##" + actionId).c_str(), &animAction->FadeInTime, 0.01f, 0.01f, 1.f);

                                                        ImGui::Checkbox(("Play once##" + actionId).c_str(), &animAction->PlayOnce);

                                                        if (animAction->PlayOnce)
                                                            ImGui::Checkbox(("Wait to the end of animation##" + actionId).c_str(), &animAction->WaitToEnd);

                                                        ImGui::Checkbox(("Flip X##" + actionId).c_str(), &animAction->FlipX);

                                                        ImGui::TreePop();
                                                    }
                                                } break;
												case Action::Wait:
												{
													auto waitAction = static_cast<WaitAction*>(action.get());

													if (ImGui::TreeNode(("Wait##" + actionId).c_str()))
													{
														ImGui::DragFloat(("Time to wait##" + actionId).c_str(), &waitAction->TimeToWait, 0.1f, 0.1f, 60.f);
														ImGui::TreePop();
													}
												} break;
                                            }

                                        }

                                        static int currentAction = -1;

                                        ImGui::PushItemWidth(100.f);
                                        ImGui::Combo(("##Action" + id).c_str(), &currentAction, "Move\0Anim\0Wait\0");

                                        ImGui::SameLine();

                                        if (ImGui::Button(("Add action##" + id).c_str()))
                                        {
                                            if (currentAction == 0)
                                            {
                                                timeline->addAction(new MoveAction(focusedObject));
                                            }
                                            else if (currentAction == 1)
                                            {
                                                if (focusedObject->hasComponent<ArmatureComponent>())
                                                    timeline->addAction(new AnimAction(focusedObject));
                                            }
											else if (currentAction == 2)
											{
												timeline->addAction(new WaitAction(focusedObject));
											}
                                            
                                        }

                                        ImGui::TreePop();

										ImGui::Separator();
                                    }
                                }
                                ImGui::TreePop();
                            }

                            ImGui::Separator();

                            if (ImGui::Button("Add timeline"))
                            {
                                object->getTrace()->addTimeline();
                            }

                            ImGui::SameLine();

                            if (ImGui::Button("Stop##trace_component"))
                            {
                                object->getTrace()->setCurrentTimeline(nullptr);
                            }

							
							if (ImGui::Button("Pause##trace_component"))
							{
								object->pause();
							}

							ImGui::SameLine();

							if (ImGui::Button("Resume##trace_component"))
							{
								object->resume();
							}
                        }


                    }
                }
            }
            ImGui::End();
        }
    }

    void ObjectParameters::_renderComponentsManager() {
		if (ImGui::BeginPopupModal("Change components...##modal", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)) {

			auto* focusedObject = _scenes.getCurrentScene()->getEntity(_scenes.getCurrentScene()->focusedObject);

			static bool selectedComponents[7]{};
			if(ImGui::IsWindowAppearing()) {
				selectedComponents[0] = focusedObject->hasComponent<SpriteComponent>();
				selectedComponents[1] = focusedObject->hasComponent<ArmatureComponent>();
				selectedComponents[2] = focusedObject->hasComponent<ScriptableComponent>();                
				selectedComponents[3] = focusedObject->hasComponent<InteractableComponent>();
				selectedComponents[4] = focusedObject->hasComponent<TriggerComponent>();
				selectedComponents[5] = focusedObject->hasComponent<TraceComponent>();
			}
			if(ImGui::Button("Accept", ImVec2(70,0))) {

				if(focusedObject->hasComponent<SpriteComponent>()!=selectedComponents[0]) {
					if(selectedComponents[0]) focusedObject->addComponent<SpriteComponent>();
					else focusedObject->removeComponent<SpriteComponent>();
				}
				if(focusedObject->hasComponent<ArmatureComponent>()!=selectedComponents[1]) {
					if(selectedComponents[1]) focusedObject->addComponent<ArmatureComponent>();
					else focusedObject->removeComponent<ArmatureComponent>();
				}
				if(focusedObject->hasComponent<ScriptableComponent>()!=selectedComponents[2]) {
					if(selectedComponents[2]) focusedObject->addComponent<ScriptableComponent>();
					else focusedObject->removeComponent<ScriptableComponent>();
				}
				if(focusedObject->hasComponent<InteractableComponent>()!=selectedComponents[3]) {
					if(selectedComponents[3]) focusedObject->addComponent<InteractableComponent>();
					else focusedObject->removeComponent<InteractableComponent>();
				}
				if(focusedObject->hasComponent<TriggerComponent>()!=selectedComponents[4]) {
					if(selectedComponents[4]) focusedObject->addComponent<TriggerComponent>();
					else focusedObject->removeComponent<TriggerComponent>();
				}
				if(focusedObject->hasComponent<TraceComponent>()!=selectedComponents[5]) {
					if(selectedComponents[5]) focusedObject->addComponent<TraceComponent>();
					else focusedObject->removeComponent<TraceComponent>();
				}

				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if(ImGui::Button("Close", ImVec2(70,0))) {
				ImGui::CloseCurrentPopup();
			}

			ImGui::Checkbox("Sprite##components_manager", &selectedComponents[0]);
			ImGui::Checkbox("Armature##components_manager", &selectedComponents[1]);
			ImGui::Checkbox("Scriptable##components_manager", &selectedComponents[2]);
			ImGui::Checkbox("InteractableComponent##components_manager", &selectedComponents[3]);
			ImGui::Checkbox("TriggerComponent##components_manager", &selectedComponents[4]);
			ImGui::Checkbox("TraceComponent##components_manager", &selectedComponents[5]);

			ImGui::EndPopup();
		}
	}
}
