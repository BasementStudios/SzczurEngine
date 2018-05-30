#include "NodeManager.hpp"

#include <algorithm>
#include <fstream>

<<<<<<< HEAD:src/Szczur/Modules/DialogEditor/NodeManager.cpp
#include <json.hpp>
#include <NodeEditor/NodeEditor.h>
=======
#include <Json/json.hpp>
>>>>>>> DialogEditor:src/Szczur/Modules/DialogEditor/NodeEditor/NodeManager.cpp

#include "Szczur/Utility/Logger.hpp"

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

bool NodeManager::read(const Json& j)
{
	auto readPin = [] (Json::reference j, NodePin* pin) 
	{
		pin->Id = j["id"];

		if (pin->Kind == ed::PinKind::Output)
		{
			if (j.find("target") != j.end())
			{
				const auto& target = j["target"];

				pin->OptionTarget.Id.Major = target["major"];
				pin->OptionTarget.Id.Minor = target["minor"];
			}

			//pin->OptionTarget.id = j["optionTarget"];
			pin->SkipOptions = j.find("skip") != j.end() ? j["skip"].get<bool>() : false;
		}

		pin->LinkToSameNode = j["linkToSameNode"];

		if (j.find("condition") != j.end())
		{
			auto& condition = j["condition"];

			pin->ConditionFunc = true;
			pin->ConditionFuncName = condition["name"].get<std::string>();
		}

		if (j.find("action") != j.end())
		{
			auto& action = j["action"];

			pin->ActionFunc = true;
			pin->ActionFuncName = action["name"].get<std::string>();
		}
	};


	if (j.find("lastId") == j.end() || j.find("nodes") == j.end() || j.find("links") == j.end())
	{
		LOG_INFO("Invalid file!");
		return false;
	}

	_lastId = j["lastId"];

	// Nodes
	Json::array_t JsonNodes = j["nodes"];

	for (auto& JsonNode : JsonNodes)
	{
		auto node = std::make_unique<Node>();
		node->Id = JsonNode["id"];
		node->Type = static_cast<Node::NodeType>(JsonNode["type"]);
		node->Name = JsonNode["name"].get<std::string>();

		if (node->Type == Node::NodeType::Options)
			node->NameId = JsonNode.find("nodeId") == JsonNode.end() ? node->Id : JsonNode["nodeId"].get<int>();

		node->_lastPinId = JsonNode["lastPinId"];

		ImVec2 pos;
		pos.x = JsonNode["position"][0];
		pos.y = JsonNode["position"][1];
		ed::SetNodePosition(node->Id, pos);

		// Inputs
		Json::array_t JsonInputs = JsonNode["inputs"];

		for (auto& JsonInput : JsonInputs)
		{
			auto input = std::make_unique<NodePin>();

			input->Node = node.get();
			input->Kind = ed::PinKind::Input;
			readPin(JsonInput, input.get());

			node->Inputs.push_back(std::move(input));
		}

		// Outputs
		Json::array_t JsonOutputs = JsonNode["outputs"];

		for (auto& JsonOutputs : JsonOutputs)
		{
			auto output = std::make_unique<NodePin>();

			output->Node = node.get();
			output->Kind = ed::PinKind::Output;

			readPin(JsonOutputs, output.get());

			node->Outputs.push_back(std::move(output));
		}

		_nodes.push_back(std::move(node));
	}

	// Links
	Json::array_t JsonLinks = j["links"];

	for (auto& JsonLink : JsonLinks)
	{
		auto link = std::make_unique<NodeLink>();

		link->Id = JsonLink["id"];
		link->StartPinId = JsonLink["startPinId"];
		link->EndPinId = JsonLink["endPinId"];
		link->SameNode = JsonLink["sameNode"];
		link->Color = ImGui::ColorConvertU32ToFloat4(JsonLink["color"]);

		_links.push_back(std::move(link));
	}

	return true;
}

void NodeManager::write(Json& j)
{
	auto writePin = [] (Json::object_t::mapped_type::reference j, NodePin* pin) {
		j["id"] = pin->Id;

		if (pin->Kind == ed::PinKind::Output)
		{
			if (pin->OptionTarget.Ptr != nullptr)
			{
				const auto& optionTargetId = pin->OptionTarget.Ptr;

				auto target = Json::object();

				target["major"] = optionTargetId->id;
				target["minor"] = optionTargetId->minorId;

				j["target"] = target;
			}

			j["skip"] = pin->SkipOptions;
		}

		j["linkToSameNode"] = pin->LinkToSameNode;


		if (pin->ConditionFunc)
		{
			j["condition"] = {
				{ "name", pin->ConditionFuncName }
			};
		}

		if (pin->ActionFunc)
		{
			j["action"] = {
				{ "name", pin->ActionFuncName }
			};
		}
	};

	j["lastId"] = _lastId;

	// Nodes
	auto JsonNodes = Json::array();

	for (auto& node : _nodes)
	{
		auto JsonNode = Json::object();

		JsonNode["id"] = node->Id;
		JsonNode["name"] = node->Name;

		if (node->Type == Node::NodeType::Options)
			JsonNode["nameId"] = node->NameId;
		
		JsonNode["type"] = node->Type;
		JsonNode["lastPinId"] = node->_lastPinId;
		
		auto pos = ed::GetNodePosition(node->Id);
		JsonNode["position"] = { pos.x, pos.y };


		// Inputs
		auto JsonInputs = Json::array();

		for (auto& input : node->Inputs)
		{
			auto JsonInput = Json::object();

			writePin(JsonInput, input.get());

			JsonInputs.push_back(JsonInput);
		}

		JsonNode["inputs"] = JsonInputs;


		// Outputs
		auto JsonOutputs = Json::array();

		for (auto& output : node->Outputs)
		{
			auto JsonOutput = Json::object();

			writePin(JsonOutput, output.get());

			JsonOutputs.push_back(JsonOutput);
		}

		JsonNode["outputs"] = JsonOutputs;


		JsonNodes.push_back(JsonNode);
	}

	j["nodes"] = JsonNodes;

	// Links
	auto JsonLinks = Json::array();

	for (auto& link : _links)
	{
		auto JsonLink = Json::object();
		JsonLink["id"] = link->Id;
		JsonLink["startPinId"] = link->StartPinId;
		JsonLink["endPinId"] = link->EndPinId;
		JsonLink["sameNode"] = link->SameNode;
		JsonLink["color"] = ImGui::ColorConvertFloat4ToU32(link->Color.Value);

		JsonLinks.push_back(JsonLink);
	}

	j["links"] = JsonLinks;
}

void NodeManager::reset()
{
	_nodes.clear();
	_links.clear();

	this->_lastId = 0;
}

NodePin* Node::createPin(ed::PinKind pinKind)
{
	auto newId = ++_lastPinId + Id * 100;

	auto pin = std::make_unique<NodePin>(newId, pinKind);

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
