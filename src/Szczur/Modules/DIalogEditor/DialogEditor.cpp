#include "DialogEditor.hpp"

#include "Szczur/Modules/FileSystem/FileDialog.hpp"

namespace rat
{

DialogEditor::DialogEditor()
	: _dlgEditor(_characters), _nodeEditor(this)
{
	_dlgEditor.load(_path);

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
		ImGui::Checkbox("Dlg Editor", &showDlgEditor);
		ImGui::Separator();

		ImGui::Checkbox("Node Editor", &showNodeEditor);
		if (ImGui::Button("Reset"))
		{
			_nodeEditor.createNew();
		}

		if (ImGui::Button("Generate lua"))
		{
			auto filePath = FileDialog::getSaveFileName("Lua", "", "Lua (*.lua)|*.lua");

			if (!filePath.empty())
				_nodeEditor.save(filePath, NodeEditor::FileFormat::Lua);
		}

		if (ImGui::Button("Save"))
		{
			auto filePath = FileDialog::getSaveFileName("Json", "", "Json (*.json)|*.json");

			if (!filePath.empty())
				_nodeEditor.save(filePath, NodeEditor::FileFormat::Json);
		}

		if (ImGui::Button("Load"))
		{
			auto filePath = FileDialog::getOpenFileName("Json", "", "Json (*.json)|*.json");

			if (!filePath.empty())
			{
				_nodeEditor.load(filePath, NodeEditor::FileFormat::Json);
			}
		}
	}
	ImGui::End();
}

}
