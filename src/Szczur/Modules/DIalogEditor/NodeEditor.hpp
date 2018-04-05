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

	int _contextId = 0;

	bool _optionConfigWindow = false;
	NodePin* _currentOption = nullptr;

public:
	NodeEditor();
	~NodeEditor();

	void update();

	void showPopups();

	void showOptionConfig();


	void save(const std::string& fileName);

private:
	void drawIcon(bool filled, ImColor&& color = ImColor(255, 255, 255));
	std::string generateCode();
};

}
