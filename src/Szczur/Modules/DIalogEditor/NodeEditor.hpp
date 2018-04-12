#pragma once

#include <NodeEditor.h>

#include "NodeManager.hpp"
#include "DLGEditor.hpp"

namespace ed = ax::NodeEditor;

namespace rat
{
class DialogEditor;

class NodeEditor
{
public:
	enum FileFormat
	{
		Lua,
		Json
	};

private:
	ed::EditorContext* _context;

	std::unique_ptr<NodeManager> _nodeManager;

	DialogEditor* _dialogEditor = nullptr;

	bool creatingNode = false;

	NodePin* _newLinkPin = nullptr;

	int _contextId = 0;

	bool _optionConfigWindow = false;
	NodePin* _currentOption = nullptr;

	DLGEditor::TextContainer_t* _parts;

public:
	NodeEditor(DialogEditor* dialogEditor);
	~NodeEditor();

	void update();

	void showPopups();

	void showOptionConfig();

	void save(const std::string& fileName, FileFormat saveFormat);
	void load(const std::string& fileName, FileFormat loadFormat);

	void createNew();
	void reset();

	void setTextContainer(DLGEditor::TextContainer_t* parts) { _parts = parts; }

private:
	void drawIcon(bool filled, ImColor&& color = ImColor(255, 255, 255));
	std::string generateCode();
};

}
