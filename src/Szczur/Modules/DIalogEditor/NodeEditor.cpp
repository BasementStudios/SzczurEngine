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

		node->createPin("Dialog in", ed::PinKind::Target);
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

				if (node->Type == Node::Dialog)
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

				if (node->Type == Node::Dialog)
				{
					if (ImGui::Button("Add option"))
					{

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
						else if (endPin->Node == startPin->Node)
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

				if (node->Type == Node::Start || node->Type == Node::End)
				{
					std::cout << "Woww" << std::endl;

					ed::RejectDeletedItem();
				}
				else if (ed::AcceptDeletedItem())
				{
					_nodeManager->removeNode(nodeId);
				}

			}

			ed::EndDelete();
		}

		ImGui::SetCursorScreenPos(cursorTopLeft);
	}

	ed::End();
}

}
