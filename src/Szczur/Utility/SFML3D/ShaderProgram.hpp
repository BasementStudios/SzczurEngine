#pragma once

#include "Szczur/Config.hpp"

#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat2x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>

#include <nlohmann/json_fwd.hpp>

#ifdef EDITOR
#   include <map>
#   include <string>
#   include <variant>
#endif // EDITOR

#include "Shader.hpp"

namespace sf3d
{

class ShaderProgram
{
public:

	using NativeHandle_t = GLuint;

	///
	ShaderProgram() = default;

	// Non-copyable
	ShaderProgram(const ShaderProgram&) = delete;
	ShaderProgram& operator = (const ShaderProgram&) = delete;

	// Movable
	ShaderProgram(ShaderProgram&& rhs) noexcept;
	ShaderProgram& operator = (ShaderProgram&& rhs) noexcept;

	///
	~ShaderProgram();

	///
	template <typename... Ts>
	ShaderProgram(Ts&&... shaders);

	///
	template <typename... Ts>
	void linkShaders(Ts&&... shaders);

	///
	bool setUniform(const char* name, bool value);

	///
	bool setUniform(const char* name, const glm::bvec2& value);

	///
	bool setUniform(const char* name, const glm::bvec3& value);

	///
	bool setUniform(const char* name, const glm::bvec4& value);

	///
	bool setUniform(const char* name, int value);

	///
	bool setUniform(const char* name, const glm::ivec2& value);

	///
	bool setUniform(const char* name, const glm::ivec3& value);

	///
	bool setUniform(const char* name, const glm::ivec4& value);

	///
	bool setUniform(const char* name, unsigned int value);

	///
	bool setUniform(const char* name, const glm::uvec2& value);

	///
	bool setUniform(const char* name, const glm::uvec3& value);

	///
	bool setUniform(const char* name, const glm::uvec4& value);

	///
	bool setUniform(const char* name, float value);

	///
	bool setUniform(const char* name, const glm::vec2& value);

	///
	bool setUniform(const char* name, const glm::vec3& value);

	///
	bool setUniform(const char* name, const glm::vec4& value);

	///
	bool setUniform(const char* name, const glm::mat2x2& value, bool transpose = GL_FALSE);

	///
	bool setUniform(const char* name, const glm::mat3x3& value, bool transpose = GL_FALSE);

	///
	bool setUniform(const char* name, const glm::mat4x4& value, bool transpose = GL_FALSE);

	///
	void loadConfig(const nlohmann::json& config);

	///
	void loadConfig(const char* path);

	///
	bool isValid() const;

	///
	NativeHandle_t getNativeHandle() const;

	#ifdef EDITOR

	using UniKey_t     = std::string;
	using UniVariant_t = std::variant<bool, glm::bvec2, glm::bvec3, glm::bvec4, int, glm::ivec2, glm::ivec3, glm::ivec4, unsigned, glm::uvec2, glm::uvec3, glm::uvec4, float, glm::vec2, glm::vec3, glm::vec4, glm::mat2x2, glm::mat3x3, glm::mat4x4>;
	using UniMap_t     = std::map<UniKey_t, UniVariant_t>;

	///
	void _showEditor(bool* open);

	///
	void _saveConfig(nlohmann::json& config) const;

	///
	void _saveConfig(const char* path) const;

	UniMap_t _uniforms;
	UniMap_t::iterator _currentElem = _uniforms.end();
	const char* const _uniTypeNames[std::variant_size_v<UniVariant_t>] = { "bool", "bvec2", "bvec3", "bvec4", "int", "ivec2", "ivec3", "ivec4", "uint", "uvec2", "uvec3", "uvec4", "float", "vec2", "vec3", "vec4", "mat2x2", "mat3x3", "mat4x4" };

	#endif // EDITOR

private:

	///
	void _destroy();

	///
	void _finishLinking();

	NativeHandle_t _program = 0;

};

template <typename... Ts>
ShaderProgram::ShaderProgram(Ts&&... shaders)
{
	linkShaders(std::forward<Ts>(shaders)...);
}

template <typename... Ts>
void ShaderProgram::linkShaders(Ts&&... shaders)
{
	static_assert((std::is_same_v<Shader, std::remove_cv_t<std::remove_reference_t<Ts>>> && ...), "All Ts must be exactly sf3d::Shader");

	_destroy();

	_program = glCreateProgram();

	(glAttachShader(_program, shaders.getNativeHandle()), ...);

	glLinkProgram(_program);

	(glDetachShader(_program, shaders.getNativeHandle()), ...);

	_finishLinking();
}

}
