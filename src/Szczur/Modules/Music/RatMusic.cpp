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
		std::array<std::vector<float>, 3> effects;
		for (unsigned int i = 0; i < MAX_AUX_FOR_SOURCE; ++i) {
            if(effectsTypes[i] != AudioEffect::EffectType::None) {
				switch(effectsTypes[i]) {
					case AudioEffect::EffectType::Reverb:
						effects[0] = {
							DEN, DIFF, G, RGH, DT, DHF, RG, RD, LRG, LRG, AAGH, RRF, (float)DHL
						};
						break;
					case AudioEffect::EffectType::Echo:
						effects[1] = {
							DL, LDL, DG, FB, SP
						}; 
						break;
					case AudioEffect::EffectType::Equalizer: 
						effects[2] = {
							LC, LG, HC, HG, M1C, M1W, M1G, M2C, M2W, M2G
						};
						break;
					default: 
						break;
				}
			}
		}

		json j;

		j[_name]["BPM"] = _bpm;
		j[_name]["FadeTime"] = _fadeTime;
		j[_name]["Volume"] = getVolume();
		j[_name]["Effects"] = effects;

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
			LOG_INFO("1");
			if(j[_name]["Effects"][0].size() > 0) {
				DEN = j[_name]["Effects"][0][0];
				DIFF = j[_name]["Effects"][0][1];
				G = j[_name]["Effects"][0][2];
				RGH = j[_name]["Effects"][0][3];
				DT = j[_name]["Effects"][0][4];
				DHF = j[_name]["Effects"][0][5];
				RG = j[_name]["Effects"][0][6];
				RD = j[_name]["Effects"][0][7];
				LRG = j[_name]["Effects"][0][8];
				LRG = j[_name]["Effects"][0][9];
				AAGH = j[_name]["Effects"][0][10];
				RRF = j[_name]["Effects"][0][11];
				DHL = j[_name]["Effects"][0][12];

				effectsTypes[lastFreeSlot()] = AudioEffect::EffectType::Reverb;
                getEffect<Reverb>().density(DEN);
				getEffect<Reverb>().diffusion(DIFF);
				getEffect<Reverb>().gain(G);
				getEffect<Reverb>().gainHf(RGH);
				getEffect<Reverb>().decayTime(DT);
				getEffect<Reverb>().decayHfRatio(DHF);
				getEffect<Reverb>().reflectionsGain(RG);
				getEffect<Reverb>().reflectionsDelay(RD);
				getEffect<Reverb>().lateReverbGain(LRG);
				getEffect<Reverb>().lateReverbDelay(LRD);
				getEffect<Reverb>().airAbsorptionGainHf(AAGH);
				getEffect<Reverb>().roomRolloffFactor(RRF);
				getEffect<Reverb>().decayHfLimit(DHL);
			}
			if(j[_name]["Effects"][1].size() > 0) {
				DL = j[_name]["Effects"][1][0];
				LDL = j[_name]["Effects"][1][1]; 
				DG = j[_name]["Effects"][1][2];
				FB = j[_name]["Effects"][1][3]; 
				SP = j[_name]["Effects"][1][4];

				effectsTypes[lastFreeSlot()] = AudioEffect::EffectType::Echo;
                getEffect<Echo>().delay(DL);
				getEffect<Echo>().lrDelay(LDL);
			    getEffect<Echo>().damping(DG);
				getEffect<Echo>().feedback(FB);
				getEffect<Echo>().spread(SP);
			}
			if(j[_name]["Effects"][2].size() > 0) {
				LC = j[_name]["Effects"][2][0];
				LG = j[_name]["Effects"][2][1];
				HC = j[_name]["Effects"][2][2]; 
				HG = j[_name]["Effects"][2][3]; 
				M1C = j[_name]["Effects"][2][4]; 
				M1W = j[_name]["Effects"][2][5]; 
				M1G = j[_name]["Effects"][2][6]; 
				M2C = j[_name]["Effects"][2][7]; 
				M2W = j[_name]["Effects"][2][8]; 
				M2G = j[_name]["Effects"][2][9];

				effectsTypes[lastFreeSlot()] = AudioEffect::EffectType::Equalizer;
                getEffect<Equalizer>().lowGain(LG);
				getEffect<Equalizer>().lowCutoff(LC);
				getEffect<Equalizer>().lowMidGain(M1G);
				getEffect<Equalizer>().lowMidCenter(M1C);
				getEffect<Equalizer>().lowMidWidth(M1W);
				getEffect<Equalizer>().highMidGain(M2G);
				getEffect<Equalizer>().highMidCenter(M2C);
				getEffect<Equalizer>().highMidWidth(M2W);
				getEffect<Equalizer>().highGain(HG);
				getEffect<Equalizer>().highCutoff(HC);
			}
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