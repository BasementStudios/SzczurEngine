#pragma once

#include <NodeEditor.h>
#include "NodeManager.hpp"

namespace ed = ax::NodeEditor;

namespace rat
{
class NodeEditor
{
private:
	ed::EditorContext* _context;

	std::unique_ptr<NodeManager> _nodeManager;


	bool creatingNode = false;

	NodePin* _newLinkPin = nullptr;

public:
	NodeEditor();
	~NodeEditor();

	void update();

	void drawIcon(bool filled);
};

}
