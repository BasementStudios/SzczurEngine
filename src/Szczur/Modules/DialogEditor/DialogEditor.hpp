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

	std::vector<std::string>& _charactersNames;
	std::vector<std::string>& _charactersImagePaths;

	DLGEditor _dlgEditor;
	NodeEditor _nodeEditor;
	CharactersManager _CharactersManager;

	bool showCharactersManager = false;
	bool showDlgEditor = false;
	bool showNodeEditor = false;

public:
	DialogEditor();
	~DialogEditor();

	void update();

private:
	std::string makePathRelative(const std::string& path);
	std::string fixPathSlashes(const std::string& path);
};

}
