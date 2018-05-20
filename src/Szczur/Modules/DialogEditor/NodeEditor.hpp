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
	enum class FunctionType
	{
		None,
		Action,
		Condition
	};

private:
	ed::EditorContext* _context;

	std::unique_ptr<NodeManager> _nodeManager;

	DialogEditor* _dialogEditor = nullptr;

	NodePin* _newLinkPin = nullptr;

	int _contextId = 0;

	bool _optionConfigWindow = false;
	NodePin* _currentOption = nullptr;

	bool _optionFunctionConfigWindow = false;
	NodePin* _currentFunctionOption = nullptr;
	FunctionType _functionType = FunctionType::None;

	DLGEditor::TextContainer_t* _parts;

	char _renameBuffer[256];

public:
	NodeEditor(DialogEditor* dialogEditor);
	~NodeEditor();

	void update();

	void save(const std::string& fileName, FileFormat saveFormat);
	void load(const std::string& fileName, FileFormat loadFormat);

	void createNew();
	void reset();

	void setTextContainer(DLGEditor::TextContainer_t* parts) { _parts = parts; }

private:
	void showPopups();
	void showOptionConfig();

	void showOptionFunctionConfig();

	void drawIcon(bool filled, ImColor&& color = ImColor(255, 255, 255));
	std::string generateCode();
	void backupLuaFunctions();
	std::string getLuaFunction(NodePin* pin, FunctionType functionType, const std::string& code = std::string(), size_t* lastIndex = nullptr);

	void showTooltip(const std::string& message);

	int generateNodeNameId();
};

}
