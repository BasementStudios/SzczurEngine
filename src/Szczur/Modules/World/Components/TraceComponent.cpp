#include "TraceComponent.hpp"

#include "Szczur/Modules/Script/Script.hpp"

#include "Trace/Trace.hpp"
#include "Trace/Timeline.hpp"
#include "Trace/Actions/AnimAction.hpp"
#include "Trace/Actions/MoveAction.hpp"
#include "Trace/Actions/WaitAction.hpp"

#include "../ScenesManager.hpp"

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

void TraceComponent::setTimeline(int id)
{
	auto& timelines = _trace->getTimelines();

	auto timeline = std::find_if(timelines.begin(), timelines.end(), [id] (auto& timeline) { return id == timeline->getId(); });

	if (*timeline)
		_trace->setCurrentTimeline((*timeline).get());
}

void TraceComponent::stop()
{
	_trace->setCurrentTimeline(nullptr);
}

void TraceComponent::loadFromConfig(Json& config)
{
	Component::loadFromConfig(config);
	
	_trace->loadFromConfig(config, this->getEntity());
}

void TraceComponent::saveToConfig(Json& config) const
{
	Component::saveToConfig(config);

	_trace->saveToConfig(config);
}

void TraceComponent::update(ScenesManager& scenes, float deltaTime)
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

void TraceComponent::initScript(ScriptClass<Entity>& entity, Script& script)
{
	auto object = script.newClass<TraceComponent>("TraceComponent", "World");

	// Main
	object.set("pause", &TraceComponent::pause);
	object.set("resume", &TraceComponent::resume);
	object.set("setTimeline", &TraceComponent::setTimeline);
	object.set("stop", &TraceComponent::stop);
	object.set("getEntity", sol::resolve<Entity*()>(&Component::getEntity));

	// Entity
	entity.set("addTraceComponent", [&](Entity& e){return (TraceComponent*)e.addComponent<TraceComponent>();});
	entity.set("trace", &Entity::getComponentAs<TraceComponent>);

	object.init();
}

std::unique_ptr<Component> TraceComponent::copy(Entity* newParent) const
{
	auto ptr = std::make_unique<TraceComponent>(*this);

	ptr->setEntity(newParent);

	return ptr;
}

void TraceComponent::renderHeader(ScenesManager& scenes, Entity* object) 
{
	if (ImGui::CollapsingHeader("Trace##trance_component"))
	{
		if (ImGui::TreeNode("Traces"))
		{
			for (auto& timeline : _trace->getTimelines())
			{
				if (ImGui::TreeNode(("Timeline " + std::to_string(timeline->getId())).c_str()))
				{
					ImGui::SameLine();

					ImGui::PushID(timeline.get());

					if (ImGui::SmallButton("Use"))
					{
						_trace->setCurrentTimeline(timeline.get());
					}

					ImGui::SameLine();

					if (ImGui::SmallButton("Delete"))
					{
						_trace->removeTimeline(timeline.get());
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

						ImGui::PushID(action.get());

						if (ImGui::SmallButton("U"))
						{
							if (i > 0)
							{
								std::swap(actions[i], actions[i - 1]);
							}
						}

						ImGui::SameLine();

						if (ImGui::SmallButton("D"))
						{
							if (i < actions.size() - 1)
							{
								std::swap(actions[i], actions[i + 1]);
							}
						}

						ImGui::SameLine();

						if (ImGui::SmallButton("X"))
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

								if (ImGui::TreeNode("Move"))
								{
									ImGui::Checkbox("Use current position as start position", &moveAction->UseCurrentPosition);

									if (!moveAction->UseCurrentPosition)
									{
										if (ImGui::Button("C##Start"))
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

										ImGui::DragFloat3("Start##", reinterpret_cast<float*>(&moveAction->Start));
									}

									ImGui::Spacing();
									ImGui::Checkbox("Relative to Start", &moveAction->EndRelativeToStart);

									if (ImGui::Button("C##End"))
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

									ImGui::DragFloat3("End", reinterpret_cast<float*>(&moveAction->End));
									ImGui::Spacing();

									ImGui::DragFloat("Speed", &moveAction->Speed, 0.01f, 0.f, 50.f);
									ImGui::Checkbox("Teleport", &moveAction->Teleport);

									ImGui::TreePop();
								}
							} break;
							case Action::Anim:
							{
								auto animAction = static_cast<AnimAction*>(action.get());

								if (ImGui::TreeNode("Anim"))
								{
									static char animName[64] = { 0 };

									if (ImGui::BeginCombo("Animation", animAction->AnimationName.c_str()))
									{
										if (auto arm = object->getComponentAs<ArmatureComponent>())
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

									ImGui::DragFloat("Fade in time", &animAction->FadeInTime, 0.01f, 0.01f, 1.f);

									ImGui::Checkbox("Play once", &animAction->PlayOnce);

									if (animAction->PlayOnce)
										ImGui::Checkbox("Wait to the end of animation", &animAction->WaitToEnd);

									ImGui::Checkbox("Flip X", &animAction->FlipX);

									ImGui::TreePop();
								}
							} break;
							case Action::Wait:
							{
								auto waitAction = static_cast<WaitAction*>(action.get());

								if (ImGui::TreeNode("Wait"))
								{
									ImGui::DragFloat("Time to wait", &waitAction->TimeToWait, 0.1f, 0.1f, 60.f);
									ImGui::TreePop();
								}
							} break;
						}

						ImGui::PopID();
					}

					static int currentAction = -1;

					ImGui::PushItemWidth(100.f);
					ImGui::Combo("##Action", &currentAction, "Move\0Anim\0Wait\0");

					ImGui::SameLine();

					if (ImGui::Button("Add action"))
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

					ImGui::PopID();
					ImGui::TreePop();
					ImGui::Separator();
				}
			}
			ImGui::TreePop();
		}

		ImGui::Separator();

		if (ImGui::Button("Add timeline"))
		{
			_trace->addTimeline();
		}

		ImGui::Separator();

		ImGui::Text("Current timeline: %d\n", _trace->getCurrentTimeline() ? _trace->getCurrentTimeline()->getId() : -1);

		if (ImGui::Button("Stop"))
		{
			_trace->setCurrentTimeline(nullptr);
		}

		ImGui::SameLine();

		if (ImGui::Button("Pause"))
		{
			pause();
		}

		ImGui::SameLine();

		if (ImGui::Button("Resume"))
		{
			resume();
		}
	}
}

}
