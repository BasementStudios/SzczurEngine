#pragma once

#include "Szczur/Utility/Modules/Module.hpp"
#include "Szczur/Modules/Script/Script.hpp"
#include "Szczur/Modules/Dialog/Dialog.hpp"

#include "NodeEditor.hpp"
#include "DLGEditor.hpp"
#include "CharactersManager.hpp"

namespace rat
{

class DialogEditor : public Module<Script, Dialog>
{
	friend NodeEditor;

protected:
	std::string _projectPath;  // relative path to dialog files

	bool _projectLoaded = false;

	std::vector<CharacterData>& _characters;

	DLGEditor _dlgEditor;
	NodeEditor _nodeEditor;
	CharactersManager _CharactersManager;

	bool _showDlgEditor = false;
	bool _showNodeEditor = false;
	bool _showCharactersManager = false;

public:
	DialogEditor();
	~DialogEditor();

	void update();

private:
	std::string makePathRelative(const std::string& path);
	std::string fixPathSlashes(const std::string& path);
};

}
