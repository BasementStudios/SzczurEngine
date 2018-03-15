#include "TestObject.hpp"

namespace rat {
	
	ScriptableBase& TestObject::getBase() {
		return base;
	}

	void TestObject::loadScript(const std::string& filepath) {
		base.loadScript<TestObject>(this, filepath);
	}

	void TestObject::setScriptModule(Script& script) {
		auto object = base.prepare(script);		
		
		object.set("this", this);
		object.set("size", &size);
		object.set("pos", &pos);
		object.set("color", &color);
	}	
	
	void TestObject::initScript(Script& script) {
		auto object = script.newClass<TestObject>("TestObject", "ScriptEditor");
		
		object.set("pos", &TestObject::pos);
		object.set("size", &TestObject::size);
		object.set("color", &TestObject::color);
		object.set("base", &TestObject::base);
		
		object.set("_init", &TestObject::initFunc);
		object.set("_update", &TestObject::updateFunc);
		
		// For pack
		object.setProperty("pack", [](TestObject& owner){}, [](TestObject& owner){return owner.base.getPack();});
		
		object.init();
	}
	
}