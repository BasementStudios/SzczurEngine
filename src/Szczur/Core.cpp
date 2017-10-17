#include "Core.h"

namespace rat {
	Core* Core::ptrOnInstance = nullptr;
	Core::Core(Canvas& _canvas) :
		canvas(_canvas) {
		if(!ptrOnInstance) {
			ptrOnInstance = this;
		}
	}
	Canvas& Core::getCanvas() { return ptrOnInstance->canvas; }
}