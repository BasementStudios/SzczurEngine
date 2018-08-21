#pragma once

#include <iostream>

#include <sol.hpp>

namespace rat
{

template <typename T>
class ScriptClass {
private:
	
	std::string _className;
	std::string _moduleName;
	std::string _scriptPath = "";
	
	sol::state &_lua;
	
	sol::simple_usertype<T> _object;
	
public:

	ScriptClass(sol::state &lua, const std::string& className, const std::string& moduleName, const std::string& scriptPath = "") :
		_lua(lua), _object(_lua.create_simple_usertype<T>()) {
		
		_className = className;
		_moduleName = moduleName;
		_scriptPath = scriptPath;
	}
	
	template <typename N, typename A>
	void set(N&& memberName, A&& memberAddress) {
		_object.set(memberName, memberAddress);
	}
	
	template <typename N, typename GA, typename SA>
	void setProperty(N&& memberName, GA&& getterAddress, SA&& setterAddress) {
		_object.set(memberName, sol::property(getterAddress, setterAddress));
	}
	
	template <typename N, typename ...Fs>
	void setOverload(N&& memberName,  Fs&&... functions) {
		_object.set(memberName, sol::overload(functions...));
	}
	
	void makeStatter() {
		_object.set("setStat", &T::setStat);
		setProperty("stat", [](T& o)->T&{return o;}, &T::setStat);
	}
	
	void makeInstance() {
		sol::state *_luap = &_lua;
		_object.set("instance", [lp = _luap, n = _moduleName, c = _className](){
			auto object = new T();
			sol::function init = (*lp)[n][c]["_instance"];
			if(init.valid()) init(object);
			return std::unique_ptr<T>(object);
		});
	}
	
	void init() {		
		_object.set("is", [](sol::object obj) {return obj.is<T*>() || obj.is<std::unique_ptr<T>>();});
		
		_lua.get<sol::table>(_moduleName).set_usertype(_className, _object);
		
		if(_scriptPath != "") _lua.script_file(_scriptPath);
	}
};

}