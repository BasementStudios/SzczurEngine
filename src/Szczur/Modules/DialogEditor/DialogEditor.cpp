#include "DialogEditor.hpp"

#include <experimental/filesystem>

#include "Szczur/Modules/FileSystem/FileDialog.hpp"
#include "Szczur/Modules/FileSystem/DirectoryDialog.hpp"

namespace rat
{

DialogEditor::DialogEditor()
	: _charactersNames(_CharactersManager.getNamesContainer()), _charactersImagePaths(_CharactersManager.getImagePathsContainer()),
	  _dlgEditor(_charactersNames), _nodeEditor(this)
{
	LOG_INFO("Initializing DialogEditor module");

	_dlgEditor.load(_projectPath);

	LOG_INFO("Module DialogEditor initialized");
}

DialogEditor::~DialogEditor()
{
	LOG_INFO("Module DialogEditor destructed");
}

void DialogEditor::update()
{
	if (showCharactersManager)
		_CharactersManager.update();

	if (showDlgEditor)
		_dlgEditor.update();

	if (showNodeEditor)
		_nodeEditor.update();

	if (ImGui::Begin("Dialog Editor", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Dialog project: %s", "Test");

		ImGui::Separator();

		if (ImGui::Button("Test dialog"))
		{
			getModule<Dialog>().load(_projectPath + "/dialog");
		}

		if (ImGui::Button("Show in explorer"))
		{
#ifdef OS_WINDOWS
			auto path = std::experimental::filesystem::current_path().string() + "\\" + _projectPath;

			ShellExecute(NULL, "open", path.c_str(), NULL, NULL, SW_SHOWDEFAULT);
#endif
		}

		ImGui::Separator();

		if (ImGui::Button("Create new project"))
		{
			auto directory = DirectoryDialog::getExistingDirectory("Select project directory", std::experimental::filesystem::current_path().string());

			if (!directory.empty())
			{
				_projectPath = directory;
			}
		}
		
		if (ImGui::Button("Open project..."))
		{
			auto directory = DirectoryDialog::getExistingDirectory("Select project directory", std::experimental::filesystem::current_path().string());

			if (!directory.empty())
			{
				LOG_INFO("Opening ", directory, "...");

				bool error = false;

				if (!std::experimental::filesystem::exists(directory + "/dialog.dlg"))
					error = true;

				if (!std::experimental::filesystem::exists(directory + "/dialog.json"))
					error = true;

				if (error == false)
				{
					_projectPath = directory;

					_dlgEditor.load(_projectPath);
					_nodeEditor.load(_projectPath + "/dialog.json", NodeEditor::Json);
					_nodeEditor.setTextContainer(&_dlgEditor.getContainer());
				}
				else
				{
					LOG_INFO("Missing files!");
				}
			}
		}
		
		if (ImGui::Button("Save"))
		{
			if (!_projectPath.empty())
			{
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

		ImGui::Checkbox("Characters Manager", &showCharactersManager);

		ImGui::Separator();

		ImGui::Checkbox("Dlg Editor", &showDlgEditor);
		ImGui::Checkbox("Node Editor", &showNodeEditor);
	}
	ImGui::End();
}

}
