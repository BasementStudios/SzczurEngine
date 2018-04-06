#include "NodeEditor.hpp"

#include <iostream>
#include <fstream>

#include <Shared/Math2D.h>
#include <Shared/Interop.h>

#include "Szczur/Config.hpp"
#include "Szczur/Utility/Logger.hpp"

namespace rat
{

NodeEditor::NodeEditor()
{
	_context = ed::CreateEditor(nullptr);

	_nodeManager = std::make_unique<NodeManager>();

	ed::SetCurrentEditor(_context);

	auto start = _nodeManager->createNode("Start", Node::Start);
	start->createPin("Start", ed::PinKind::Output);

	auto end = _nodeManager->createNode("End", Node::End);
	end->createPin("End", ed::PinKind::Input);

	ed::SetNodePosition(start->Id, ImVec2(100.f, 100.f));
	ed::SetNodePosition(end->Id, ImVec2(300, 100.f));

	ed::NavigateToContent();
}

NodeEditor::~NodeEditor()
{
	ed::DestroyEditor(_context);
}

void NodeEditor::drawIcon(bool filled, ImColor&& color)
{
	auto cursorPos = ImGui::GetCursorScreenPos();
	auto drawList = ImGui::GetWindowDrawList();

	auto a = cursorPos;
	auto b = a + ImVec2(24, 24);

	auto rect = ax::rect(to_point(a), to_point(b));
	const auto outline_scale = rect.w / 24.0f;
	const auto extra_segments = roundi(2 * outline_scale); // for full circle

	auto innerColor = ImColor(32, 32, 32, 255);

	const auto origin_scale = rect.w / 24.0f;

	const auto offset_x = 1.0f * origin_scale;
	const auto offset_y = 0.0f * origin_scale;
	const auto margin = (filled ? 2.0f : 2.0f) * origin_scale;
	const auto rounding = 0.1f * origin_scale;
	const auto tip_round = 0.7f; // percentage of triangle edge (for tip)
								 //const auto edge_round = 0.7f; // percentage of triangle edge (for corner)
	const auto canvas = ax::rectf(
		rect.x + margin + offset_x,
		rect.y + margin + offset_y,
		rect.w - margin * 2.0f,
		rect.h - margin * 2.0f);

	const auto left = canvas.x + canvas.w            * 0.5f * 0.3f;
	const auto right = canvas.x + canvas.w - canvas.w * 0.5f * 0.3f;
	const auto top = canvas.y + canvas.h            * 0.5f * 0.2f;
	const auto bottom = canvas.y + canvas.h - canvas.h * 0.5f * 0.2f;
	const auto center_y = (top + bottom) * 0.5f;
	//const auto angle = AX_PI * 0.5f * 0.5f * 0.5f;

	const auto tip_top = ImVec2(canvas.x + canvas.w * 0.5f, top);
	const auto tip_right = ImVec2(right, center_y);
	const auto tip_bottom = ImVec2(canvas.x + canvas.w * 0.5f, bottom);

	drawList->PathLineTo(ImVec2(left, top) + ImVec2(0, rounding));
	drawList->PathBezierCurveTo(
		ImVec2(left, top),
		ImVec2(left, top),
		ImVec2(left, top) + ImVec2(rounding, 0));
	drawList->PathLineTo(tip_top);
	drawList->PathLineTo(tip_top + (tip_right - tip_top) * tip_round);
	drawList->PathBezierCurveTo(
		tip_right,
		tip_right,
		tip_bottom + (tip_right - tip_bottom) * tip_round);
	drawList->PathLineTo(tip_bottom);
	drawList->PathLineTo(ImVec2(left, bottom) + ImVec2(rounding, 0));
	drawList->PathBezierCurveTo(
		ImVec2(left, bottom),
		ImVec2(left, bottom),
		ImVec2(left, bottom) - ImVec2(0, rounding));

	if (!filled)
	{
		if (innerColor & 0xFF000000)
			drawList->AddConvexPolyFilled(drawList->_Path.Data, drawList->_Path.Size, innerColor);

		drawList->PathStroke(color, true, 2.0f * outline_scale);
	}
	else
		drawList->PathFillConvex(color);
}

void NodeEditor::save(const std::string& fileName)
{
	auto code = generateCode();

	LOG_INFO("Saving code to '", fileName, "'...");

	std::ofstream file(fileName);

	if (file.good())
	{
		file << code;
		file.close();

		LOG_INFO("Saved!");
	}
	else
	{
		LOG_ERROR("Cannot save code to file!");
	}
}

std::string NodeEditor::generateCode()
{
	LOG_INFO("Generating code...");

	std::vector<std::string> optionsCode;

	for (int i = 2; i < _nodeManager->getNodes().size(); i++)
	{
		auto& node = _nodeManager->getNodes()[i];

		if (node->Type == Node::Options)
		{
			LOG_INFO("Generating ", node->Name.c_str(), "...");

			///// Runners
			std::string code = "options = dialog:newOption(";

			auto& in = node->Inputs.front();
			bool first = true;

			std::vector<int> runners;

			for (auto& link : _nodeManager->getLinks())
			{
				if (link->EndPinId == in->Id)
				{
					auto pin = _nodeManager->findPin(link->StartPinId);

					if (pin)
					{
						if (pin->OptionTarget < 1)
							continue;

						bool skip = false;

						// check if runner is already in
						for (auto runner : runners)
						{
							if (runner == pin->OptionTarget)
							{
								skip = true;
								break;
							}
						}

						if (skip)
							continue;

						if (first)
							first = false;
						else
							code += ", ";

						code += std::to_string(pin->OptionTarget);

						runners.push_back(pin->OptionTarget);
					}
				}
			}

			code += ")\n";

			////// Options
			for (auto& out : node->Outputs)
			{
				code += "options.add = {\n";

				// target
				code += "target = " + std::to_string(out->OptionTarget) + ";\n";

				// finishing
				for (auto& link : _nodeManager->getLinks())
				{
					if (link->StartPinId == out->Id)
					{
						auto endPin = _nodeManager->findPin(link->EndPinId);

						if (endPin && endPin->Node->Type == Node::End)
						{
							code += "finishing = true;\n";
							break;
						}
					}
				}

				code += "}\n";
			}

			optionsCode.push_back(code);
		}
	}

	auto& pinStartNode = _nodeManager->getNodes().front()->Outputs.front();

	LOG_INFO("Finalizing generating code...");

	// start with
	for (auto& link : _nodeManager->getLinks())
	{
		if (link->StartPinId == pinStartNode->Id)
		{
			std::string code = "dialog:startWith(" + std::to_string(pinStartNode->OptionTarget) + ")\n";

			optionsCode.push_back(code);
		}
	}

	std::string finalCode;

	for (auto& code : optionsCode)
	{
		finalCode += code + "\n";
	}

	return finalCode;
}

void NodeEditor::update()
{
	ed::SetCurrentEditor(_context);

	ed::Begin("Node Editor");
	{
		auto cursorTopLeft = ImGui::GetCursorScreenPos();

		// draw nodes
		for (auto& node : _nodeManager->getNodes())
		{
			ed::BeginNode(node->Id);
			{
				ImGui::Text(node->Name.c_str());

				ImGui::BeginGroup();

				// draw inputs
				for (auto& input : node->Inputs)
				{
					ed::PushStyleVar(ed::StyleVar_PivotAlignment, ImVec2(0, 0.5f));
					ed::PushStyleVar(ed::StyleVar_PivotSize, ImVec2(0, 0));

					ed::BeginPin(input->Id, ed::PinKind::Input);
					
					drawIcon(_nodeManager->isPinLinked(input->Id));

					ImGui::Dummy(ImVec2(24, 24));
					ImGui::SameLine();

					ImGui::BeginGroup();
					ImGui::Dummy(ImVec2(1.f, 0.f));
					ImGui::Text(input->Name.c_str());
					ImGui::EndGroup();


					ed::EndPin();

					ed::PopStyleVar(2);
				}

				ImGui::EndGroup();

				ImGui::SameLine();

				ImGui::BeginGroup();

				// draw outputs
				for (auto& output : node->Outputs)
				{
					ed::PushStyleVar(ed::StyleVar_PivotAlignment, ImVec2(1.0f, 0.5f));
					ed::PushStyleVar(ed::StyleVar_PivotSize, ImVec2(0, 0));

					ed::BeginPin(output->Id, ed::PinKind::Output);

					if (node->Type == Node::Options || node->Type == Node::Start)
					{
						ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));

						std::string label = (output->OptionTarget == -1 ? "SET" : std::to_string(output->OptionTarget)) + "##" + std::to_string(output->Id);

						if (ImGui::Button(label.c_str()))
						{
							ImGui::SetWindowFocus("Option config");

							_currentOption = output.get();
							_optionConfigWindow = true;
						}

						ImGui::PopStyleVar();
						ImGui::SameLine();
					}

					ImGui::BeginGroup();
					ImGui::Dummy(ImVec2(1.f, 0.f));
					ImGui::Text(output->Name.c_str());
					ImGui::EndGroup();

					ImGui::SameLine();

					if (output->LinkToSameNode)
						drawIcon(_nodeManager->isPinLinked(output->Id), ImColor(239, 83, 80));
					else
						drawIcon(_nodeManager->isPinLinked(output->Id));

					ImGui::Dummy(ImVec2(24, 24));

					ed::EndPin();

					ed::PopStyleVar(2);
				}

				if (node->Type == Node::Options)
				{
					std::string label = "Add option##" + std::to_string(node->Id);

					if (ImGui::Button(label.c_str()))
					{
						auto pin = node->createPin("Option", ed::PinKind::Output);
					}
				}

				ImGui::EndGroup();

			}
			ed::EndNode();
		}

		// draw links
		for (auto& link : _nodeManager->getLinks())
		{
			if (!link->SameNode)
				ed::Link(link->Id, link->StartPinId, link->EndPinId, link->Color, 2.0f);
		}

		// create link
		if (!creatingNode)
		{
			if (ed::BeginCreate(ImColor(255, 255, 255), 2.0f))
			{
				int startPinId = 0, endPinId = 0;

				if (ed::QueryNewLink(&startPinId, &endPinId))
				{
					auto startPin = _nodeManager->findPin(startPinId);
					auto endPin = _nodeManager->findPin(endPinId);

					_newLinkPin = startPin ? startPin : endPin;

					if (startPin->Kind == ed::PinKind::Input)
					{
						std::swap(startPin, endPin);
						std::swap(startPinId, endPinId);
					}

					if (startPin && endPin)
					{
						if (endPin == startPin)
						{
							ed::RejectNewItem(ImColor(255, 0, 0), 2.0f);
						}
						else if (endPin->Kind == startPin->Kind)
						{
							ed::RejectNewItem(ImColor(255, 0, 0), 2.0f);
						}
						else if (startPin->Node->Type == Node::Start && endPin->Node->Type == Node::End)
						{
							ed::RejectNewItem(ImColor(255, 0, 0), 1.0f);
						}
						else if (_nodeManager->isPinLinked(startPinId))
						{
							ed::RejectNewItem(ImColor(255, 0, 0), 1.0f);
						}
						else
						{
							if (ed::AcceptNewItem(ImColor(128, 255, 128), 4.0f))
							{
								auto link = _nodeManager->createLink(startPinId, endPinId);

								if (startPin->Node == endPin->Node)
								{
									link->SameNode = true;
									startPin->LinkToSameNode = true;
								}
								else
								{
									link->SameNode = false;
									startPin->LinkToSameNode = false;
								}
							}
						}
					}
				}
			}
			else
				_newLinkPin = nullptr;

			ed::EndCreate();
		}

		if (ed::BeginDelete())
		{
			// delete links
			int linkId = 0;
			while (ed::QueryDeletedLink(&linkId))
			{
				if (ed::AcceptDeletedItem())
				{
					auto link = _nodeManager->findLink(linkId);

					if (link)
					{
						auto startPin = _nodeManager->findPin(link->StartPinId);
						
						if (startPin)
						{
							startPin->LinkToSameNode = false;
						}
					}

					_nodeManager->removeLink(linkId);
				}
			}

			// delete nodes
			int nodeId = 0;
			while (ed::QueryDeletedNode(&nodeId))
			{
				auto node = _nodeManager->findNode(nodeId);

				if ((node->Type == Node::Options) && ed::AcceptDeletedItem())
				{
					_nodeManager->removeNode(nodeId);
				}
				else
				{
					ed::RejectDeletedItem();
				}
			}

			ed::EndDelete();
		}

		ImGui::SetCursorScreenPos(cursorTopLeft);
	}
	ed::End();

	if (ed::ShowNodeContextMenu(&_contextId))
		ImGui::OpenPopup("Node Context Menu");
	else if (ed::ShowLinkContextMenu(&_contextId))
		ImGui::OpenPopup("Link Context Menu");
	else if (ed::ShowPinContextMenu(&_contextId))
		ImGui::OpenPopup("Pin Context Menu");
	else if (ed::ShowBackgroundContextMenu())
		ImGui::OpenPopup("Background Context Menu");

	showPopups();
	showOptionConfig();

	if (ImGui::Begin("Main window", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		if (ImGui::Button("Generate code"))
		{
			save("code.lua");
		}
	}

	ImGui::End();
}

void NodeEditor::showPopups()
{
	if (ImGui::BeginPopup("Background Context Menu"))
	{
		if (ImGui::MenuItem("Create options"))
		{
			ImGui::EndPopup();
			ImGui::OpenPopup("Create Node Popup");
		}
		else
			ImGui::EndPopup();
	}

	if (ImGui::BeginPopup("Node Context Menu"))
	{
		if (ImGui::MenuItem("Delete"))
			ed::DeleteNode(_contextId);

		ImGui::EndPopup();
	}

	if (ImGui::BeginPopup("Link Context Menu"))
	{
		if (ImGui::MenuItem("Delete"))
			ed::DeleteLink(_contextId);

		ImGui::EndPopup();
	}

	if (ImGui::BeginPopup("Pin Context Menu"))
	{
		auto pin = _nodeManager->findPin(_contextId);

		if (ImGui::MenuItem("Disconnect"))
		{
			auto& links = _nodeManager->getLinks();

			for (int i = 0; i < links.size(); i++)
			{
				auto& link = links[i];

				if (link->StartPinId == pin->Id || link->EndPinId == pin->Id)
				{
					auto startPin = _nodeManager->findPin(link->StartPinId);

					if (startPin)
					{
						startPin->LinkToSameNode = false;
					}

					_nodeManager->removeLink(link->Id);
					ed::DeleteLink(link->Id);
					i--;
				}
			}
		}

		if (pin->Kind == ed::PinKind::Output && pin->Node->Type == Node::Options)
		{
			if (ImGui::MenuItem("Delete"))
			{
				if (pin->Kind == ed::PinKind::Output)
				{
					// remove links with this pin
					auto& links = _nodeManager->getLinks();

					for (int i = 0; i < links.size(); i++)
					{
						auto& link = links[i];

						if (link->StartPinId == pin->Id)
						{
							_nodeManager->removeLink(link->Id);

							i--;
						}
					}

					pin->Node->removePin(pin);
				}
			}
		}
		ImGui::EndPopup();
	}

	if (ImGui::BeginPopupModal("Create Node Popup", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		auto newNodePostion = ImGui::GetMousePosOnOpeningCurrentPopup();

		ImGui::Text("Create new node");
		ImGui::Separator();

		static char buffer[256];

		ImGui::InputText("Node name", buffer, 256);

		if (ImGui::Button("OK", ImVec2(120, 0))) 
		{
			auto node = _nodeManager->createNode(buffer);
			node->createPin("Trigger", ed::PinKind::Input);

			ed::SetNodePosition(node->Id, newNodePostion);

			ImGui::CloseCurrentPopup(); 
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel", ImVec2(120, 0))) 
		{ 
			ImGui::CloseCurrentPopup(); 
		}

		ImGui::EndPopup();
	}
}

void NodeEditor::showOptionConfig()
{
	if (_optionConfigWindow)
	{
		if (ImGui::Begin("Option config", &_optionConfigWindow, ImGuiWindowFlags_AlwaysAutoResize))
		{
			if (_currentOption)
			{
				ImGui::Text("Node: %s", _currentOption->Node->Name.c_str());

				static char buf[256] = { 0 };

				strcpy(buf, _currentOption->Name.c_str());

				if (ImGui::InputText("Name: ", buf, 256))
				{
					_currentOption->Name = buf;
				}

				ImGui::Separator();

				ImGui::Text("Set target dialog: ");
				ImGui::InputInt("Target dialog: ", &_currentOption->OptionTarget);
			}
		}
		ImGui::End();
	}
}

}
