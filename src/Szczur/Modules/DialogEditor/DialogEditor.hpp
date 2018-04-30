#pragma once

#include <string>

#include "Szczur/Utility/Modules/Module.hpp"
#include "Szczur/Modules/Script/Script.hpp"
#include "Szczur/Modules/Dialog/Dialog.hpp"

#include "NodeEditor.hpp"
#include "DLGEditor.hpp"
#include "CharactersManager.hpp"

namespace rat
{

class Directory
{
public:
	enum DirectoryType
	{
		DialogsDir, // list of dialogs 
		ProjectDir  // directory with dialog and all files
	};

	std::string Name;

	std::string Path;

	DirectoryType Type;

	bool Root = false;

	// Only when Type == DialogsDir
	std::vector<Directory> Childs;
};

class DialogEditor : public Module<Input, Script, Dialog>
{
	friend NodeEditor;

protected:
	std::string _projectPath;  // relative path to dialog files

	bool _projectLoaded = false;

	std::vector<CharacterData>& _characters;

	DLGEditor _dlgEditor;
	NodeEditor _nodeEditor;
	CharactersManager _CharactersManager;

	bool _showDialogEditor = false;
	bool _showDlgEditor = false;
	bool _showNodeEditor = false;
	bool _showCharactersManager = false;

private:
	Directory _dialogsDirectory;
	bool _showNewMapPopup = false;

	bool _showNewDialogPopup = false;
	Directory* _newDialogPopupCurrentDir = nullptr;

public:
	DialogEditor();
	~DialogEditor();

	void update();

	void saveProject();
	bool isProjectLoaded() { return _projectLoaded; }

private:
	void createProject(const std::string& path);
	void openProject(const std::string& path);

	void showDirectory(Directory& directory);

	void refreshDialogsList();

	bool isProjectDirectory(const std::string& path);

	std::string fixPathSlashes(const std::string& path);
};

}
