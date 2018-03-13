#include "Script.hpp"


namespace rat {
	void Script::updateEditor() {
		
		// IF_EDITOR {
			// ImGui::Begin("Scripts", nullptr);
			// if(ImGui::Button("Open")) {
				
				// // ShellExecute(GetDesktopWindow(), "open", "./scripts/test.lua", NULL, NULL , SW_SHOW );
			// }
			// ImGui::End();
		// }
	}
}



/*
			static TextEditor editor;	
			
			static const char* fileToEdit = "scripts/test.lua";			
			
			static bool isStart = false;
			if(!isStart) {
				isStart = true;
				auto lang = TextEditor::LanguageDefinition::Lua();
				editor.SetLanguageDefinition(lang);
				std::ifstream file(fileToEdit);				
				std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
				if(file.good()) editor.SetText(str);
			}
			
	
			// ImGui::Begin("Scripts", nullptr);
			auto cpos = editor.GetCursorPosition();
			ImGui::Begin("Script editor", nullptr, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar);
			ImGui::SetWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("Save"))
					{
						auto textToSave = editor.GetText();
						std::ofstream file(fileToEdit);
						file.clear();
						file<<textToSave;
						file.close();
						/// save text....
					}
					if (ImGui::MenuItem("Quit", "Alt-F4")) std::cout<<"Quit\n";
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Edit"))
				{
					bool ro = editor.IsReadOnly();
					if (ImGui::MenuItem("Read-only mode", nullptr, &ro))
						editor.SetReadOnly(ro);
					ImGui::Separator();

					if (ImGui::MenuItem("Undo", "ALT-Backspace", nullptr, !ro && editor.CanUndo()))
						editor.Undo();
					if (ImGui::MenuItem("Redo", "Ctrl-Y", nullptr, !ro && editor.CanRedo()))
						editor.Redo();

					ImGui::Separator();

					if (ImGui::MenuItem("Copy", "Ctrl-C", nullptr, editor.HasSelection()))
						editor.Copy();
					if (ImGui::MenuItem("Cut", "Ctrl-X", nullptr, !ro && editor.HasSelection()))
						editor.Cut();
					if (ImGui::MenuItem("Delete", "Del", nullptr, !ro && editor.HasSelection()))
						editor.Delete();
					if (ImGui::MenuItem("Paste", "Ctrl-V", nullptr, !ro && ImGui::GetClipboardText() != nullptr))
						editor.Paste();

					ImGui::Separator();

					if (ImGui::MenuItem("Select all", nullptr, nullptr))
						editor.SetSelection(TextEditor::Coordinates(), TextEditor::Coordinates(editor.GetTotalLines(), 0));

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("View"))
				{
					if (ImGui::MenuItem("Dark palette"))
						editor.SetPalette(TextEditor::GetDarkPalette());
					if (ImGui::MenuItem("Light palette"))
						editor.SetPalette(TextEditor::GetLightPalette());
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}

			ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s | %s", cpos.mLine + 1, cpos.mColumn + 1, editor.GetTotalLines(),
				editor.IsOverwrite() ? "Ovr" : "Ins",
				editor.CanUndo() ? "*" : " ",
				editor.GetLanguageDefinition().mName.c_str(), fileToEdit);

			editor.Render("TextEditor");
			ImGui::End();
		}
		*/