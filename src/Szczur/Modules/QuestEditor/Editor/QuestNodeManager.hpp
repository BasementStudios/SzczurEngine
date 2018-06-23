#pragma once

#include <memory>
#include <vector>
#include <iostream>
#include <string>

#include <NodeEditor/NodeEditor.h>

namespace ed = ax::NodeEditor;

namespace rat
{

class QuestNodeManager;

namespace Quest
{
	class Node;

	struct NodePin
	{
		int Id = -1;
		Node* Node = nullptr;
		ed::PinKind Kind = ed::PinKind::Input;

		bool IsInHeader = false;

		NodePin() {}
		NodePin(int id, ed::PinKind kind) :
			Id(id), Kind(kind)
		{
		}
	};

	class Node
	{
		friend QuestNodeManager;

	public:
		int Id = -1;
		std::string Name;

		// data here

		std::vector<std::unique_ptr<NodePin>> Inputs;
		std::vector<std::unique_ptr<NodePin>> Outputs;

		NodePin* Output;

	protected:
		int _lastPinId = 0;

	public:
		Node() {}
		Node(int id, const std::string& name) :
			Id(id), Name(name)
		{
		}

		NodePin* createPin(ed::PinKind pinKind);

		void removePin(NodePin *pin);
	};

	struct NodeLink
	{
		int Id = -1;

		int StartPinId = 1;
		int EndPinId = -1;

		ImColor Color = { 255, 255, 255 };

		NodeLink() {}
		NodeLink(int id, int startPinId, int endPinId) :
			Id(id), StartPinId(startPinId), EndPinId(endPinId)
		{
		}
	};

}

class QuestNodeManager
{
private:
	std::vector<std::unique_ptr<Quest::Node>> _nodes;

	std::vector<std::unique_ptr<Quest::NodeLink>> _links;

	int _lastId = 0;

public:
	QuestNodeManager() = default;
	~QuestNodeManager() = default;

	Quest::NodePin* findPin(int pinId);


	Quest::Node* createNode(const std::string& name);

	Quest::Node* findNode(int nodeId);

	void removeNode(int nodeId);

	auto& getNodes() { return _nodes; }


	Quest::NodeLink* createLink(int startPinId, int endPinId);

	bool isPinLinked(int pinId);

	Quest::NodeLink* findLink(int linkId);

	void removeLink(int linkId);

	auto& getLinks() { return _links; }

private:
	int GetNewId() { return ++_lastId; }
};

}
