#pragma once

#include "Szczur/Utility/Modules/Module.hpp"

#include "NodeEditor.hpp"
#include "DLGEditor.hpp"

namespace rat
{

class DialogEditor : public Module<>
{
	friend NodeEditor;

protected:
	std::string _projectPath = "dialogs/dialog_1";

	std::vector<std::string> _characters = { "Mroczny", "Locha" };

	DLGEditor _dlgEditor;
	NodeEditor _nodeEditor;

	bool showDlgEditor = false;
	bool showNodeEditor = false;

public:
	DialogEditor();
	~DialogEditor();

	void update();
};

}
