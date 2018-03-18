#include "MiniObject.hpp"

namespace rat {

	MiniObject::MiniObject(Script &script) 
		: script(script), frame(pos) {	
	}	
	
/////////////////////////// *ETTERS ///////////////////////////
	
	void MiniObject::setPosition(const sf::Vector2f& _pos) {
		pos = _pos;
	}
	const sf::Vector2f& MiniObject::getPosition(){
		return pos;
	}
	void MiniObject::setName(const std::string& _name) {
		name = _name;
	}	
	const std::string& MiniObject::getName() {
		return name;
	}
	MiniFrame& MiniObject::getFrame() {
		return frame;
	}
	
/////////////////////////// SCRIPT ///////////////////////////

	// void MiniObject::runFileScript(const std::string& filepath) {
		// auto& lua = script.get();
		// lua.set("THIS", scriptComponents);
		// lua.script_file(filepath);
	// }
	
	// void MiniObject::runScript(const std::string& code) {
		// auto& lua = script.get();
		// lua.set("THIS", scriptComponents);
		// lua.script(code);
	// }
	
	// void MiniObject::initScript(Script &script) {
		// auto object = script.newClass<MiniObject>("MiniObject", "MiniWorld");
		// object.init();
		// initComponents(script);
	// }	
}