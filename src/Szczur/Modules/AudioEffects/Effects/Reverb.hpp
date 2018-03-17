#pragma once

#include "EffectBase.hpp"

#include "Szczur/Modules/Script/Script.hpp"

namespace rat
{
	class Reverb : public EffectBase
	{

	public:

		Reverb(unsigned int& source);

		static void initScript(Script& script);

		//density: [Range: 0.0 - 1.0] [Default: 1.0]
		void density(float density);

		//diffusion: [Range: 0.0 - 1.0] [Default: 1.0]
		void diffusion(float diffusion);


		//gain: [Range: 0.0 - 1.0] [Default: 0.32]
		void gain(float gain);

		//gainHf: [Range: 0.0 - 1.0] [Default: 0.89]
		void gainHf(float gainHF);


		//decayTime: [Range: 0.1 - 20.0] [Default: 1.49]
		void decayTime(float decayTime);

		//decayHfRatio: [Range: 0.1 - 2.0] [Default: 0.83]
		void decayHfRatio(float decayHfRatio);


		//reflectionsGain: [Range: 0.0 - 3.16] [Default: 0.05]
		void reflectionsGain(float reflectionsGain);

		//reflectionsDelay: [Range: 0.0 - 0.3] [Default: 0.007]
		void reflectionsDelay(float reflectionsDelay);


		//lateReverbGain: [Range: 0.0 - 10.0] [Default: 1.26]
		void lateReverbGain(float lateReverbGain);

		//lateReverbDelay: [Range: 0.0 - 0.1] [Default: 0.011]
		void lateReverbDelay(float lateReverbDelay);


		//airAbsorptionGainHf: [Range: 0.892 - 1.0] [Default: 0.994]
		void airAbsorptionGainHf(float airAbsorptionGainHf);

		//roomRolloffFactor: [Range: 0.0 - 10.0] [Default: 0.0]
		void roomRolloffFactor(float roomRolloffFactor);


		//decayHfLimit: [Default: true]
		void decayHfLimit(bool decayHfLimit);

	};
}