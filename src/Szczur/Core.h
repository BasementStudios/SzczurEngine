#pragma once

#include "Canvas.h"

namespace rat {
	class Core {
	public:
		Canvas& canvas;
	private:
		static Core* ptrOnInstance;
	
	public:
		Core(Canvas& _canvas);
		static Canvas& getCanvas();
	};
}