#include "QuestNodeEditor.hpp"

#include <NodeEditor/Shared/Math2D.h>
#include <NodeEditor/Shared/Interop.h>

#include "Szczur/Utility/Logger.hpp"

namespace rat
{

QuestNodeEditor::QuestNodeEditor()
{
	_context = ed::CreateEditor(nullptr);

	_nodeManager = std::make_unique<QuestNodeManager>();

	ed::NavigateToContent(); //traitor
}

QuestNodeEditor::~QuestNodeEditor()
{
	ed::DestroyEditor(_context);
}

void QuestNodeEditor::update()
{
	ed::SetCurrentEditor(_context);

	ed::Begin("Node Editor");
	{
		auto cursorTopLeft = ImGui::GetCursorScreenPos();

		// draw nodes
		for (auto& node : _nodeManager->getNodes())
		{
			drawNode(node.get());
		}

		// draw links
		for (auto& link : _nodeManager->getLinks())
		{
			ed::Link(link->Id, link->StartPinId, link->EndPinId, link->Color, 2.f);
		}

		creatingLinks();
		deleting();

		ImGui::SetCursorScreenPos(cursorTopLeft);
	}
	ed::End();

	showPopups();
}

void QuestNodeEditor::drawNode(Quest::Node* node)
{
	ed::BeginNode(node->Id);
	{
		ImGui::BeginGroup();
		ImGui::Text(node->Name.c_str());

		ImGui::SameLine();

		ImGui::BeginGroup();
		{
			ed::PushStyleVar(ed::StyleVar_PivotAlignment, ImVec2(1.0f, 0.5f));
			ed::PushStyleVar(ed::StyleVar_PivotSize, ImVec2(0, 0));

			ed::BeginPin(node->Output->Id, ed::PinKind::Output);

			ImGui::Text("Output");

			ImGui::SameLine();
			drawIcon(_nodeManager->isPinLinked(node->Output->Id));
			ImGui::Dummy(ImVec2(16, 16));
			ed::EndPin();

			ed::PopStyleVar(2);
		}
		ImGui::EndGroup();
		ImGui::EndGroup();

		ImGui::BeginGroup();

		// draw inputs
		for (auto& input : node->Inputs)
		{
			ed::PushStyleVar(ed::StyleVar_PivotAlignment, ImVec2(0, 0.5f));
			ed::PushStyleVar(ed::StyleVar_PivotSize, ImVec2(0, 0));

			ed::BeginPin(input->Id, ed::PinKind::Input);

			drawIcon(_nodeManager->isPinLinked(input->Id));

			ImGui::Dummy(ImVec2(16, 16));
			ImGui::SameLine();

			ImGui::Text("Input");

			ed::EndPin();

			ed::PopStyleVar(2);
		}

		ImGui::EndGroup();

		ImGui::SameLine();

		ImGui::BeginGroup();

		for (auto& output : node->Outputs)
		{
			if (output->IsInHeader)
				continue;

			ed::PushStyleVar(ed::StyleVar_PivotAlignment, ImVec2(1.0f, 0.5f));
			ed::PushStyleVar(ed::StyleVar_PivotSize, ImVec2(0, 0));

			ed::BeginPin(output->Id, ed::PinKind::Output);

			ImGui::Text("Output");

			ImGui::SameLine();
			drawIcon(_nodeManager->isPinLinked(output->Id));
			ImGui::Dummy(ImVec2(16, 16));
			ed::EndPin();

			ed::PopStyleVar(2);
		}

		ImGui::EndGroup();
	}
	ed::EndNode();
}


void QuestNodeEditor::showPopups()
{
	if (ed::ShowNodeContextMenu(&_contextId))
		ImGui::OpenPopup("Node Context Menu");
	else if (ed::ShowLinkContextMenu(&_contextId))
		ImGui::OpenPopup("Link Context Menu");
	else if (ed::ShowPinContextMenu(&_contextId))
		ImGui::OpenPopup("Pin Context Menu");
	else if (ed::ShowBackgroundContextMenu())
	{
		if (ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows))
		{
			ImGui::OpenPopup("Background Context Menu");
		}
	}

	if (ImGui::BeginPopup("Background Context Menu"))
	{
		if (ImGui::MenuItem("Create"))
		{
			auto node = _nodeManager->createNode("Quest");
			node->createPin(ed::PinKind::Input);
			node->createPin(ed::PinKind::Output);

			ed::SetNodePosition(node->Id, ed::ScreenToCanvas(ImGui::GetMousePos()));
		}

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

					ed::DeleteLink(link->Id);
				}
			}
		}

		ImGui::EndPopup();
	}

}

void QuestNodeEditor::creatingLinks()
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
				else if (endPin->Node == startPin->Node)
				{
					ed::RejectNewItem(ImColor(255, 0, 0), 2.0f);
				}
				else if (endPin->Kind == startPin->Kind)
				{
					ed::RejectNewItem(ImColor(255, 0, 0), 2.0f);
				}
				/*else if (!startPin->IsInHeader && _nodeManager->isPinLinked(startPinId))
				{
					ed::RejectNewItem(ImColor(255, 0, 0), 1.0f);
				}*/
				else
				{
					if (ed::AcceptNewItem(ImColor(128, 255, 128), 4.0f))
					{
						auto link = _nodeManager->createLink(startPinId, endPinId);
					}
				}
			}
		}
	}

	ed::EndCreate();
}

void QuestNodeEditor::deleting()
{
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
				}

				_nodeManager->removeLink(linkId);
			}
		}

		// delete nodes
		int nodeId = 0;
		while (ed::QueryDeletedNode(&nodeId))
		{
			auto node = _nodeManager->findNode(nodeId);

			if (node != nullptr && ed::AcceptDeletedItem())
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

}

void QuestNodeEditor::drawIcon(bool filled, ImColor&& color)
{
	auto cursorPos = ImGui::GetCursorScreenPos();
	auto drawList = ImGui::GetWindowDrawList();

	auto a = cursorPos;
	auto b = a + ImVec2(16, 16);

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

}
