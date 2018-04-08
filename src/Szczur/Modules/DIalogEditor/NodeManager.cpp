#include "NodeManager.hpp"

#include <algorithm>
#include <fstream>

#include <nlohmann/json.hpp>
#include <NodeEditor.h>

#include "Szczur/Utility/Logger.hpp"

using json = nlohmann::json;

namespace rat
{

NodePin* NodeManager::findPin(int pinId)
{
	if (pinId <= 0)
		return nullptr;

	for (auto& node : _nodes)
	{
		for (auto& pin : node->Inputs)
		{
			if (pin->Id == pinId)
				return pin.get();
		}

		for (auto& pin : node->Outputs)
		{
			if (pin->Id == pinId)
				return pin.get();
		}
	}

	return nullptr;
}

Node* NodeManager::createNode(const std::string& name, Node::NodeType type)
{
	auto node = std::make_unique<Node>(GetNewId(), name.c_str(), type);

	auto ptr = node.get();

	_nodes.push_back(std::move(node));

	return ptr;
}

Node* NodeManager::findNode(int nodeId)
{
	for (auto& node : _nodes)
	{
		if (node->Id == nodeId)
			return node.get();
	}

	return nullptr;
}

void NodeManager::removeNode(int nodeId)
{
	auto id = std::find_if(_nodes.begin(), _nodes.end(), [nodeId] (auto& node) { return node->Id == nodeId; });

	if (id != _nodes.end())
	{
		_nodes.erase(id);
	}
}

NodeLink* NodeManager::createLink(int startPinId, int endPinId)
{
	auto link = std::make_unique<NodeLink>(GetNewId(), startPinId, endPinId);
	link->Color = ImVec4(255, 255, 255, 255);

	auto ptr = link.get();

	_links.push_back(std::move(link));

	return ptr;
}

bool NodeManager::isPinLinked(int pinId)
{
	if (pinId <= 0)
		return false;

	for (auto& link : _links)
	{
		if (link->StartPinId == pinId || link->EndPinId == pinId)
			return true;
	}

	return false;
}

NodeLink* NodeManager::findLink(int linkId)
{
	for (auto& link : _links)
	{
		if (link->Id == linkId)
			return link.get();
	}

	return nullptr;
}

void NodeManager::removeLink(int linkId)
{
	auto id = std::find_if(_links.begin(), _links.end(), [linkId] (auto& link) { return link->Id == linkId; });

	if (id != _links.end())
	{
		_links.erase(id);
	}
}

bool NodeManager::read(const json& j)
{
	auto readPin = [] (json::reference j, NodePin* pin) 
	{
		pin->Id = j["id"];
		pin->Name = j["name"].get<std::string>();
		pin->OptionTarget = j["optionTarget"];
		pin->LinkToSameNode = j["linkToSameNode"];
	};


	if (j.find("lastId") == j.end() || j.find("nodes") == j.end() || j.find("links") == j.end())
	{
		LOG_INFO("Invalid file!");
		return false;
	}

	_lastId = j["lastId"];

	// Nodes
	json::array_t jsonNodes = j["nodes"];

	for (auto& jsonNode : jsonNodes)
	{
		auto node = std::make_unique<Node>();
		node->Id = jsonNode["id"];
		node->Name = jsonNode["name"].get<std::string>();
		node->Type = static_cast<Node::NodeType>(jsonNode["type"]);

		node->_lastPinId = jsonNode["lastPinId"];

		ImVec2 pos;
		pos.x = jsonNode["position"][0];
		pos.y = jsonNode["position"][1];
		ed::SetNodePosition(node->Id, pos);

		// Inputs
		json::array_t jsonInputs = jsonNode["inputs"];

		for (auto& jsonInput : jsonInputs)
		{
			auto input = std::make_unique<NodePin>();

			readPin(jsonInput, input.get());

			input->Node = node.get();
			input->Kind = ed::PinKind::Input;

			node->Inputs.push_back(std::move(input));
		}

		// Outputs
		json::array_t jsonOutputs = jsonNode["outputs"];

		for (auto& jsonOutputs : jsonOutputs)
		{
			auto output = std::make_unique<NodePin>();

			readPin(jsonOutputs, output.get());

			output->Node = node.get();
			output->Kind = ed::PinKind::Output;

			node->Outputs.push_back(std::move(output));
		}

		_nodes.push_back(std::move(node));
	}

	// Links
	json::array_t jsonLinks = j["links"];

	for (auto& jsonLink : jsonLinks)
	{
		auto link = std::make_unique<NodeLink>();

		link->Id = jsonLink["id"];
		link->StartPinId = jsonLink["startPinId"];
		link->EndPinId = jsonLink["endPinId"];
		link->SameNode = jsonLink["sameNode"];
		link->Color = ImGui::ColorConvertU32ToFloat4(jsonLink["color"]);

		_links.push_back(std::move(link));
	}

	return true;
}

void NodeManager::write(json& j)
{
	auto writePin = [] (json::object_t::mapped_type::reference j, NodePin* pin) {
		j["id"] = pin->Id;
		j["name"] = pin->Name;
		j["optionTarget"] = pin->OptionTarget;
		j["linkToSameNode"] = pin->LinkToSameNode;
	};

	j["lastId"] = _lastId;

	// Nodes
	auto jsonNodes = json::array();

	for (auto& node : _nodes)
	{
		auto jsonNode = json::object();

		jsonNode["id"] = node->Id;
		jsonNode["name"] = node->Name;
		jsonNode["type"] = node->Type;
		jsonNode["lastPinId"] = node->_lastPinId;
		
		auto pos = ed::GetNodePosition(node->Id);
		jsonNode["position"] = { pos.x, pos.y };


		// Inputs
		auto jsonInputs = json::array();

		for (auto& input : node->Inputs)
		{
			auto jsonInput = json::object();

			writePin(jsonInput, input.get());

			jsonInputs.push_back(jsonInput);
		}

		jsonNode["inputs"] = jsonInputs;


		// Outputs
		auto jsonOutputs = json::array();

		for (auto& output : node->Outputs)
		{
			auto jsonOutput = json::object();

			writePin(jsonOutput, output.get());

			jsonOutputs.push_back(jsonOutput);
		}

		jsonNode["outputs"] = jsonOutputs;


		jsonNodes.push_back(jsonNode);
	}

	j["nodes"] = jsonNodes;

	// Links
	auto jsonLinks = json::array();

	for (auto& link : _links)
	{
		auto jsonLink = json::object();
		jsonLink["id"] = link->Id;
		jsonLink["startPinId"] = link->StartPinId;
		jsonLink["endPinId"] = link->EndPinId;
		jsonLink["sameNode"] = link->SameNode;
		jsonLink["color"] = ImGui::ColorConvertFloat4ToU32(link->Color.Value);

		jsonLinks.push_back(jsonLink);
	}

	j["links"] = jsonLinks;
}

void NodeManager::reset()
{
	_nodes.clear();
	_links.clear();

	this->_lastId = 0;
}

NodePin* Node::createPin(const std::string& name, ed::PinKind pinKind)
{
	auto newId = ++_lastPinId + Id * 100;;

	auto pin = std::make_unique<NodePin>(newId, name, pinKind);

	pin->Node = this;

	auto ptr = pin.get();

	if (pinKind == ed::PinKind::Input)
	{
		Inputs.push_back(std::move(pin));
	}
	else if (pinKind == ed::PinKind::Output)
	{
		Outputs.push_back(std::move(pin));
	}

	return ptr;
}

void Node::removePin(NodePin* pin)
{
	if (pin->Kind == ed::PinKind::Input)
	{
		auto id = std::find_if(Inputs.begin(), Inputs.end(), [pin] (auto& node) { return node->Id == pin->Id; });

		if (id != Inputs.end())
		{
			Inputs.erase(id);
		}
	}
	else if (pin->Kind == ed::PinKind::Output)
	{
		auto id = std::find_if(Outputs.begin(), Outputs.end(), [pin] (auto& node) { return node->Id == pin->Id; });

		if (id != Outputs.end())
		{
			Outputs.erase(id);
		}
	}
}

}
