#pragma once

#include "MusicEffect.hpp"

namespace rat
{
	class Equalizer : public MusicEffect
	{

	public:

		Equalizer(unsigned int& source);

		//gain: [Range: 0.126 - 7.943] [Default: 1.0]
		//cutoff: [Range: 50.0 - 800.0] [Default: 200.0]
		void low(float gain, float cutoff = 0);

		//gain: [Range: 0.126 - 7.943] [Default: 1.0]
		//center: [Range: 200.0 - 3000.0] [Default: 500.0]
		//width: [Range: 0.01 - 1.0] [Default: 1.0]
		void lowMid(float gain, float center = 0, float width = 0);

		//gain: [Range: 0.126 - 7.943] [Default: 1.0]
		//center: [Range: 1000.0 - 8000.0] [Default: 3000.0]
		//width: [Range: 0.01 - 1.0] [Default: 1.0]
		void highMid(float gain, float center = 0, float width = 0);

		//gain: [Range: 0.126 - 7.943] [Default: 1.0]
		//cutoff: [Range: 4000.0 - 16000.0] [Default: 6000.0]
		void high(float gain, float cutoff = 0);


		//cutoff: [Range: 50.0 - 800.0] [Default: 200.0]
		void lowCutoff(float cutoff);

		//gain: [Range: 0.126 - 7.943] [Default: 1.0]
		void lowGain(float gain);


		//width: [Range: 0.01 - 1.0] [Default: 1.0]
		void lowMidWidth(float width);

		//center: [Range: 200.0 - 3000.0] [Default: 500.0]
		void lowMidCenter(float center);

		//gain: [Range: 0.126 - 7.943] [Default: 1.0]
		void lowMidGain(float gain);


		//width: [Range: 0.01 - 1.0] [Default: 1.0]
		void highMidWidth(float width);

		//center: [Range: 1000.0 - 8000.0] [Default: 3000.0]
		void highMidCenter(float center);

		//gain: [Range: 0.126 - 7.943] [Default: 1.0]
		void highMidGain(float gain);


		//delay: [Range: 0.0 - 0.207] [Default: 0.1]
		void highCutoff(float cutoff);

		//cutoff: [Range: 4000.0 - 16000.0] [Default: 6000.0]
		void highGain(float gain);

	};
}