#pragma once

#include <memory>
#include <vector>
#include <iostream>
#include <string>

#include <NodeEditor/NodeEditor.h>
#include <nlohmann/json.hpp>
#include <glm/vec4.hpp>

#include "../DLGEditor/DialogData.hpp"

namespace ed = ax::NodeEditor;
using Json = nlohmann::json;

namespace rat
{

class Node;
class NodeManager;

struct NodePin
{
	int Id = -1;
	Node* Node = nullptr;
	ed::PinKind Kind = ed::PinKind::Input;

	struct
	{
		DialogData* Ptr = nullptr;
		std::weak_ptr<DialogData> WeakPtr;

		struct
		{
			int Major = -1;
			int Minor = -1;
		} Id;
	} OptionTarget;

	bool LinkToSameNode = false;

	bool ConditionFunc = false;
	std::string ConditionFuncName;
	std::string ConditionFuncCode;

	bool ActionFunc = false;
	std::string ActionFuncName;
	std::string ActionFuncCode;

	bool SkipOptions = false;

	bool Colorful = false;
	glm::vec4 Color = { 1.f, 1.f, 1.f, 1.f };

	bool Icon = false;
	int IconId = -1;

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
	int NameId = 0;

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

	bool read(const Json& j);
	void write(Json& j);

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
