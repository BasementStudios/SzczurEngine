#include "DialogEditor.hpp"

#include <experimental/filesystem>

#include "Szczur/Modules/FileSystem/FileDialog.hpp"
#include "Szczur/Modules/FileSystem/DirectoryDialog.hpp"

namespace rat
{

DialogEditor::DialogEditor()
	: _dlgEditor(_characters), _nodeEditor(this)
{
	_dlgEditor.load(_projectPath);

	LOG_INFO(this, " : Module DialogEditor constructed");
}

DialogEditor::~DialogEditor()
{
	LOG_INFO(this, " : Module DialogEditor destructed");
}

void DialogEditor::update()
{
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

		}

		ImGui::Separator();

		if (ImGui::Button("Create new project"))
		{
			_nodeEditor.createNew();
		}
		
		if (ImGui::Button("Open project..."))
		{
			auto directory = DirectoryDialog::getExistingDirectory("Select project folder", std::experimental::filesystem::current_path().string());

			if (!directory.empty())
			{
				LOG_INFO("Wizard is opening your directory (*magic happening*)");
			}
		}
		
		if (ImGui::Button("Save"))
		{
			if (!_projectPath.empty())
			{
				LOG_INFO("Wizard is saving project (*magic happening*)");
			}
		}

		if (ImGui::Button("Save as..."))
		{
			auto directory = DirectoryDialog::getExistingDirectory("Select project folder", std::experimental::filesystem::current_path().string() + _projectPath);

			if (!directory.empty())
			{
				LOG_INFO("Wizard is saving project (*magic happening*)");
			}
		}

		if (ImGui::Button("Generate"))
		{
			LOG_INFO("Generating lua...");

			if (!_projectPath.empty())
				_nodeEditor.save(_projectPath, NodeEditor::FileFormat::Lua);
		}

		ImGui::Separator();

		ImGui::Checkbox("Dlg Editor", &showDlgEditor);
		if (ImGui::Button("Reload"))
		{

		}

		ImGui::Separator();

		ImGui::Checkbox("Node Editor", &showNodeEditor);
		if (ImGui::Button("Reset"))
		{
			_nodeEditor.createNew();
		}

		if (ImGui::Button("Reset camera"))
		{
		}
	}
	ImGui::End();
}

}
