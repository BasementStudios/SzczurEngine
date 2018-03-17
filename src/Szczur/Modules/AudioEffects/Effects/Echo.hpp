#pragma once

#include "EffectBase.hpp"

#include "Szczur/Modules/Script/Script.hpp"

namespace rat
{
	class Echo : public EffectBase
	{

	public:

		Echo(unsigned int& source);

		static void initScript(Script& script);

		//delay: [Range: 0.0 - 0.207] [Default: 0.1]
		void delay(float delay);
		
		//lrDelay: [Range: 0.0 - 0.404] [Default: 0.1]
		void lrDelay(float lrDelay);

		//damping: [Range: 0.0 - 0.99] [Default: 0.5]
		void damping(float damping);

		//feedback: [Range: 0.0 - 1.0] [Default: 0.5]
		void feedback(float feedback);

		//spread: [Range: -1.0 - 1.0] [Default: -1.0]
		void spread(float spread);

	};
}