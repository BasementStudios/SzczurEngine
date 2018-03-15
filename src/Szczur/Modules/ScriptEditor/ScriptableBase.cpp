#include "ScriptableBase.hpp"

namespace rat {	
	
	sol::table ScriptableBase::prepare(Script& script) {
		this->script = &script;
		sol::state& state = script.get();
		pack = sol::table::create(state.lua_state());
		return pack;
	}
}
