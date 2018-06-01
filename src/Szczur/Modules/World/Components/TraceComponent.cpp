#include "TraceComponent.hpp"

#include "Szczur/Modules/Trace/Trace.hpp"
#include "Szczur/Modules/Script/Script.hpp"

#include "Szczur/Modules/Trace/Trace.hpp"
#include "Szczur/Modules/Trace/Timeline.hpp"
#include "Szczur/Modules/Trace/Actions/AnimAction.hpp"
#include "Szczur/Modules/Trace/Actions/MoveAction.hpp"
#include "Szczur/Modules/Trace/Actions/WaitAction.hpp"

namespace rat
{

TraceComponent::TraceComponent(Entity* parent)
  : Component { parent, fnv1a_64("TraceComponent"), "TraceComponent", Component::Drawable}
{
	_trace = std::make_shared<Trace>();
}

void* TraceComponent::getFeature(Component::Feature_e feature)
{
	if (feature == Component::Drawable) 
		return static_cast<sf3d::Drawable*>(this);

	return nullptr;
}

const void* TraceComponent::getFeature(Component::Feature_e feature) const
{
	if (feature == Component::Drawable) 
		return static_cast<const sf3d::Drawable*>(this);

	return nullptr;
}

void TraceComponent::pause()
{
	_trace->pause();
}

void TraceComponent::resume()
{
	_trace->resume();
}

void TraceComponent::loadFromConfig(const Json& config)
{
	Component::loadFromConfig(config);
	
	_trace->loadFromConfig(config, this->getEntity());
}

void TraceComponent::saveToConfig(Json& config) const
{
	Component::saveToConfig(config);

	_trace->saveToConfig(config);
}

void TraceComponent::update(float deltaTime)
{
	if (_trace)
	{
		_trace->update(deltaTime);
	}
}

void TraceComponent::draw(sf3d::RenderTarget& target, sf3d::RenderStates states) const
{
	_trace->draw(target, states);
}

void TraceComponent::initScript(Script& script)
{
	auto object = script.newClass<TraceComponent>("TraceComponent", "World");
	object.set("pause", &TraceComponent::pause);
	object.set("resume", &TraceComponent::resume);
}

std::unique_ptr<Component> TraceComponent::copy(Entity* newParent) const
{
	auto ptr = std::make_unique<TraceComponent>(*this);

	ptr->setEntity(newParent);

	return ptr;
}

void TraceComponent::renderHeader(ScenesManager& scenes, Entity* object) {
    if (ImGui::CollapsingHeader("Trace##trance_component"))
    {
        if (ImGui::TreeNode("Traces"))
        {
            for (auto& timeline : getTrace()->getTimelines())
            {
                std::string id = std::to_string(reinterpret_cast<uintptr_t>(timeline.get()));

                if (ImGui::TreeNode(("Timeline##" + id).c_str()))
                {
                    ImGui::SameLine();

                    if (ImGui::SmallButton(("Use##" + id).c_str()))
                    {
                        getTrace()->setCurrentTimeline(timeline.get());
                    }

					ImGui::SameLine();

					if (ImGui::SmallButton(("Delete##" + id).c_str()))
					{
						getTrace()->removeTimeline(timeline.get());
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
											moveAction->Start = object->getPosition();
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
										moveAction->End = object->getPosition();
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
                                    strcpy(animName, animAction->AnimationName.c_str());

                                    if (ImGui::InputText(("Animation name##" + actionId).c_str(), animName, 64))
                                    {
                                        animAction->AnimationName = animName;
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
                            timeline->addAction(new MoveAction(object));
                        }
                        else if (currentAction == 1)
                        {
                            if (object->hasComponent<ArmatureComponent>())
                                timeline->addAction(new AnimAction(object));
                        }
						else if (currentAction == 2)
						{
							timeline->addAction(new WaitAction(object));
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
            getTrace()->addTimeline();
        }

        ImGui::SameLine();

        if (ImGui::Button("Stop##trace_component"))
        {
            getTrace()->setCurrentTimeline(nullptr);
        }

		
		if (ImGui::Button("Pause##trace_component"))
		{
			pause();
		}

		ImGui::SameLine();

		if (ImGui::Button("Resume##trace_component"))
		{
			resume();
		}
    }
}

}
