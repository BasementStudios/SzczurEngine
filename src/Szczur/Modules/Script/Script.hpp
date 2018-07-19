#pragma once

#include <string>
#include <memory> // unique_ptr

#include "Szczur/Utility/Modules/Module.hpp"
#include "Szczur/Modules/Script/ScriptClass.hpp"

namespace rat
{

	class Script : public Module<>

	{
	private:

		sol::state _lua;

	public:

		inline static Script* _this;

		template <typename Tuple>
		Script(Tuple&& tuple);

		Script(const Script&) = delete;
		Script& operator = (const Script&) = delete;
		Script(Script&&) = delete;
		Script& operator = (Script&&) = delete;

		void init();

		void initMainFunctions();

		void initSFML();

		void scriptFile(const std::string& filePath);

		void script(const std::string& code);

		sol::state& get();

		template <typename T, typename U, typename ...Ts>
		void initClasses() {
			T::initScript(*this);
			initClasses<U, Ts...>();
		}

		template <typename T>
		void initClasses() {
			T::initScript(*this);
		}

		sol::table newModule(const std::string& moduleName, const std::string &scriptPath = "");

		template <typename T>
		auto newClass(const std::string& className, const std::string& moduleName, const std::string& scriptPath = "") {
			// sol::table module = _lua[moduleName];
			// auto object = module.create_simple_usertype<T>();
			auto ret = ScriptClass<T>(_lua, className, moduleName, scriptPath);
			ret.set("is", [](sol::object obj) {return obj.is<T*>() || obj.is<std::unique_ptr<T>>(); });
			return ret;
		}

		template <typename T>
		void initClass(ScriptClass<T>& scriptClass, const std::string& scriptPath = "") {
			sol::table module = _lua[scriptClass.moduleName];
			module.set_usertype(scriptClass.className, scriptClass.object);
			if (scriptPath != "") _lua.script_file(scriptPath);
		}

		static Script& ref();

		Script();

		~Script();
	};

}


// MACROOOOOS! <3

// BASE FOR OVERLOADING MACROS

#define OVERLOADED_MACRO(M, ...) _OVR(M, _COUNT_ARGS(__VA_ARGS__)) (__VA_ARGS__)
#define _OVR(macroName, number_of_args)   _OVR_EXPAND(macroName, number_of_args)
#define _OVR_EXPAND(macroName, number_of_args)    macroName##number_of_args

#define _COUNT_ARGS(...)  _ARG_PATTERN_MATCH(__VA_ARGS__, 17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1)
#define _ARG_PATTERN_MATCH(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17, N, ...)   N

// MODULE : SCRIPT_SET_MODULE_BODY

#define SCRIPT_SET_MODULE_BODY(c, f) module.set_function( #f , & c :: f , this);

// MODULE : SCRIPT_SET_MODULE (functions only)

#define SCRIPT_SET_MODULE(className, ...) OVERLOADED_MACRO(SCRIPT_SET_MODULE_, className, __VA_ARGS__)

#define SCRIPT_SET_MODULE_2(c, f) SCRIPT_SET_MODULE_BODY(c, f) 
#define SCRIPT_SET_MODULE_3(c, f, ...) SCRIPT_SET_MODULE_BODY(c, f) SCRIPT_SET_MODULE_2(c, __VA_ARGS__)
#define SCRIPT_SET_MODULE_4(c, f, ...) SCRIPT_SET_MODULE_BODY(c, f) SCRIPT_SET_MODULE_3(c, __VA_ARGS__)
#define SCRIPT_SET_MODULE_5(c, f, ...) SCRIPT_SET_MODULE_BODY(c, f) SCRIPT_SET_MODULE_4(c, __VA_ARGS__)
#define SCRIPT_SET_MODULE_6(c, f, ...) SCRIPT_SET_MODULE_BODY(c, f) SCRIPT_SET_MODULE_5(c, __VA_ARGS__)
#define SCRIPT_SET_MODULE_7(c, f, ...) SCRIPT_SET_MODULE_BODY(c, f) SCRIPT_SET_MODULE_6(c, __VA_ARGS__)
#define SCRIPT_SET_MODULE_8(c, f, ...) SCRIPT_SET_MODULE_BODY(c, f) SCRIPT_SET_MODULE_7(c, __VA_ARGS__)
#define SCRIPT_SET_MODULE_9(c, f, ...) SCRIPT_SET_MODULE_BODY(c, f) SCRIPT_SET_MODULE_8(c, __VA_ARGS__)
#define SCRIPT_SET_MODULE_10(c, f, ...) SCRIPT_SET_MODULE_BODY(c, f) SCRIPT_SET_MODULE_9(c, __VA_ARGS__)
#define SCRIPT_SET_MODULE_11(c, f, ...) SCRIPT_SET_MODULE_BODY(c, f) SCRIPT_SET_MODULE_10(c, __VA_ARGS__)
#define SCRIPT_SET_MODULE_12(c, f, ...) SCRIPT_SET_MODULE_BODY(c, f) SCRIPT_SET_MODULE_11(c, __VA_ARGS__)
#define SCRIPT_SET_MODULE_13(c, f, ...) SCRIPT_SET_MODULE_BODY(c, f) SCRIPT_SET_MODULE_12c, __VA_ARGS__)
#define SCRIPT_SET_MODULE_14(c, f, ...) SCRIPT_SET_MODULE_BODY(c, f) SCRIPT_SET_MODULE_13(c, __VA_ARGS__)
#define SCRIPT_SET_MODULE_15(c, f, ...) SCRIPT_SET_MODULE_BODY(c, f) SCRIPT_SET_MODULE_14(c, __VA_ARGS__)
#define SCRIPT_SET_MODULE_16(c, f, ...) SCRIPT_SET_MODULE_BODY(c, f) SCRIPT_SET_MODULE_15(c, __VA_ARGS__)
#define SCRIPT_SET_MODULE_17(c, f, ...) SCRIPT_SET_MODULE_BODY(c, f) SCRIPT_SET_MODULE_16(c, __VA_ARGS__)

// MODULE : SCRIPT_NEW_MODULE

#define SCRIPT_NEW_MODULE(...) OVERLOADED_MACRO(SCRIPT_NEW_MODULE_, __VA_ARGS__)

#define SCRIPT_NEW_MODULE_1(className) sol::state& lua = getModule<Script>().get(); auto module=lua.create_table(#className);
#define SCRIPT_NEW_MODULE_2(className,scriptPath) SCRIPT_NEW_MODULE_1(className) lua.script_file(scriptPath);

// CLASS : SCRIPT_SET_CLASS_BODY

#define SCRIPT_SET_CLASS_BODY(c, m) object.set( #m , & c :: m);

// CLASS : SCRIPT_SET_CLASS

#define SCRIPT_SET_CLASS(className, ...) OVERLOADED_MACRO(SCRIPT_SET_CLASS_, className, __VA_ARGS__)

#define SCRIPT_SET_CLASS_2(c, m) SCRIPT_SET_CLASS_BODY(c, m)
#define SCRIPT_SET_CLASS_3(c, m, ...) SCRIPT_SET_CLASS_BODY(c, m) SCRIPT_SET_CLASS_2(c, __VA_ARGS__)
#define SCRIPT_SET_CLASS_4(c, m, ...) SCRIPT_SET_CLASS_BODY(c, m) SCRIPT_SET_CLASS_3(c, __VA_ARGS__)
#define SCRIPT_SET_CLASS_5(c, m, ...) SCRIPT_SET_CLASS_BODY(c, m) SCRIPT_SET_CLASS_4(c, __VA_ARGS__)
#define SCRIPT_SET_CLASS_6(c, m, ...) SCRIPT_SET_CLASS_BODY(c, m) SCRIPT_SET_CLASS_5(c, __VA_ARGS__)
#define SCRIPT_SET_CLASS_7(c, m, ...) SCRIPT_SET_CLASS_BODY(c, m) SCRIPT_SET_CLASS_6(c, __VA_ARGS__)
#define SCRIPT_SET_CLASS_8(c, m, ...) SCRIPT_SET_CLASS_BODY(c, m) SCRIPT_SET_CLASS_7(c, __VA_ARGS__)
#define SCRIPT_SET_CLASS_9(c, m, ...) SCRIPT_SET_CLASS_BODY(c, m) SCRIPT_SET_CLASS_8(c, __VA_ARGS__)
#define SCRIPT_SET_CLASS_10(c, m, ...) SCRIPT_SET_CLASS_BODY(c, m) SCRIPT_SET_CLASS_9(c, __VA_ARGS__)
#define SCRIPT_SET_CLASS_11(c, m, ...) SCRIPT_SET_CLASS_BODY(c, m) SCRIPT_SET_CLASS_10(c, __VA_ARGS__)
#define SCRIPT_SET_CLASS_12(c, m, ...) SCRIPT_SET_CLASS_BODY(c, m) SCRIPT_SET_CLASS_11(c, __VA_ARGS__)
#define SCRIPT_SET_CLASS_13(c, m, ...) SCRIPT_SET_CLASS_BODY(c, m) SCRIPT_SET_CLASS_12(c, __VA_ARGS__)
#define SCRIPT_SET_CLASS_14(c, m, ...) SCRIPT_SET_CLASS_BODY(c, m) SCRIPT_SET_CLASS_13(c, __VA_ARGS__)
#define SCRIPT_SET_CLASS_15(c, m, ...) SCRIPT_SET_CLASS_BODY(c, m) SCRIPT_SET_CLASS_14(c, __VA_ARGS__)
#define SCRIPT_SET_CLASS_16(c, m, ...) SCRIPT_SET_CLASS_BODY(c, m) SCRIPT_SET_CLASS_15(c, __VA_ARGS__)
#define SCRIPT_SET_CLASS_17(c, m, ...) SCRIPT_SET_CLASS_BODY(c, m) SCRIPT_SET_CLASS_16(c, __VA_ARGS__)
