#pragma once

#include <NodeEditor/NodeEditor.h>
#include "QuestNodeManager.hpp"

namespace ed = ax::NodeEditor;

namespace rat
{
class QuestNodeManager;

class QuestNodeEditor
{
private:
	ed::EditorContext* _context = nullptr;

	int _contextId = 0;

	std::unique_ptr<QuestNodeManager> _nodeManager;

	Quest::NodePin* _newLinkPin = nullptr;

public:
	QuestNodeEditor();
	~QuestNodeEditor();

	void update();

private:
	void drawIcon(bool filled, ImColor&& color = ImColor(255, 255, 255));

	void drawNode(Quest::Node* node);

	void showPopups();
	
	void creatingLinks();
	void deleting();
};

}
