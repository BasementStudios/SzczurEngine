#include "ShaderProgram.hpp"

#include <fstream>

#include <nlohmann/json.hpp>

#include <glm/gtc/type_ptr.hpp>

#ifdef EDITOR
#	include <imgui.h>
#	include <imgui-SFML.h>
#endif // EDITOR

#include "Szczur/Utility/Logger.hpp"
#include "Szczur/Utility/Convert/Hash.hpp"

namespace nlohmann
{

///
template <glm::length_t L, typename T, glm::qualifier Q>
struct adl_serializer<glm::vec<L, T, Q>>
{
	///
	template <size_t... Ns>
	static void serialize(nlohmann::json& j, const glm::vec<L, T, Q>& value, std::index_sequence<Ns...>)
	{
		((j[Ns] = value[Ns]), ...);
	}

	///
	static void to_json(nlohmann::json& j, const glm::vec<L, T, Q>& value)
	{
		serialize(j, value, std::make_index_sequence<L>{});
	}

	///
	template <size_t... Ns>
	static void deserialize(const nlohmann::json& j, glm::vec<L, T, Q>& value, std::index_sequence<Ns...>)
	{
		((value[Ns] = j[Ns]), ...);
	}

	///
	static void from_json(const nlohmann::json& j, glm::vec<L, T, Q>& value)
	{
		deserialize(j, value, std::make_index_sequence<L>{});
	}

};

///
template <glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
struct adl_serializer<glm::mat<C, R, T, Q>>
{
	///
	template <size_t... Ns>
	static void serialize(nlohmann::json& j, const glm::mat<C, R, T, Q>& value, std::index_sequence<Ns...>)
	{
		((j[Ns] = value[Ns]), ...);
	}

	///
	static void to_json(nlohmann::json& j, const glm::mat<C, R, T, Q>& value)
	{
		serialize(j, value, std::make_index_sequence<C>{});
	}

	///
	template <size_t... Ns>
	static void deserialize(const nlohmann::json& j, glm::mat<C, R, T, Q>& value, std::index_sequence<Ns...>)
	{
		((value[Ns] = j[Ns]), ...);
	}

	///
	static void from_json(const nlohmann::json& j, glm::mat<C, R, T, Q>& value)
	{
		deserialize(j, value, std::make_index_sequence<C>{});
	}

};

}

template <typename... Ts>
struct Overloaded : Ts... { using Ts::operator()...; };

template <typename... Ts>
Overloaded(Ts&&...) -> Overloaded<Ts...>;

namespace sf3d
{

///
class UniformBinder
{
public:

	///
	UniformBinder(GLuint program, const char* name)
		: _savedProgram { 0 }
		, _program { program }
		, _location { -1 }
	{
		if (_program)
		{
			GLint currentProgram;
			glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);

			_savedProgram = currentProgram;

			if (_program != _savedProgram)
			{
				glUseProgram(_program);
			}

			_location = glGetUniformLocation(_program, name);
		}
	}

	///
	UniformBinder(const UniformBinder&) = delete;

	///
	UniformBinder& operator = (const UniformBinder&) = delete;

	///
	UniformBinder(UniformBinder&& rhs)  = delete;

	///
	UniformBinder& operator = (UniformBinder&& rhs)  = delete;

	///
	~UniformBinder()
	{
		if (_program && (_program != _savedProgram))
		{
			glUseProgram(_savedProgram);
		}
	}

	///
	GLint getLocation() const
	{
		return _location;
	}

	///
	bool hasValidLocation() const
	{
		return _location != -1;
	}

private:

	GLuint _savedProgram;
	GLuint _program;
	GLint _location;

};

ShaderProgram::ShaderProgram(ShaderProgram&& rhs) noexcept
	: _program { rhs._program }
{
	rhs._program = 0;
}

ShaderProgram& ShaderProgram::operator = (ShaderProgram&& rhs) noexcept
{
	if (this != &rhs)
	{
		_destroy();

		_program = rhs._program;
		rhs._program = 0;
	}

	return *this;
}

ShaderProgram::~ShaderProgram()
{
	_destroy();
}

bool ShaderProgram::setUniform(const char* name, bool value)
{
	UniformBinder binder{ _program, name };

	if (binder.hasValidLocation())
	{
		#ifdef EDITOR
		{
			uniforms[name] = value;
		}
		#endif // EDITOR

		glUniform1i(binder.getLocation(), value);

		return true;
	}

	return false;
}

bool ShaderProgram::setUniform(const char* name, const glm::bvec2& value)
{
	UniformBinder binder{ _program, name };

	if (binder.hasValidLocation())
	{
		#ifdef EDITOR
		{
			uniforms[name] = value;
		}
		#endif // EDITOR

		glUniform2i(binder.getLocation(), value.x, value.y);

		return true;
	}

	return false;
}

bool ShaderProgram::setUniform(const char* name, const glm::bvec3& value)
{
	UniformBinder binder{ _program, name };

	if (binder.hasValidLocation())
	{
		#ifdef EDITOR
		{
			uniforms[name] = value;
		}
		#endif // EDITOR

		glUniform3i(binder.getLocation(), value.x, value.y, value.z);

		return true;
	}

	return false;
}

bool ShaderProgram::setUniform(const char* name, const glm::bvec4& value)
{
	UniformBinder binder{ _program, name };

	if (binder.hasValidLocation())
	{
		#ifdef EDITOR
		{
			uniforms[name] = value;
		}
		#endif // EDITOR

		glUniform4i(binder.getLocation(), value.x, value.y, value.z, value.w);

		return true;
	}

	return false;
}

bool ShaderProgram::setUniform(const char* name, int value)
{
	UniformBinder binder{ _program, name };

	if (binder.hasValidLocation())
	{
		#ifdef EDITOR
		{
			uniforms[name] = value;
		}
		#endif // EDITOR

		glUniform1i(binder.getLocation(), value);

		return true;
	}

	return false;
}

bool ShaderProgram::setUniform(const char* name, const glm::ivec2& value)
{
	UniformBinder binder{ _program, name };

	if (binder.hasValidLocation())
	{
		#ifdef EDITOR
		{
			uniforms[name] = value;
		}
		#endif // EDITOR

		glUniform2i(binder.getLocation(), value.x, value.y);

		return true;
	}

	return false;
}

bool ShaderProgram::setUniform(const char* name, const glm::ivec3& value)
{
	UniformBinder binder{ _program, name };

	if (binder.hasValidLocation())
	{
		#ifdef EDITOR
		{
			uniforms[name] = value;
		}
		#endif // EDITOR

		glUniform3i(binder.getLocation(), value.x, value.y, value.z);

		return true;
	}

	return false;
}

bool ShaderProgram::setUniform(const char* name, const glm::ivec4& value)
{
	UniformBinder binder{ _program, name };

	if (binder.hasValidLocation())
	{
		#ifdef EDITOR
		{
			uniforms[name] = value;
		}
		#endif // EDITOR

		glUniform4i(binder.getLocation(), value.x, value.y, value.z, value.w);

		return true;
	}

	return false;
}

bool ShaderProgram::setUniform(const char* name, unsigned int value)
{
	UniformBinder binder{ _program, name };

	if (binder.hasValidLocation())
	{
		#ifdef EDITOR
		{
			uniforms[name] = value;
		}
		#endif // EDITOR

		glUniform1ui(binder.getLocation(), value);

		return true;
	}

	return false;
}

bool ShaderProgram::setUniform(const char* name, const glm::uvec2& value)
{
	UniformBinder binder{ _program, name };

	if (binder.hasValidLocation())
	{
		#ifdef EDITOR
		{
			uniforms[name] = value;
		}
		#endif // EDITOR

		glUniform2ui(binder.getLocation(), value.x, value.y);

		return true;
	}

	return false;
}

bool ShaderProgram::setUniform(const char* name, const glm::uvec3& value)
{
	UniformBinder binder{ _program, name };

	if (binder.hasValidLocation())
	{
		#ifdef EDITOR
		{
			uniforms[name] = value;
		}
		#endif // EDITOR

		glUniform3ui(binder.getLocation(), value.x, value.y, value.z);

		return true;
	}

	return false;
}

bool ShaderProgram::setUniform(const char* name, const glm::uvec4& value)
{
	UniformBinder binder{ _program, name };

	if (binder.hasValidLocation())
	{
		#ifdef EDITOR
		{
			uniforms[name] = value;
		}
		#endif // EDITOR

		glUniform4ui(binder.getLocation(), value.x, value.y, value.z, value.w);

		return true;
	}

	return false;
}

bool ShaderProgram::setUniform(const char* name, float value)
{
	UniformBinder binder{ _program, name };

	if (binder.hasValidLocation())
	{
		#ifdef EDITOR
		{
			uniforms[name] = value;
		}
		#endif // EDITOR

		glUniform1f(binder.getLocation(), value);

		return true;
	}

	return false;
}

bool ShaderProgram::setUniform(const char* name, const glm::vec2& value)
{
	UniformBinder binder{ _program, name };

	if (binder.hasValidLocation())
	{
		#ifdef EDITOR
		{
			uniforms[name] = value;
		}
		#endif // EDITOR

		glUniform2f(binder.getLocation(), value.x, value.y);

		return true;
	}

	return false;
}

bool ShaderProgram::setUniform(const char* name, const glm::vec3& value)
{
	UniformBinder binder{ _program, name };

	if (binder.hasValidLocation())
	{
		#ifdef EDITOR
		{
			uniforms[name] = value;
		}
		#endif // EDITOR

		glUniform3f(binder.getLocation(), value.x, value.y, value.z);

		return true;
	}

	return false;
}

bool ShaderProgram::setUniform(const char* name, const glm::vec4& value)
{
	UniformBinder binder{ _program, name };

	if (binder.hasValidLocation())
	{
		#ifdef EDITOR
		{
			uniforms[name] = value;
		}
		#endif // EDITOR

		glUniform4f(binder.getLocation(), value.x, value.y, value.z, value.w);

		return true;
	}

	return false;
}

bool ShaderProgram::setUniform(const char* name, const glm::mat2x2& value, bool transpose)
{
	UniformBinder binder{ _program, name };

	if (binder.hasValidLocation())
	{
		#ifdef EDITOR
		{
			uniforms[name] = value;
		}
		#endif // EDITOR

		glUniformMatrix2fv(binder.getLocation(), 1, transpose, glm::value_ptr(value));

		return true;
	}

	return false;
}

bool ShaderProgram::setUniform(const char* name, const glm::mat3x3& value, bool transpose)
{
	UniformBinder binder{ _program, name };

	if (binder.hasValidLocation())
	{
		#ifdef EDITOR
		{
			uniforms[name] = value;
		}
		#endif // EDITOR

		glUniformMatrix3fv(binder.getLocation(), 1, transpose, glm::value_ptr(value));

		return true;
	}

	return false;
}

bool ShaderProgram::setUniform(const char* name, const glm::mat4x4& value, bool transpose)
{
	UniformBinder binder{ _program, name };

	if (binder.hasValidLocation())
	{
		#ifdef EDITOR
		{
			uniforms[name] = value;
		}
		#endif // EDITOR

		glUniformMatrix4fv(binder.getLocation(), 1, transpose, glm::value_ptr(value));

		return true;
	}

	return false;
}

void ShaderProgram::loadConfig(const nlohmann::json& config)
{
	#define case_str(__x) case rat::fnv1a_32(__x)

	for (auto it = config.begin(); it != config.end(); ++it)
	{
		const char* name = it.key().data();
		const std::string type = it.value()["type"];
		const nlohmann::json& value = it.value()["value"];

		switch (rat::fnv1a_32(type.begin(), type.end()))
		{
			case_str("bool"):
			{
				setUniform(name, value.get<bool>());
			}
			break;
			case_str("bvec2"):
			{
				setUniform(name, value.get<glm::bvec2>());
			}
			break;
			case_str("bvec3"):
			{
				setUniform(name, value.get<glm::bvec3>());
			}
			break;
			case_str("bvec4"):
			{
				setUniform(name, value.get<glm::bvec4>());
			}
			break;
			case_str("int"):
			{
				setUniform(name, value.get<int>());
			}
			break;
			case_str("ivec2"):
			{
				setUniform(name, value.get<glm::ivec2>());
			}
			break;
			case_str("ivec3"):
			{
				setUniform(name, value.get<glm::ivec3>());
			}
			break;
			case_str("ivec4"):
			{
				setUniform(name, value.get<glm::ivec4>());
			}
			break;
			case_str("uint"):
			{
				setUniform(name, value.get<unsigned int>());
			}
			break;
			case_str("uvec2"):
			{
				setUniform(name, value.get<glm::uvec2>());
			}
			break;
			case_str("uvec3"):
			{
				setUniform(name, value.get<glm::uvec3>());
			}
			break;
			case_str("uvec4"):
			{
				setUniform(name, value.get<glm::uvec4>());
			}
			break;
			case_str("float"):
			{
				setUniform(name, value.get<float>());
			}
			break;
			case_str("vec2"):
			{
				setUniform(name, value.get<glm::vec2>());
			}
			break;
			case_str("vec3"):
			{
				setUniform(name, value.get<glm::vec3>());
			}
			break;
			case_str("vec4"):
			{
				setUniform(name, value.get<glm::vec4>());
			}
			break;
			case_str("mat2x2"):
			{
				setUniform(name, value.get<glm::mat2x2>());
			}
			break;
			case_str("mat3x3"):
			{
				setUniform(name, value.get<glm::mat3x3>());
			}
			break;
			case_str("mat4x4"):
			{
				setUniform(name, value.get<glm::mat4x4>());
			}
			break;
		}
	}

	#undef case_str
}

void ShaderProgram::loadConfig(const char* path)
{
	nlohmann::json config;

	std::ifstream{ path } >> config;

	loadConfig(config);
}

bool ShaderProgram::isValid() const
{
	return _program != 0;
}

GLuint ShaderProgram::getNativeHandle() const
{
	return _program;
}

#ifdef EDITOR

void ShaderProgram::_setAllUniforms()
{
	for (const auto& [ k, v ] : uniforms)
	{
		std::visit([this, &k](const auto& value) {
			setUniform(k.data(), value);
		}, v);
	}
}

///
class ImGuiID
{
public:

	///
	ImGuiID()
		: m_id { 0u }
		, m_buffer {}
	{

	}

	///
	ImGuiID(const ImGuiID&) = delete;

	///
	ImGuiID& operator = (const ImGuiID&) = delete;

	///
	ImGuiID(ImGuiID&& rhs)  = delete;

	///
	ImGuiID& operator = (ImGuiID&& rhs)  = delete;

	///
	~ImGuiID() = default;

	///
	void reset()
	{
		m_id = 0u;
	}

	///
	const char* get()
	{
		m_buffer = "##" + std::to_string(++m_id);

		return m_buffer.data();
	}

private:

	size_t m_id;
	std::string m_buffer;

};

void ShaderProgram::_showEditor(bool* open)
{
	if (!isValid()) return;

	static ImGuiID imID;

	if (ImGui::Begin(("ShaderProgram editor " + std::to_string(_program)).data(), open))
	{
		static char path[128] = { "test.json" };
		ImGui::InputText("##slPath", path, sizeof(path));

		ImGui::SameLine();
		if (ImGui::Button("Save"))
		{
			_saveConfig(path);
		}

		ImGui::SameLine();
		if (ImGui::Button("Load"))
		{
			_loadConfig(path);
		}

		ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();

		if (ImGui::BeginChild("##uniformsSelectPane", ImVec2(160, 0), true))
		{
			static int selected = -1;
			int i = 0;

			for (auto it = uniforms.begin(); it != uniforms.end(); ++it)
			{
				if (ImGui::Selectable(it->first.data(), selected == i))
				{
					currentElem = it;
					selected = i;
				}

				++i;
			}
		}
		ImGui::EndChild();

		ImGui::SameLine();

		if (ImGui::BeginChild("##uniformsEditPane", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())))
		{
			if (currentElem != uniforms.end())
			{
				std::visit(Overloaded {
					[this](bool& value) {
						ImGui::Text("Checkbox");
						ImGui::Checkbox(imID.get(), &value);
						setUniform(currentElem->first.data(), value);
					},
					[this](glm::bvec2& value) {
						ImGui::Text("Checkboxes");
						ImGui::Checkbox(imID.get(), &value[0]); ImGui::SameLine();
						ImGui::Checkbox(imID.get(), &value[1]);
						setUniform(currentElem->first.data(), value);
					},
					[this](glm::bvec3& value) {
						ImGui::Text("Checkboxes");
						ImGui::Checkbox(imID.get(), &value[0]); ImGui::SameLine();
						ImGui::Checkbox(imID.get(), &value[1]); ImGui::SameLine();
						ImGui::Checkbox(imID.get(), &value[2]);
						setUniform(currentElem->first.data(), value);
					},
					[this](glm::bvec4& value) {
						ImGui::Text("Checkboxes");
						ImGui::Checkbox(imID.get(), &value[0]); ImGui::SameLine();
						ImGui::Checkbox(imID.get(), &value[1]); ImGui::SameLine();
						ImGui::Checkbox(imID.get(), &value[2]); ImGui::SameLine();
						ImGui::Checkbox(imID.get(), &value[3]);
						setUniform(currentElem->first.data(), value);
					},
					[this](int& value) {
						ImGui::Text("Input"); ImGui::Spacing();
						ImGui::InputInt(imID.get(), &value);
						setUniform(currentElem->first.data(), value);
					},
					[this](glm::ivec2& value) {
						ImGui::Text("Input"); ImGui::Spacing();
						ImGui::InputInt2(imID.get(), glm::value_ptr(value));
						setUniform(currentElem->first.data(), value);
					},
					[this](glm::ivec3& value) {
						ImGui::Text("Input"); ImGui::Spacing();
						ImGui::InputInt3(imID.get(), glm::value_ptr(value));
						setUniform(currentElem->first.data(), value);
					},
					[this](glm::ivec4& value) {
						ImGui::Text("Input"); ImGui::Spacing();
						ImGui::InputInt4(imID.get(), glm::value_ptr(value));
						setUniform(currentElem->first.data(), value);
					},
					[this](unsigned& value) {
						ImGui::Text("Input"); ImGui::Spacing();
						ImGui::InputScalar(imID.get(), ImGuiDataType_U32, &value);
						setUniform(currentElem->first.data(), value);
					},
					[this](glm::uvec2& value) {
						ImGui::Text("Input"); ImGui::Spacing();
						ImGui::InputScalarN(imID.get(), ImGuiDataType_U32, glm::value_ptr(value), 2);
						setUniform(currentElem->first.data(), value);
					},
					[this](glm::uvec3& value) {
						ImGui::Text("Input"); ImGui::Spacing();
						ImGui::InputScalarN(imID.get(), ImGuiDataType_U32, glm::value_ptr(value), 3);
						setUniform(currentElem->first.data(), value);
					},
					[this](glm::uvec4& value) {
						ImGui::Text("Input"); ImGui::Spacing();
						ImGui::InputScalarN(imID.get(), ImGuiDataType_U32, glm::value_ptr(value), 4);
						setUniform(currentElem->first.data(), value);
					},
					[this](float& value) {
						ImGui::Text("DragInput"); ImGui::Spacing();
						ImGui::DragFloat(imID.get(), &value, 0.01f);
						setUniform(currentElem->first.data(), value);
					},
					[this](glm::vec2& value) {
						ImGui::Text("DragInput"); ImGui::Spacing();
						ImGui::DragFloat2(imID.get(), glm::value_ptr(value), 0.01f);
						setUniform(currentElem->first.data(), value);
					},
					[this](glm::vec3& value) {
						ImGui::Text("DragInput"); ImGui::Spacing();
						ImGui::DragFloat3(imID.get(), glm::value_ptr(value), 0.01f);
						ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();
						ImGui::Text("ColorEdit"); ImGui::Spacing();
						ImGui::ColorEdit3(imID.get(), glm::value_ptr(value));
						setUniform(currentElem->first.data(), value);
					},
					[this](glm::vec4& value) {
						ImGui::Text("DragInput"); ImGui::Spacing();
						ImGui::DragFloat4(imID.get(), glm::value_ptr(value), 0.01f);
						ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();
						ImGui::Text("ColorEdit"); ImGui::Spacing();
						ImGui::ColorEdit4(imID.get(), glm::value_ptr(value));
						setUniform(currentElem->first.data(), value);
					},
					[this](glm::mat2x2& value) {
						ImGui::Text("DragInput"); ImGui::Spacing();
						ImGui::DragFloat2(imID.get(), glm::value_ptr(value[0]), 0.01f);
						ImGui::DragFloat2(imID.get(), glm::value_ptr(value[1]), 0.01f);
						setUniform(currentElem->first.data(), value);
					},
					[this](glm::mat3x3& value) {
						ImGui::Text("DragInput"); ImGui::Spacing();
						ImGui::DragFloat3(imID.get(), glm::value_ptr(value[0]), 0.01f);
						ImGui::DragFloat3(imID.get(), glm::value_ptr(value[1]), 0.01f);
						ImGui::DragFloat3(imID.get(), glm::value_ptr(value[2]), 0.01f);
						setUniform(currentElem->first.data(), value);
					},
					[this](glm::mat4x4& value) {
						ImGui::Text("DragInput"); ImGui::Spacing();
						ImGui::DragFloat4(imID.get(), glm::value_ptr(value[0]), 0.01f);
						ImGui::DragFloat4(imID.get(), glm::value_ptr(value[1]), 0.01f);
						ImGui::DragFloat4(imID.get(), glm::value_ptr(value[2]), 0.01f);
						ImGui::DragFloat4(imID.get(), glm::value_ptr(value[3]), 0.01f);
						setUniform(currentElem->first.data(), value);
					},
				}, currentElem->second);

				imID.reset();
			}
		}
		ImGui::EndChild();
	}
	ImGui::End();
}

void ShaderProgram::_saveConfig(nlohmann::json& config) const
{
	for (const auto& [ k, v ] : uniforms)
	{
		nlohmann::json& j = config[k];

		std::visit([&j, type = uniTypeNames[v.index()]](const auto& value) {
			j["value"] = value;
			j["type"] = type;
		}, v);
	}
}

void ShaderProgram::_saveConfig(const char* path) const
{
	nlohmann::json config;

	_saveConfig(config);

	std::ofstream{ path } << std::setw(4) << config;
}

void ShaderProgram::_loadConfig(const nlohmann::json& config)
{
	for (auto& [ k, v ] : uniforms)
	{
		if (auto it = config.find(k); it != config.end())
		{
			std::visit([it](auto& value) {
				value = it.value()["value"];
			}, v);
		}
	}

	_setAllUniforms();
}

void ShaderProgram::_loadConfig(const char* path)
{
	nlohmann::json config;

	std::ifstream{ path } >> config;

	_loadConfig(config);
}

#endif // EDITOR

void ShaderProgram::_destroy()
{
	if (isValid())
	{
		glDeleteProgram(_program);

		_program = 0;
	}
}

bool ShaderProgram::_finishLinking()
{
	GLint success;
	glGetProgramiv(_program, GL_LINK_STATUS, &success);

	if (success != GL_TRUE)
	{
		GLchar infoLog[512];
		glGetProgramInfoLog(_program, sizeof(infoLog), nullptr, infoLog);

		LOG_ERROR("Unable to link shader program:\n", infoLog);

		_destroy();

		return false;
	}

	#ifdef EDITOR
	{
		GLint count;
		GLint size;
		GLenum type;
		GLchar name[64];

		glGetProgramiv(_program, GL_ACTIVE_UNIFORMS, &count);

		for (GLint i = 0; i < count; ++i)
		{
			glGetActiveUniform(_program, i, sizeof(name), nullptr, &size, &type, name);

			switch (type)
			{
				case GL_BOOL:
				{
					int value;
					glGetUniformiv(_program, i, &value);
					uniforms.emplace(name, static_cast<bool>(value));
				}
				break;
				case GL_BOOL_VEC2:
				{
					glm::ivec2 value;
					glGetUniformiv(_program, i, glm::value_ptr(value));
					uniforms.emplace(name, glm::bvec2{ value });
				}
				break;
				case GL_BOOL_VEC3:
				{
					glm::ivec3 value;
					glGetUniformiv(_program, i, glm::value_ptr(value));
					uniforms.emplace(name, glm::bvec3{ value });
				}
				break;
				case GL_BOOL_VEC4:
				{
					glm::ivec4 value;
					glGetUniformiv(_program, i, glm::value_ptr(value));
					uniforms.emplace(name, glm::bvec4{ value });
				}
				break;
				case GL_INT:
				{
					int value;
					glGetUniformiv(_program, i, &value);
					uniforms.emplace(name, value);
				}
				break;
				case GL_INT_VEC2:
				{
					glm::ivec2 value;
					glGetUniformiv(_program, i, glm::value_ptr(value));
					uniforms.emplace(name, value);
				}
				break;
				case GL_INT_VEC3:
				{
					glm::ivec3 value;
					glGetUniformiv(_program, i, glm::value_ptr(value));
					uniforms.emplace(name, value);
				}
				break;
				case GL_INT_VEC4:
				{
					glm::ivec4 value;
					glGetUniformiv(_program, i, glm::value_ptr(value));
					uniforms.emplace(name, value);
				}
				break;
				case GL_UNSIGNED_INT:
				{
					unsigned value;
					glGetUniformuiv(_program, i, &value);
					uniforms.emplace(name, value);
				}
				break;
				case GL_UNSIGNED_INT_VEC2:
				{
					glm::uvec2 value;
					glGetUniformuiv(_program, i, glm::value_ptr(value));
					uniforms.emplace(name, value);
				}
				break;
				case GL_UNSIGNED_INT_VEC3:
				{
					glm::uvec3 value;
					glGetUniformuiv(_program, i, glm::value_ptr(value));
					uniforms.emplace(name, value);
				}
				break;
				case GL_UNSIGNED_INT_VEC4:
				{
					glm::uvec4 value;
					glGetUniformuiv(_program, i, glm::value_ptr(value));
					uniforms.emplace(name, value);
				}
				break;
				case GL_FLOAT:
				{
					float value;
					glGetUniformfv(_program, i, &value);
					uniforms.emplace(name, value);
				}
				break;
				case GL_FLOAT_VEC2:
				{
					glm::vec2 value;
					glGetUniformfv(_program, i, glm::value_ptr(value));
					uniforms.emplace(name, value);
				}
				break;
				case GL_FLOAT_VEC3:
				{
					glm::vec3 value;
					glGetUniformfv(_program, i, glm::value_ptr(value));
					uniforms.emplace(name, value);
				}
				break;
				case GL_FLOAT_VEC4:
				{
					glm::vec4 value;
					glGetUniformfv(_program, i, glm::value_ptr(value));
					uniforms.emplace(name, value);
				}
				break;
				case GL_FLOAT_MAT2:
				{
					glm::mat2x2 value;
					glGetUniformfv(_program, i, glm::value_ptr(value));
					uniforms.emplace(name, value);
				}
				break;
				case GL_FLOAT_MAT3:
				{
					glm::mat3x3 value;
					glGetUniformfv(_program, i, glm::value_ptr(value));
					uniforms.emplace(name, value);
				}
				break;
				case GL_FLOAT_MAT4:
				{
					glm::mat4x4 value;
					glGetUniformfv(_program, i, glm::value_ptr(value));
					uniforms.emplace(name, value);
				}
				break;
			}
		}
	}
	#endif // EDITOR

	return true;
}

}
