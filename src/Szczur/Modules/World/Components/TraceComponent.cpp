#include "TraceComponent.hpp"

#include "Szczur/Modules/Script/Script.hpp"

#include "Trace/Trace.hpp"
#include "Trace/Timeline.hpp"
#include "Trace/Actions/AnimAction.hpp"
#include "Trace/Actions/MoveAction.hpp"
#include "Trace/Actions/WaitAction.hpp"
#include "Trace/Actions/ScriptAction.hpp"

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
	static Timeline* currentTimeline = nullptr;
	static Action* currentAction = nullptr;

	if (ImGui::CollapsingHeader("Trace##trance_component"))
	{
		ImGui::Text("Timeline: %d\n", _trace->getCurrentTimeline() ? _trace->getCurrentTimeline()->getId() : -1);

		ImGui::Separator();

		if (ImGui::Button("+"))
		{
			_trace->addTimeline();
		}

		ImGui::SameLine();

		auto playStopButtonText = _trace->getCurrentTimeline() == nullptr ? "Play" : "Stop";

		if (ImGui::Button(playStopButtonText))
		{
			if (currentTimeline)
			{
				if (_trace->getCurrentTimeline() == nullptr)
					_trace->setCurrentTimeline(currentTimeline);
				else
					_trace->setCurrentTimeline(nullptr);
			}
		}

		ImGui::SameLine();

		auto pauseResumeButtonText = _trace->isPlaying() ? "Pause" : "Resume";

		if (ImGui::Button(pauseResumeButtonText))
		{
			if (_trace->isPlaying())
			{
				_trace->pause();
			}
			else
			{
				_trace->resume();
			}
		}

		ImGui::SameLine();

		if (ImGui::Button("-"))
		{
			if (currentTimeline)
			{
				_trace->removeTimeline(currentTimeline);
				currentTimeline = nullptr;
			}
		}

		auto& style = ImGui::GetStyle();

		// Show all timelines
		ImGui::BeginChild("Timelines", ImVec2(0.f, ImGui::GetFrameHeightWithSpacing() + 12.f), true);
		{
			for (auto& timeline : _trace->getTimelines())
			{
				ImGui::PushID(timeline.get());

				bool activeTimeline = timeline.get() == currentTimeline;

				if (activeTimeline)
				{
					ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_HeaderHovered]);
					ImGui::PushStyleColor(ImGuiCol_Border, ImGui::GetStyle().Colors[ImGuiCol_SeparatorHovered]);
				}

				if (ImGui::Button(std::to_string(timeline->getId()).c_str(), { ImGui::GetFrameHeight(), 0.f }))
				{
					if (currentTimeline == timeline.get())
						currentTimeline = nullptr;
					else
						currentTimeline = timeline.get();
				}

				if (activeTimeline)
				{
					ImGui::PopStyleColor(2);
				}

				ImGui::SameLine();

				ImGui::PopID();
			}
		}
		ImGui::EndChild();

		// Show current timeline
		if (currentTimeline)
		{
			ImGui::Checkbox("Loop", &currentTimeline->Loop);
			ImGui::Checkbox("Show lines in editor", &currentTimeline->ShowLines);

			ImGui::PushItemWidth(100.f);
			ImGui::DragFloat("Speed multiplier", &currentTimeline->SpeedMultiplier, 0.01f, 0.f, 100.f);
			ImGui::PopItemWidth();
			ImGui::Spacing();

			auto& actions = currentTimeline->getActions();

			if (ImGui::Button("+Move"))
			{
				currentTimeline->addAction(new MoveAction(object));
			}

			ImGui::SameLine();

			if (ImGui::Button("+Anim"))
			{
				if (object->hasComponent<ArmatureComponent>())
					currentTimeline->addAction(new AnimAction(object));
			}

			ImGui::SameLine();

			if (ImGui::Button("+Wait"))
			{
				currentTimeline->addAction(new WaitAction(object));
			}

			ImGui::SameLine();

			if (ImGui::Button("+Script"))
			{
				if (object->hasComponent<ScriptableComponent>())
					currentTimeline->addAction(new ScriptAction(object));
			}

			style.ScrollbarSize = 10;

			ImGui::BeginChild("Actions", ImVec2(0.f, ImGui::GetFrameHeightWithSpacing() + style.ScrollbarSize + 12.f), true, ImGuiWindowFlags_HorizontalScrollbar);
			{
				for (auto& action : actions)
				{
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
						case Action::Script:
							buttonName = "S";
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

					if (ImGui::BeginPopupContextItem("item context menu"))
					{
						if (ImGui::Selectable("Remove"))
						{
							currentTimeline->removeAction(action.get());
							ImGui::EndPopup();
							ImGui::PopID();
							break;
						}

						ImGui::Separator();

						if (ImGui::Selectable("Move to left"))
						{
							for (int i = 0; i < actions.size(); ++i)
							{
								if (i > 0 && actions[i].get() == action.get())
								{
									std::swap(actions[i], actions[i - 1]);
									break;
								}
							}
						}

						if (ImGui::Selectable("Move to right"))
						{
							for (int i = 0; i < actions.size(); ++i)
							{
								if (i < actions.size() - 1 && actions[i].get() == action.get())
								{
									std::swap(actions[i], actions[i + 1]);
									break;
								}
							}
						}

						ImGui::EndPopup();
					}


					ImGui::PopID();

					ImGui::SameLine();
				}
			}
			ImGui::EndChild();

			style.ScrollbarSize = 20;

			if (currentAction)
			{
				ImGui::Indent(ImGui::GetStyle().IndentSpacing);

				switch (currentAction->getType())
				{
					case Action::Move:
					{
						auto moveAction = static_cast<MoveAction*>(currentAction);

						ImGui::PushItemWidth(-80);

						ImGui::Text("Action: Move");
						ImGui::Separator();

						ImGui::Text("Start position");

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

						ImGui::Text("End position");

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

						ImGui::PopItemWidth();
					} break;
					case Action::Anim:
					{
						auto animAction = static_cast<AnimAction*>(currentAction);

						ImGui::Text("Action: Anim");
						ImGui::Separator();

						static char animName[64] = { 0 };

						if (ImGui::BeginCombo("Animation", animAction->AnimationName.c_str()))
						{
							if (auto arm = object->getComponentAs<ArmatureComponent>(); arm && arm->getArmature())
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

						ImGui::Text("Action: Wait");
						ImGui::Separator();

						ImGui::DragFloat("Time to wait", &waitAction->TimeToWait, 0.1f, 0.1f, 60.f);
					} break;
					case Action::Script:
					{
						auto scriptAction = static_cast<ScriptAction*>(currentAction);

						ImGui::Text("Action: Script");
						ImGui::Separator();

						if (ImGui::Button("Set script file path"))
						{
							// Path to script
							std::string file = scenes.getRelativePathFromExplorer("Select script file", ".\\Assets", "Lua (*.lua)|*.lua");

							// Loading script
							if (file != "")
							{
								scriptAction->ScriptFilePath = file;
							}
						}

						ImGui::Text("Path: %s", scriptAction->ScriptFilePath.empty() ? "None" : scriptAction->ScriptFilePath.c_str());
					} break;
				}

				ImGui::Unindent(ImGui::GetStyle().IndentSpacing);
			}
		}
	}
}

}
