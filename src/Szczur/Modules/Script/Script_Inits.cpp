#include "Script.hpp"

#include <sol.hpp>

#include <glm/glm.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Clock.hpp>

namespace rat {

	void Script::initSFML() {

		// SFML
		sol::table sfml = _lua.create_table("Sfml");

		// SFML : Vector2f		
		sfml.new_simple_usertype<sf::Vector2f>("Vector2f",
			"x", &sf::Vector2f::x,
			"y", &sf::Vector2f::y
		);
		// SFML : Vector2i
		sfml.new_simple_usertype<sf::Vector2i>("Vector2i",
			"x", &sf::Vector2i::x,
			"y", &sf::Vector2i::y
		);
		// SFML : Color
		sfml.new_simple_usertype<sf::Color>("Color",
			"r", &sf::Color::r,
			"g", &sf::Color::g,
			"b", &sf::Color::b,
			"a", &sf::Color::a
		);
	
		// Math library
		auto mathTab = _lua.get<sol::table>("Math");
		mathTab.set_function("atan2", sol::resolve<float(float, float)>(std::atan2));

		// sf::Clock
		auto moduleUtility = newModule("Utility");
		auto classClock = newClass<sf::Clock>("Clock", "Utility");
		classClock.set("new", sol::constructors<sf::Clock()>());
		classClock.set("restart", &sf::Clock::restart);
		classClock.set("elapsed", [](sf::Clock& clock){return clock.getElapsedTime().asSeconds();});
		classClock.init();
	}

	void Script::initGLM() {

		// GLM
		sol::table glmTable = _lua.create_table("GLM");

	// vec2, vec3, vec4

		// Color (Vec4)
		glmTable.new_simple_usertype<glm::vec4>("Color",
			// Constructor
			sol::call_constructor, sol::constructors<glm::vec4(const float&, const float&, const float&, const float&)>(),

			// Multiplication
			sol::meta_function::multiplication, sol::overload(
				[](const glm::vec4& v, float f){return v*f;},
				[](float f, const glm::vec4& v){return f*v;},
				[](const glm::vec4& v1, const glm::vec4& v2){return v1*v2;}
			),

			// Division
			sol::meta_function::division, sol::overload(
				[](const glm::vec4& v, float f){return v/f;},
				[](float f, const glm::vec4& v){return f/v;},
				[](const glm::vec4& v1, const glm::vec4& v2){return v1/v2;}
			),

			// Addition
			sol::meta_function::addition, sol::overload(
				[](const glm::vec4& v, float f){return v+f;},
				[](float f, const glm::vec4& v){return f+v;},
				[](const glm::vec4& v1, const glm::vec4& v2){return v1+v2;}
			),

			// Subtraction
			sol::meta_function::subtraction, sol::overload(
				[](const glm::vec4& v, float f){return v-f;},
				[](float f, const glm::vec4& v){return f-v;},
				[](const glm::vec4& v1, const glm::vec4& v2){return v1-v2;}
			),
			
			// Members
			"r", &glm::vec4::r,
			"g", &glm::vec4::g,
			"b", &glm::vec4::b,
			"a", &glm::vec4::a
		);

		// Vec3
		glmTable.new_simple_usertype<glm::vec3>("Vec3",
			// Constructor
			sol::call_constructor, sol::constructors<glm::vec3(const float&, const float&, const float&)>(),

			// Multiplication
			sol::meta_function::multiplication, sol::overload(
				[](const glm::vec3& v, float f){return v*f;},
				[](float f, const glm::vec3& v){return f*v;},
				[](const glm::vec3& v1, const glm::vec3& v2){return v1*v2;}
			),

			// Division
			sol::meta_function::division, sol::overload(
				[](const glm::vec3& v, float f){return v/f;},
				[](float f, const glm::vec3& v){return f/v;},
				[](const glm::vec3& v1, const glm::vec3& v2){return v1/v2;}
			),

			// Addition
			sol::meta_function::addition, sol::overload(
				[](const glm::vec3& v, float f){return v+f;},
				[](float f, const glm::vec3& v){return f+v;},
				[](const glm::vec3& v1, const glm::vec3& v2){return v1+v2;}
			),

			// Subtraction
			sol::meta_function::subtraction, sol::overload(
				[](const glm::vec3& v, float f){return v-f;},
				[](float f, const glm::vec3& v){return f-v;},
				[](const glm::vec3& v1, const glm::vec3& v2){return v1-v2;}
			),			

			// Members
			"x", &glm::vec3::x,
			"y", &glm::vec3::y,
			"z", &glm::vec3::z
		);

		// Vec2
		glmTable.new_simple_usertype<glm::vec2>("Vec2",
			// Constructor
			sol::call_constructor, sol::constructors<glm::vec2(const float&, const float&)>(),

			// Multiplication
			sol::meta_function::multiplication, sol::overload(
				[](const glm::vec2& v, float f){return v*f;},
				[](float f, const glm::vec2& v){return f*v;},
				[](const glm::vec2& v1, const glm::vec2& v2){return v1*v2;}
			),

			// Division
			sol::meta_function::division, sol::overload(
				[](const glm::vec2& v, float f){return v/f;},
				[](float f, const glm::vec2& v){return f/v;},
				[](const glm::vec2& v1, const glm::vec2& v2){return v1/v2;}
			),

			// Addition
			sol::meta_function::addition, sol::overload(
				[](const glm::vec2& v, float f){return v+f;},
				[](float f, const glm::vec2& v){return f+v;},
				[](const glm::vec2& v1, const glm::vec2& v2){return v1+v2;}
			),

			// Subtraction
			sol::meta_function::subtraction, sol::overload(
				[](const glm::vec2& v, float f){return v-f;},
				[](float f, const glm::vec2& v){return f-v;},
				[](const glm::vec2& v1, const glm::vec2& v2){return v1-v2;}
			),	

			// Members
			"x", &glm::vec2::x,
			"y", &glm::vec2::y
		);

	// pi
		glmTable.set_function("pi", [](){return 3.141592;});

	// sin, cos, tan
		glmTable.set_function("sin", [](float x){return glm::sin(x);});
		glmTable.set_function("cos", [](float x){return glm::cos(x);});
		glmTable.set_function("tan", [](float x){return glm::tan(x);});

	// asin, acos, atan, atan2
		glmTable.set_function("asin", [](float x){return glm::asin(x);});
		glmTable.set_function("acos", [](float x){return glm::acos(x);});
		glmTable.set_function("atan", [](float x){return glm::atan(x);});
		glmTable.set_function("atan2", [](float y, float x){return glm::atan(y,x);});

	// distance
		glmTable.set_function("distance", [](glm::vec2 p1, glm::vec2 p2){return glm::distance(p1, p2);});

	// length
		glmTable.set_function("length", [](glm::vec2 v){return glm::length(v);});

	// normalize
		glmTable.set_function("normalize", [](glm::vec2 vec){return glm::normalize(vec);});

	// angle
		glmTable.set_function("angle", sol::overload(
			[](glm::vec2 p1, glm::vec2 p2){
				glm::vec2 delta = p2 - p1;
				return glm::atan(delta.y, delta.x);
			},
			[](glm::vec2 v){
				return glm::atan(v.y, v.x);
			}
		));

	// pow
		glmTable.set_function("pow", [](float x, float n){return glm::pow(x, n);});

	// sqr	
		glmTable.set_function("sqr", [](float x){return x*x;});

	// sqrt
		glmTable.set_function("sqrt", [](float x){return glm::sqrt(x);});

	// radians
	glmTable.set_function("radians", [](float x){return glm::radians(x);});

	// degrees
	glmTable.set_function("degrees", [](float x){return glm::degrees(x);});
	
	}
}