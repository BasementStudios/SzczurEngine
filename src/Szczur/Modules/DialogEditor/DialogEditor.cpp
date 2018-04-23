#include "DialogEditor.hpp"

#include <experimental/filesystem>

#include "Szczur/Modules/FileSystem/FileDialog.hpp"
#include "Szczur/Modules/FileSystem/DirectoryDialog.hpp"

#include "Szczur/Utility/MsgBox.hpp"

namespace rat
{

DialogEditor::DialogEditor()
	: _characters(_CharactersManager.getCharactersContainer()),
	  _dlgEditor(_characters), _nodeEditor(this), _CharactersManager(_dlgEditor.getContainer())
{
	LOG_INFO("Initializing DialogEditor module");
	LOG_INFO("Module DialogEditor initialized");
}

DialogEditor::~DialogEditor()
{
	LOG_INFO("Module DialogEditor destructed");
}

void DialogEditor::update()
{
	if (_projectLoaded)
	{
		if (_showCharactersManager)
			_CharactersManager.update();

		if (_showDlgEditor)
			_dlgEditor.update();

		if (_showNodeEditor)
			_nodeEditor.update();
	}

	if (ImGui::Begin("Dialog Editor", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		if (_projectLoaded)
		{
			ImGui::Text("Path: ./%s", _projectPath.c_str());

			ImGui::Separator();

			if (ImGui::Button("Test dialog"))
			{
				getModule<Dialog>().unload();

				_dlgEditor.save();
				_nodeEditor.save(_projectPath + "/dialog.json", NodeEditor::Json);
				_nodeEditor.save(_projectPath + "/dialog.lua", NodeEditor::Lua);
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
				auto path = std::experimental::filesystem::current_path().string() + "\\" + _projectPath;

				ShellExecute(NULL, "open", path.c_str(), NULL, NULL, SW_SHOWDEFAULT);
#endif
			}

			ImGui::Separator();
		}
		else
		{
			ImGui::Text("First create or load project");
			ImGui::Separator();
		}

		if (ImGui::Button("Create new project"))
		{
			auto directory = DirectoryDialog::getExistingDirectory("Select project directory", std::experimental::filesystem::current_path().string());

			if (!directory.empty())
			{
				_projectPath = fixPathSlashes(makePathRelative(directory));

				_dlgEditor.load(_projectPath);
				_nodeEditor.createNew();
				_nodeEditor.save(_projectPath + "/dialog.json", NodeEditor::Json);
				_CharactersManager.clear();
				_CharactersManager.save(_projectPath + "/characters.json");

				_projectLoaded = true;
			}
		}

		if (ImGui::Button("Open project..."))
		{
			auto currentPath = std::experimental::filesystem::current_path().string();

			auto directory = DirectoryDialog::getExistingDirectory("Select project directory", currentPath);

			if (!directory.empty())
			{
				directory = fixPathSlashes(makePathRelative(directory));

				LOG_INFO("Opening ", directory, "...");

				bool error = false;

				if (!std::experimental::filesystem::exists(directory + "/dialog.dlg"))
					error = true;

				if (!std::experimental::filesystem::exists(directory + "/dialog.json"))
					error = true;

				if (!std::experimental::filesystem::exists(directory + "/characters.json"))
					error = true;

				if (error == false)
				{
					_projectPath = directory;

					_CharactersManager.load(_projectPath + "/characters.json");
					_dlgEditor.load(_projectPath);
					_nodeEditor.load(_projectPath + "/dialog.json", NodeEditor::Json);
					_nodeEditor.setTextContainer(&_dlgEditor.getContainer());

					_projectLoaded = true;
				}
				else
				{
					LOG_INFO("Missing files!");
					MsgBox::show("Cannot open project because cannot find dialog.dlg, dialog.json or characters.json", "Missing files", MsgBox::Icon::Warning);
				}
			}
		}

		if (_projectLoaded)
		{
			if (ImGui::Button("Save"))
			{
				if (!_projectPath.empty())
				{
					_CharactersManager.save(_projectPath + "/characters.json");
					_dlgEditor.save();
					_nodeEditor.save(_projectPath + "/dialog.json", NodeEditor::Json);
				}
			}

			if (ImGui::Button("Generate"))
			{
				LOG_INFO("Generating lua...");

				if (!_projectPath.empty())
					_nodeEditor.save(_projectPath + "/dialog.lua", NodeEditor::FileFormat::Lua);
			}

			ImGui::Separator();

			ImGui::Checkbox("Characters Manager", &_showCharactersManager);
			ImGui::Checkbox("Dlg Editor", &_showDlgEditor);
			ImGui::Checkbox("Node Editor", &_showNodeEditor);
		}
	}
	ImGui::End();
}

std::string DialogEditor::makePathRelative(const std::string& path)
{
	auto currentPath = std::experimental::filesystem::current_path().string();

	size_t start = path.find(currentPath);

	if (start != -1)
	{
		return path.substr(currentPath.length() + 1, path.length() - currentPath.length() - 1);
	}

	return path;
}

std::string DialogEditor::fixPathSlashes(const std::string& path)
{
	std::string result = path;

	std::replace(result.begin(), result.end(), '\\', '/');

	return result;
}

}
