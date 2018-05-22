#include "SoundEditor.hpp"
namespace rat
{
  void SoundEditor::init()
  {
    _soundManager.setVolume(100);
  }

  void SoundEditor::render()
  { 
    ImGui::Begin("Hello, world!");
      for(int i=0; i<_soundManager.getSize(); ++i){
            
        if(ImGui::Button(_soundManager.getName(i).c_str())){
          _soundManager.play( _soundManager.getName(i));
        }
        ImGui::SameLine(); 
        if(ImGui::Button((_soundManager.getName(i)+" Pause").c_str())){
          _soundManager.pause( _soundManager.getName(i));
        }
        ImGui::SameLine();
        if(ImGui::Button((_soundManager.getName(i)+" Stop").c_str())){
          _soundManager.stop( _soundManager.getName(i));
        }
        
        ImGui::Checkbox(("Edit "+_soundManager.getName(i)).c_str(),&_soundsInfo[i]->enable);
        ImGui::SameLine(); 
        ImGui::Checkbox(("Delete "+_soundManager.getName(i)).c_str(),&_soundsInfo[i]->toDelete);

        if(_soundsInfo[i]->enable){
          ImGui::SliderFloat((_soundManager.getName(i)+" Volume").c_str(),&_soundsInfo[i]->volume,mini,max);  
          _soundManager.setVolume(_soundsInfo[i]->volume,_soundManager.getName(i));

           ImGui::SliderFloat((_soundManager.getName(i)+" Pitch").c_str(),&_soundsInfo[i]->pitch,0,2);  
          _soundManager.setPitch(_soundsInfo[i]->pitch,_soundManager.getName(i));

          ImGui::SliderFloat2((_soundManager.getName(i)+" Time").c_str(),_soundsInfo[i]->offsetTime,0,_soundManager.getLength(_soundManager.getName(i))); 
          _soundManager.setOffset(_soundManager.getName(i),_soundsInfo[i]->offsetTime[0],_soundsInfo[i]->offsetTime[1]);
        }

        if(_soundsInfo[i]->toDelete && ImGui::Button((_soundManager.getName(i)+" Delete").c_str())){
          _soundManager.eraseSingleSound(i);
          _soundsInfo.erase(_soundsInfo.begin()+i);
          break;
        }

      }
    ImGui::End();
  }

  void SoundEditor::addSound(const std::string& soundName,const std::string& soundFileName)
  {
    if(!_soundManager.newSound(soundName,soundFileName))
      return;
    std::unique_ptr<SoundInfo> soundInfo(new SoundInfo);
    soundInfo->enable=false;
    soundInfo->toDelete=false;
    soundInfo->offsetTime[0]=0;
    soundInfo->offsetTime[1]=_soundManager.getLength(soundName);
    soundInfo->name=soundName;         
    soundInfo->fileName=soundFileName;         
    soundInfo->volume=100;         
    soundInfo->pitch=1;         
    _soundsInfo.push_back(std::move(soundInfo));
  }

 void SoundEditor::save(const std::string& fileName)
  {
    std::fstream file;    
    std::string mainName = fileName + ".dat";
    file.open(mainName, std::ios::out | std::ios::trunc);
    for (int i=0; i<_soundManager.getSize(); ++i){
      file<<_soundsInfo[i]->name<<" ";
      file<<_soundsInfo[i]->fileName<<" ";
      file<<_soundsInfo[i]->offsetTime[0]<<" ";
      file<<_soundsInfo[i]->offsetTime[1]<<" ";
      file<<_soundsInfo[i]->volume<<" ";
      file<<_soundsInfo[i]->pitch<<"\n";
    }

  }

  void SoundEditor::load(const std::string& fileName)
  {
    std::fstream file;        
    std::string mainName = fileName + ".dat";
    file.open(mainName, std::ios::in);
    std::string name;
    std::string soundFileName;

    while(file >> name){ 
      file >> soundFileName;
      addSound(name,soundFileName);
      file >> _soundsInfo[_soundsInfo.size()-1]->offsetTime[0];
      file >> _soundsInfo[_soundsInfo.size()-1]->offsetTime[1];
      file >> _soundsInfo[_soundsInfo.size()-1]->volume;
      file >> _soundsInfo[_soundsInfo.size()-1]->pitch;
    }
  }

  void SoundEditor::clear()
  {
    _soundManager.eraseSounds();
    _soundsInfo.clear();
  }
}