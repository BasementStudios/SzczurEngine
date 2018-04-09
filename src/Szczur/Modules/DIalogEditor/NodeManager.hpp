#pragma once

#include <memory>
#include <vector>
#include <iostream>
#include <string>

#include <NodeEditor.h>
#include <nlohmann/json.hpp>

namespace ed = ax::NodeEditor;
using json = nlohmann::json;

namespace rat
{
class Node;
class NodeManager;

struct NodePin
{
	int Id = -1;
	Node* Node = nullptr;
	ed::PinKind Kind = ed::PinKind::Input;

	int OptionTarget = -1;

	bool LinkToSameNode = false;

	NodePin() { }
	NodePin(int id, ed::PinKind kind) :
		Id(id), Kind(kind) { }
};

class Node
{
	friend NodeManager;

public:
	enum NodeType
	{
		Start,
		End,
		Options
	};

public:
	int Id = -1;
	std::string Name;
	std::vector<std::unique_ptr<NodePin>> Inputs;
	std::vector<std::unique_ptr<NodePin>> Outputs;

	NodeType Type = Options;

private:
	int _lastPinId = 0;

public:
	Node() { }
	Node(int id, const std::string& name, NodeType nodeType) :
		Id(id), Name(name), Type(nodeType) { }

	NodePin* createPin(ed::PinKind pinKind);

	void removePin(NodePin *pin);
};

struct NodeLink
{
	int Id = -1;

	int StartPinId = 1;
	int EndPinId = -1;

	bool SameNode = false;

	ImColor Color = { 255, 255, 255 };

	NodeLink() {}
	NodeLink(int id, int startPinId, int endPinId) :
		Id(id), StartPinId(startPinId), EndPinId(endPinId) { }
};

class NodeManager
{
private:
	std::vector<std::unique_ptr<Node>> _nodes;

	std::vector<std::unique_ptr<NodeLink>> _links;

	int _lastId = 0;

public:
	NodeManager() = default;
	~NodeManager() = default;

	bool read(const json& j);
	void write(json& j);

	void reset();


	NodePin* findPin(int pinId);


	Node* createNode(const std::string& name, Node::NodeType type = Node::Options);

	Node* findNode(int nodeId);

	void removeNode(int nodeId);

	auto& getNodes() { return _nodes; }


	NodeLink* createLink(int startPinId, int endPinId);

	bool isPinLinked(int pinId);

	NodeLink* findLink(int linkId);

	void removeLink(int linkId);

	auto& getLinks() { return _links; }

private:
	int GetNewId() { return ++_lastId; }
};

}
