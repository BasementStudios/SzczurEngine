#include "QuestNodeManager.hpp"

#include <algorithm>

namespace rat
{

Quest::NodePin* QuestNodeManager::findPin(int pinId)
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

Quest::Node* QuestNodeManager::createNode(const std::string& name)
{
	auto node = std::make_unique<Quest::Node>(GetNewId(), name.c_str());
	
	auto newId = ++node->_lastPinId + node->Id * 100;
	auto pin = std::make_unique<Quest::NodePin>(newId, ed::PinKind::Output);
	pin->IsInHeader = true;
	pin->Node = node.get();
	node->Output = pin.get();
	node->Outputs.push_back(std::move(pin));

	auto ptr = node.get();
	_nodes.push_back(std::move(node));
	return ptr;
}

Quest::Node* QuestNodeManager::findNode(int nodeId)
{
	for (auto& node : _nodes)
	{
		if (node->Id == nodeId)
			return node.get();
	}

	return nullptr;
}

void QuestNodeManager::removeNode(int nodeId)
{
	auto id = std::find_if(_nodes.begin(), _nodes.end(), [nodeId] (auto& node) { return node->Id == nodeId; });

	if (id != _nodes.end())
	{
		_nodes.erase(id);
	}
}

Quest::NodeLink* QuestNodeManager::createLink(int startPinId, int endPinId)
{
	auto link = std::make_unique<Quest::NodeLink>(GetNewId(), startPinId, endPinId);
	link->Color = ImVec4(255, 255, 255, 255);

	auto ptr = link.get();

	_links.push_back(std::move(link));

	return ptr;
}

bool QuestNodeManager::isPinLinked(int pinId)
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

Quest::NodeLink* QuestNodeManager::findLink(int linkId)
{
	for (auto& link : _links)
	{
		if (link->Id == linkId)
			return link.get();
	}

	return nullptr;
}

void QuestNodeManager::removeLink(int linkId)
{
	auto id = std::find_if(_links.begin(), _links.end(), [linkId] (auto& link) { return link->Id == linkId; });

	if (id != _links.end())
	{
		_links.erase(id);
	}
}

Quest::NodePin* Quest::Node::createPin(ed::PinKind pinKind)
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

void Quest::Node::removePin(Quest::NodePin* pin)
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
