#include "NodeEditor.hpp"

#include <iostream>

#include "Szczur/Config.hpp"

#include <Shared/Math2D.h>
#include <Shared/Interop.h>

namespace rat
{

NodeEditor::NodeEditor()
{
	ed::Config config;
	config.SettingsFile = "NodeEditor.json";
	_context = ed::CreateEditor(&config);

	_nodeManager = std::make_unique<NodeManager>();

	auto start = _nodeManager->createNode("Start", Node::Start);
	start->createPin("Start", ed::PinKind::Source);

	auto end = _nodeManager->createNode("End", Node::End);
	end->createPin("End", ed::PinKind::Target);

	/*for (int i = 0; i < 20; i++)
	{
		auto node = _nodeManager->createNode("Pole wyboru");

		node->createPin("Options in", ed::PinKind::Target);
		node->createPin("Option 1", ed::PinKind::Source);
		node->createPin("Option 2", ed::PinKind::Source);
		node->createPin("Option 3", ed::PinKind::Source);
	}*/
}

NodeEditor::~NodeEditor()
{
	ed::DestroyEditor(_context);
}


void NodeEditor::drawIcon(bool filled)
{
	auto cursorPos = ImGui::GetCursorScreenPos();
	auto drawList = ImGui::GetWindowDrawList();

	auto a = cursorPos;
	auto b = a + ImVec2(16, 16);

	auto rect = ax::rect(to_point(a), to_point(b));
	const auto outline_scale = rect.w / 24.0f;
	const auto extra_segments = roundi(2 * outline_scale); // for full circle

	auto innerColor = ImColor(32, 32, 32, 255);
	auto color = ImColor(255, 255, 255);

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

void NodeEditor::update()
{
	// ugly workaround ;/
	static bool openNewOptionPopup = false;

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

				if (node->Type == Node::Options)
				{
					ImGui::SameLine();

					ImGui::Button("Edit");
				}

				ImGui::BeginGroup();

				for (auto& input : node->Inputs)
				{
					ed::PushStyleVar(ed::StyleVar_PivotAlignment, ImVec2(0, 0.5f));
					ed::PushStyleVar(ed::StyleVar_PivotSize, ImVec2(0, 0));

					ed::BeginPin(input->Id, ed::PinKind::Target);
					
					drawIcon(_nodeManager->isPinLinked(input->Id));

					ImGui::Dummy(ImVec2(10, 16));
					ImGui::SameLine();

					ImGui::Text(input->Name.c_str());

					ed::EndPin();

					ed::PopStyleVar(2);
				}

				ImGui::EndGroup();

				ImGui::SameLine();

				ImGui::BeginGroup();

				for (auto& output : node->Outputs)
				{
					ed::PushStyleVar(ed::StyleVar_PivotAlignment, ImVec2(1.0f, 0.5f));
					ed::PushStyleVar(ed::StyleVar_PivotSize, ImVec2(0, 0));

					ed::BeginPin(output->Id, ed::PinKind::Source);

					ImGui::Text(output->Name.c_str());
					ImGui::SameLine();

					drawIcon(_nodeManager->isPinLinked(output->Id));

					ImGui::Dummy(ImVec2(16, 16));

					ed::EndPin();

					ed::PopStyleVar(2);
				}

				if (node->Type == Node::Options)
				{
					std::string label = "Add option##" + std::to_string(node->Id);

					if (ImGui::Button(label.c_str()))
					{
						_contextId = node->Id;

						openNewOptionPopup = true;
					}
				}

				ImGui::EndGroup();

			}
			ed::EndNode();
		}

		// draw links
		for (auto& link : _nodeManager->getLinks())
		{
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

					if (startPin->Kind == ed::PinKind::Target)
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
								_nodeManager->createLink(startPinId, endPinId);
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
					_nodeManager->removeLink(linkId);
				}
			}

			// delete nodes
			int nodeId = 0;
			while (ed::QueryDeletedNode(&nodeId))
			{
				auto node = _nodeManager->findNode(nodeId);

				if ((node->Type == Node::Options || node->Type == Node::Connector) && ed::AcceptDeletedItem())
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

	if (openNewOptionPopup)
	{
		ImGui::OpenPopup("Add Option Popup");
		openNewOptionPopup = false;
	}

	if (ed::ShowNodeContextMenu(&_contextId))
		ImGui::OpenPopup("Node Context Menu");
	else if (ed::ShowLinkContextMenu(&_contextId))
		ImGui::OpenPopup("Link Context Menu");
	else if (ed::ShowPinContextMenu(&_contextId))
		ImGui::OpenPopup("Pin Context Menu");
	else if (ed::ShowBackgroundContextMenu())
		ImGui::OpenPopup("Background Context Menu");

	showPopups();
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
		else if (ImGui::MenuItem("Create connector"))
		{
			auto node = _nodeManager->createNode("Connector", Node::Connector);
			node->createPin("In", ed::PinKind::Target);
			node->createPin("Out", ed::PinKind::Source);

			ed::SetNodePosition(node->Id, ImGui::GetMousePos());
			ImGui::EndPopup();
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
					_nodeManager->removeLink(link->Id);

					i--;
				}
			}
		}

		if (pin->Kind == ed::PinKind::Source && pin->Node->Type == Node::Options)
		{
			if (ImGui::MenuItem("Delete"))
			{
				if (pin->Kind == ed::PinKind::Source)
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
			node->createPin("Options in", ed::PinKind::Target);

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

	if (ImGui::BeginPopupModal("Add Option Popup", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		static int target = 0;

		ImGui::Text("Add option");
		ImGui::Separator();

		ImGui::InputInt("Options id", &target);

		if (ImGui::Button("OK", ImVec2(120, 0)))
		{
			auto node = _nodeManager->findNode(_contextId);

			node->createPin("Target " + std::to_string(target), ed::PinKind::Source);

			_contextId = 0;

			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			_contextId = 0;
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

}
