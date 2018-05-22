#include "AudioEditor.hpp"
namespace rat
{
  AudioEditor::AudioEditor()
  {
    _soundEditor.init();
  }

  void AudioEditor::render()
  { 
    ImGui::Begin("Hello, world2!");
          
      ImGui::InputText("Path to file .flac",file,BUFFER);
      ImGui::InputText("Sound name",soundName,BUFFER);
      if(ImGui::Button("Add sound")){
        addSound(soundName,file);
      }   

      if(ImGui::Button("Save Sounds")){
        _soundEditor.save(saveFileName);
      }
      ImGui::SameLine();
      ImGui::InputText("Save Sounds",saveFileName,BUFFER);
      
      if(ImGui::Button("Load Sounds")){
        _soundEditor.load(loadFileName);
      }
      ImGui::SameLine();
      ImGui::InputText("Load sounds from file",loadFileName,BUFFER);
      
      ImGui::Checkbox("Delete Sounds",&deleteField);
      ImGui::SameLine(); 
      if(deleteField && ImGui::Button("Delete all sounds")){
        _soundEditor.clear();
      } 
    
    ImGui::End();

    _soundEditor.render();
  }

  void AudioEditor::addSound(const std::string& soundName,const std::string& soundFileName)
  {
    _soundEditor.addSound(soundName,soundFileName);
  }

  void AudioEditor::save(const std::string& fileName)
  {
    _soundEditor.save(fileName);
  }
  
  void AudioEditor::load(const std::string& fileName)
  {
    _soundEditor.load(fileName);
  }
}