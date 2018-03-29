#include "NodeManager.hpp"

#include <algorithm>

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


void Node::createPin(const std::string& name, ed::PinKind pinKind)
{
	auto newId = (Inputs.size() + Outputs.size() + 1) + Id * 100;

	auto pin = std::make_unique<NodePin>(static_cast<int>(newId), name, pinKind);

	pin->Node = this;

	if (pinKind == ed::PinKind::Target)
	{
		Inputs.push_back(std::move(pin));
	}
	else if (pinKind == ed::PinKind::Source)
	{
		Outputs.push_back(std::move(pin));
	}
}

}
