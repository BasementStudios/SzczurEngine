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
  : Component { parent, fnv1a_64("TraceComponent"), "TraceComponent" }
{
	_trace = std::make_shared<Trace>();
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

void TraceComponent::update(ScenesManager& scenes, float deltaTime)
{
	if (_trace)
	{
		_trace->update(deltaTime);
	}
}

void TraceComponent::render(sf3d::RenderTarget& target)
{
	_trace->draw(target, sf3d::RenderStates());
}

void TraceComponent::initScript(Script& script)
{
	auto object = script.newClass<TraceComponent>("TraceComponent", "World");
	object.set("pause", &TraceComponent::pause);
	object.set("resume", &TraceComponent::resume);
	object.set("setTimeline", &TraceComponent::setTimeline);
	object.set("stop", &TraceComponent::stop);
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
	static Action* currentAction = nullptr;

	if (ImGui::CollapsingHeader("Trace##trance_component"))
	{
		if (ImGui::Button("Add timeline"))
		{
			_trace->addTimeline();
		}

		for (auto& timeline : _trace->getTimelines())
		{
			if (ImGui::TreeNodeEx(("Timeline " + std::to_string(timeline->getId())).c_str(), ImGuiTreeNodeFlags_FramePadding))
			{
				ImGui::SameLine();

				ImGui::PushID(timeline.get());

				if (ImGui::SmallButton("Play"))
				{
					_trace->setCurrentTimeline(timeline.get());
				}

				ImGui::SameLine();

				if (ImGui::SmallButton("-"))
				{
					_trace->removeTimeline(timeline.get());
					ImGui::PopID();
					ImGui::TreePop();
					break;
				}

				ImGui::Checkbox("Loop", &timeline->Loop);
				ImGui::Checkbox("Show lines in editor", &timeline->ShowLines);

				ImGui::Spacing();

				auto& actions = timeline->getActions();

				// Select list actions to add
				static int currentComboItem = -1;

				ImGui::PushItemWidth(100.f);
				ImGui::Combo("##Action", &currentComboItem, "Move\0Anim\0Wait\0");

				ImGui::SameLine();

				if (ImGui::Button("Add action"))
				{
					if (currentComboItem == 0)
					{
						timeline->addAction(new MoveAction(object));
					}
					else if (currentComboItem == 1)
					{
						if (object->hasComponent<ArmatureComponent>())
							timeline->addAction(new AnimAction(object));
					}
					else if (currentComboItem == 2)
					{
						timeline->addAction(new WaitAction(object));
					}
				}

				auto& style = ImGui::GetStyle();

				ImGui::BeginChild(timeline.get(), ImVec2(0.f, ImGui::GetFrameHeightWithSpacing() + style.ScrollbarSize + 12.f),  true, ImGuiWindowFlags_AlwaysHorizontalScrollbar);
				{
					for (int i = 0; i < actions.size(); ++i)
					{
						auto& action = actions[i];

						std::string buttonName;

						switch (action->getType())
						{
							case Action::Move:
								buttonName = "M";
								break;
							case Action::Anim:
								buttonName = "A";
								break;
							case Action::Wait:
								buttonName = "W";
								break;
						}

						ImGui::PushID(action.get());

						bool active = action.get() == currentAction;

						if (active)
						{
							ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_HeaderHovered]);
							ImGui::PushStyleColor(ImGuiCol_Border, ImGui::GetStyle().Colors[ImGuiCol_SeparatorHovered]);
						}
							
						if (ImGui::Button((buttonName).c_str(), { ImGui::GetFrameHeight(), 0.f }))
						{

							if (currentAction)
								currentAction->Color = glm::vec3(1.f, 1.f, 1.f);

							if (currentAction == action.get())
								currentAction = nullptr;
							else
							{
								currentAction = action.get();
								currentAction->Color = glm::vec3(0.f, 1.f, 0.f);
							}
						}

						if (active)
						{
							ImGui::PopStyleColor(2);
						}

						ImGui::PopID();

						ImGui::SameLine();
					}

					ImGui::SetScrollX(0.5f);
				}
				ImGui::EndChild();

				if (currentAction)
				{
					ImGui::Separator();

					if (ImGui::Button("<<", { 30.f, 0.f }))
					{
						for (int i = 0; i < actions.size(); ++i)
						{
							if (i > 0 && actions[i].get() == currentAction)
							{
								std::swap(actions[i], actions[i - 1]);
								break;
							}
						}
					}

					ImGui::SameLine();

					if (ImGui::Button("X", { 30.f, 0.f }))
					{
						timeline->removeAction(currentAction);
						currentAction = nullptr;
						ImGui::PopID();
						ImGui::TreePop();
						break;
					}

					ImGui::SameLine();

					if (ImGui::Button(">>", { 30.f, 0.f }))
					{
						for (int i = 0; i < actions.size(); ++i)
						{
							if (i < actions.size() - 1 && actions[i].get() == currentAction)
							{
								std::swap(actions[i], actions[i + 1]);
								break;
							}
						}
					}

					switch (currentAction->getType())
					{
						case Action::Move:
						{
							auto moveAction = static_cast<MoveAction*>(currentAction);

							ImGui::Text("Move");
							ImGui::Spacing();

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

							ImGui::PushItemWidth(-60.f);

							ImGui::DragFloat3("End", reinterpret_cast<float*>(&moveAction->End));
							//ImGui::Spacing();

							ImGui::DragFloat("Speed", &moveAction->Speed, 0.01f, 0.f, 50.f);

							ImGui::PopItemWidth();

							ImGui::Checkbox("Teleport", &moveAction->Teleport);
						} break;
						case Action::Anim:
						{
							auto animAction = static_cast<AnimAction*>(currentAction);

							ImGui::Text("Anim");
							ImGui::Spacing();

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
						} break;
						case Action::Wait:
						{
							auto waitAction = static_cast<WaitAction*>(currentAction);

							ImGui::Text("Wait");
							ImGui::Spacing();

							ImGui::DragFloat("Time to wait", &waitAction->TimeToWait, 0.1f, 0.1f, 60.f);
						} break;
					}

					ImGui::Separator();
				}

				ImGui::PopID();
				ImGui::TreePop();
				ImGui::Separator();
			}
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
