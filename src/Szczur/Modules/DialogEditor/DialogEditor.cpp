#include "DialogEditor.hpp"

#include <experimental/filesystem>

#include "Szczur/Modules/FileSystem/DirectoryDialog.hpp"

#include "Szczur/Utility/MsgBox.hpp"

#ifdef OS_WINDOWS 
#include <shellapi.h> 
#endif

namespace fs = std::experimental::filesystem;

namespace rat
{

DialogEditor::DialogEditor()
	: _characters(_CharactersManager.getCharactersContainer()),
	  _dlgEditor(_characters, getModule<Input>().getManager()), _nodeEditor(this), _CharactersManager(_dlgEditor.getContainer())
{
	LOG_INFO("Initializing DialogEditor module");
	refreshDialogsList();
	LOG_INFO("Module DialogEditor initialized");
}

DialogEditor::~DialogEditor()
{
	LOG_INFO("Module DialogEditor destructed");
}

void DialogEditor::update()
{
	if (_projectLoaded && _showDialogEditor)
	{
		if (_showCharactersManager)
			_CharactersManager.update();

		if (_showDlgEditor)
			_dlgEditor.update();

		if (_showNodeEditor)
			_nodeEditor.update();

		if (ImGui::Begin("Dialog Editor", &_showDialogEditor, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Path: ./%s", _projectPath.c_str());

			ImGui::Separator();

			if (ImGui::Button("Test dialog"))
			{
				getModule<Dialog>().unload();
				saveProject();
				_nodeEditor.save(_projectPath + "/dialog.lua", NodeEditor::FileFormat::Lua);
				_showCharactersManager = false;
				_showDlgEditor = false;
				_showNodeEditor = false;

				getModule<Script>().scriptFile(_projectPath + "/dialog.lua");
			}

			ImGui::SameLine();

			if (ImGui::Button("Stop dialog"))
			{
				getModule<Dialog>().unload();
			}

			if (ImGui::Button("Show in explorer"))
			{
#ifdef OS_WINDOWS
				auto path = fs::current_path().string() + "\\" + _projectPath;

				ShellExecute(NULL, "open", path.c_str(), NULL, NULL, SW_SHOWDEFAULT);
#endif
			}

			if (ImGui::Button("Save"))
			{
				if (!_projectPath.empty())
				{
					saveProject();
				}
			}

			ImGui::Separator();

			ImGui::Checkbox("Characters Manager", &_showCharactersManager);
			ImGui::Checkbox("Dlg Editor", &_showDlgEditor);
			ImGui::Checkbox("Node Editor", &_showNodeEditor);
		}
		ImGui::End();
}

	if (ImGui::Begin("Dialogs' Directory Browser", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		if (ImGui::Button("Refresh list"))
		{
			refreshDialogsList();
		}

		ImGui::SameLine();

		if (ImGui::Button("Show in explorer"))
		{
			auto path = fs::current_path().string() + "\\" + _dialogsDirectory.Path;

			ShellExecute(NULL, "open", path.c_str(), NULL, NULL, SW_SHOWDEFAULT);
		}

		if (ImGui::Button("Add map"))
		{
			_showNewMapPopup = true;
		}

		ImGui::Separator();

		showDirectory(_dialogsDirectory);
	}

	ImGui::End();

	if (_showNewMapPopup)
	{
		ImGui::OpenPopup("New Map Popup");
		_showNewMapPopup = false;
	}

	if (ImGui::BeginPopupModal("New Map Popup", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Map name:");

		static char mapName[64];

		ImGui::InputText("##MapName", mapName, 64);

		ImGui::Separator();

		if (ImGui::Button("Ok"))
		{
			fs::create_directory(MainDirectory + std::string(mapName));
			refreshDialogsList();

			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel"))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	if (_showNewDialogPopup)
	{
		ImGui::OpenPopup("New Dialog Popup");
		_showNewDialogPopup = false;
	}

	if (ImGui::BeginPopupModal("New Dialog Popup", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		if (_newDialogPopupCurrentDir == nullptr)
			ImGui::CloseCurrentPopup();

		ImGui::Text("Dialog name:");

		static char dialogName[64];

		ImGui::InputText("##DialogName", dialogName, 64);

		ImGui::Separator();

		if (ImGui::Button("Ok"))
		{
			auto path = _newDialogPopupCurrentDir->Path + "\\" + std::string(dialogName);

			fs::create_directory(path);
			createProject(path);
			refreshDialogsList();

			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel"))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

void DialogEditor::saveProject()
{
	if (!_projectPath.empty())
	{
		_dlgEditor.save();
		_nodeEditor.save(_projectPath + "/dialog.json", NodeEditor::FileFormat::Json);
		_nodeEditor.save(_projectPath + "/dialog.lua", NodeEditor::FileFormat::Lua);
		_CharactersManager.save(_projectPath + "/characters.json");
	}
}

void DialogEditor::createProject(const std::string& path)
{
	_projectPath = fixPathSlashes(path);

	_dlgEditor.load(_projectPath);
	_nodeEditor.createNew();
	_nodeEditor.save(_projectPath + "/dialog.json", NodeEditor::FileFormat::Json);
	_CharactersManager.clear();
	_CharactersManager.save(_projectPath + "/characters.json");

	fs::copy(std::string(MainDirectory) + "dialog.flac", path + "/dialog.flac");

	_projectLoaded = true;
	_showDialogEditor = true;
	_showDlgEditor = false;
	_showNodeEditor = false;
	_showCharactersManager = false;
}

void DialogEditor::openProject(const std::string& path)
{
	if (path.empty())
		return;

	auto _path = fixPathSlashes(path);

	auto currentPath = fs::current_path().string();

	LOG_INFO("Opening `", _path, "`...");

	if (isProjectDirectory(_path))
	{
		getModule<Dialog>().unload();

		_projectPath = _path;

		_CharactersManager.load(_projectPath + "/characters.json");
		_dlgEditor.load(_projectPath);
		_nodeEditor.load(_projectPath + "/dialog.json", NodeEditor::FileFormat::Json);
		_nodeEditor.setTextContainer(&_dlgEditor.getContainer());

		_projectLoaded = true;
		_showDialogEditor = true;
		_showDlgEditor = false;
		_showNodeEditor = false;
		_showCharactersManager = false;
	}
	else
	{
		LOG_INFO("Missing files!");
		MsgBox::show("Cannot open project because cannot find dialog.dlg, dialog.json or characters.json", "Missing files", MsgBox::Icon::Warning);
	}
}

void DialogEditor::showDirectory(Directory& directory)
{
	for (auto& child : directory.Childs)
	{
		if (child.Type == Directory::DialogsDir)
		{
			if (ImGui::TreeNode(child.Name.c_str()))
			{
				showDirectory(child);
				ImGui::TreePop();
			}
		}
		else
		{
			ImGui::BulletText(child.Name.c_str());

			ImGui::SameLine();

			if (ImGui::SmallButton(("Open##" + child.Name).c_str()))
			{
				openProject(child.Path);
			}
		}
	}

	if (!directory.Root)
	{
		ImGui::Bullet();
		ImGui::SameLine();

		if (ImGui::SmallButton(("New##" + directory.Name).c_str()))
		{
			_newDialogPopupCurrentDir = &directory;
			_showNewDialogPopup = true;
		}
	}
}

void DialogEditor::refreshDialogsList()
{
	_dialogsDirectory.Type = Directory::DialogsDir;
	_dialogsDirectory.Name = MainDirectory;
	_dialogsDirectory.Path = MainDirectory;
	_dialogsDirectory.Root = true;

	_dialogsDirectory.Childs.clear();

	for (auto& mapDirPath : fs::directory_iterator(MainDirectory))
	{
		if (mapDirPath.path().filename() == "config")
			continue;

		if (!fs::is_directory(mapDirPath.status()))
			continue;

		Directory mapDir;
		mapDir.Name = mapDirPath.path().filename().string();
		mapDir.Path = mapDirPath.path().string();
		mapDir.Type = Directory::DialogsDir;

		for (auto& dialogDirPath : fs::directory_iterator(mapDirPath.path()))
		{
			if (!fs::is_directory(dialogDirPath.status()))
				continue;

			if (!isProjectDirectory(dialogDirPath.path().string()))
				continue;

			Directory dialogDir;
			dialogDir.Name = dialogDirPath.path().filename().string();
			dialogDir.Path = dialogDirPath.path().string();
			dialogDir.Type = Directory::ProjectDir;
			mapDir.Childs.push_back(std::move(dialogDir));
		}

		_dialogsDirectory.Childs.push_back(std::move(mapDir));
	}
}

bool DialogEditor::isProjectDirectory(const std::string& path)
{
	bool error = false;

	if (!fs::exists(path + "/dialog.dlg"))
		error = true;

	if (!fs::exists(path + "/dialog.json"))
		error = true;

	if (!fs::exists(path + "/characters.json"))
		error = true;

	return !error;
}

std::string DialogEditor::fixPathSlashes(const std::string& path)
{
	std::string result = path;

	std::replace(result.begin(), result.end(), '\\', '/');

	return result;
}

}
