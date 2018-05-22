#include "RatMusic.hpp"

#include "Szczur/Utility/Logger.hpp"

#include <json.hpp>

namespace rat
{

	RatMusic::RatMusic()
		: AudioEffect(m_source)
	{
		LOG_INFO("RatMusic created");
	}

	void RatMusic::init(const std::string& name)
	{
		_name = name;
		getJsonData();
	}

	void RatMusic::incrementCounter()
	{
		++_counter;
	}

	void RatMusic::decrementCounter()
	{
		--_counter;
	}

	unsigned int RatMusic::getCounterValue() const
	{
		return _counter;
	}

	void RatMusic::saveToJson()
	{
		json j;

		j[_name]["BPM"] = _bpm;
		j[_name]["FadeTime"] = _fadeTime;
		j[_name]["Volume"] = getVolume();

		std::ofstream file("music/" + _name + ".json", std::ios::trunc);
        if (file.is_open()) {
            file << j;
        }
        file.close();
	}
	
	void RatMusic::getJsonData()
	{
		json j;
		auto path = "music/" + _name + ".json";
		std::ifstream file(path);

		float numberOfBars;
		
		if(file.is_open()){
			file >> j;
			file.close();
			_bpm = j[_name]["BPM"];
			numberOfBars = j[_name]["FadeTime"];
			setVolume(j[_name]["Volume"]);
		}
		else {
			_bpm = 60;
			numberOfBars = 0;
			setVolume(100);
		}

		if(numberOfBars > 0) {
			float barTime = 240 / _bpm;
			_fadeTime = barTime * numberOfBars;
		}
	}

}