#include "ArmatureDisplayDataManager.hpp"

#include "../ScenesManager.hpp"

#include "Szczur/Modules/FileSystem/FileDialog.hpp"
#include "Szczur/Utility/Convert/Windows1250.hpp"

namespace rat 
{

ArmatureDisplayDataManager::ArmatureDisplayDataManager(ScenesManager& scenes) 
	: _scenes{scenes} 
{

}

void ArmatureDisplayDataManager::render(bool& ifRender)
{
	static char enteredText[255];
	if (ImGui::Begin("Armature Display Data Manager", &ifRender))
	{
		auto& armatureDisplayDataHolder = _scenes.getArmatureDisplayDataHolder();

		if (ImGui::Button("Add"))
		{
			std::string directory = _scenes.getRelativePathFromExplorer("Select armature file", ".\\Assets");

			auto path = std::experimental::filesystem::path(directory).parent_path().string();

			auto armatureDisplayData = std::find_if(armatureDisplayDataHolder.begin(), armatureDisplayDataHolder.end(), [&] (auto& armature) { 
				return armature->getFolderPath() == path;
			});

			if (armatureDisplayData == armatureDisplayDataHolder.end())
			{
				try
				{
					armatureDisplayDataHolder.emplace_back(std::move(std::make_unique<ArmatureDisplayData>(path))).get();
				}
				catch (std::exception& ex)
				{
					LOG_EXCEPTION(ex);
					return;
				}
			}
		}

		ImGui::Separator();

		if (ImGui::BeginChild("Datas"))
		{
			for (auto it = armatureDisplayDataHolder.begin(); it != armatureDisplayDataHolder.end(); ++it)
			{
				if (ImGui::SmallButton("-"))
				{
					armatureDisplayDataHolder.erase(it);

					break;
				}
				ImGui::SameLine();
				ImGui::Text(mapWindows1250ToUtf8((*it)->getName()).c_str());
			}
		}
		ImGui::EndChild();
	}
	ImGui::End();
}

}
