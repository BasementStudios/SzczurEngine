#include "SFX.hpp"

namespace rat
{

SFX::SFX()
{
	_manager.loadFromConfig("Assets/Shader/shader.json");

	LOG_INFO(this, " : Module SFX constructed");
}

SFX::~SFX()
{
	LOG_INFO(this, " : Module SFX destructed");
}

void SFX::update()
{
	#ifdef EDITOR
	{
		if (gVar->get<bool>("isShaderWindow")) {
			if (ImGui::Begin("Shader composer", &gVar->get<bool>("isShaderWindow"))) {
				auto& info = _manager._shaderInfo;
				static int index = 0;
				static int currentShaderType = 0;
				static char contentBuffer[2][1024 * 8] = {};
				static std::vector<std::tuple<Hash32_t, std::string, std::any>> uniforms[2];
				static std::regex mainRegex{ R"(uniform\s+(\w+)\s+(\w+)\s*\=\s*(.+)\s*\;)" };

				static auto onLoad = [&] {
					for(int i = 0; i < 2; ++i) {
						if (!info[index].has[i]) continue;
						currentShaderType = i;
						uniforms[i].clear();
						std::fill(contentBuffer[i], contentBuffer[i] + sizeof(contentBuffer[i]), '\0');
						std::copy(info[index].content[i].begin(), info[index].content[i].end(), contentBuffer[i]);
						for (auto it = std::sregex_iterator(info[index].content[i].begin(), info[index].content[i].end(), mainRegex); it != std::sregex_iterator(); ++it) {
							auto type = fnv1a_32(it->str(1).data());
							auto name = it->str(2);
							auto value = [&]() -> std::any {
								switch (type) {
									case fnv1a_32("bool"): {
										return it->str(3) == "true" ? true : false;
									}
									case fnv1a_32("int"): {
										return boost::lexical_cast<int>(it->str(3));
									}
									case fnv1a_32("uint"): {
										return boost::lexical_cast<int>(it->str(3));
									}
									case fnv1a_32("float"): {
										return boost::lexical_cast<float>(it->str(3));
									}
									case fnv1a_32("double"): {
										return boost::lexical_cast<double>(it->str(3));
									}
									case fnv1a_32("vec2"): {
										std::cmatch cm;
										std::regex_match(it->str(3).data(), cm, std::regex{ R"(vec2\s*\(\s*(\S+)\s*,\s*(\S+)\s*\))" });
										return sf::Glsl::Vec2{ boost::lexical_cast<float>(cm.str(1)), boost::lexical_cast<float>(cm.str(2)) };
									}
									case fnv1a_32("vec3"): {
										std::cmatch cm;
										std::regex_match(it->str(3).data(), cm, std::regex{ R"(vec3\s*\(\s*(\S+)\s*,\s*(\S+)\s*,\s*(\S+)\s*\))" });
										return sf::Glsl::Vec3{ boost::lexical_cast<float>(cm.str(1)), boost::lexical_cast<float>(cm.str(2)), boost::lexical_cast<float>(cm.str(3)) };
									}
								}
								return {};
							}();
							uniforms[i].emplace_back(type, name, value);
						}
					}
				};
				static auto onSave = [&] {
					// std::ofstream out{ filenameBuffer };
					// out.write(contentBuffer, std::strlen(contentBuffer));
				};

				static const char* shaderName = nullptr;
				if (ImGui::BeginCombo("Avaible shaders", shaderName)) {
					for (size_t v = 0; v < info.size(); ++v) {
						if (ImGui::Selectable(info[v].name.data(), index == static_cast<int>(v))) {
							shaderName = info[v].name.data();
							index = v;
							onLoad();
						}
					}
			        ImGui::EndCombo();
			    }

				if (ImGui::Button("Load")) onLoad();
				ImGui::SameLine();
				if (ImGui::Button("Save")) onSave();
				ImGui::SameLine();
				if (ImGui::Button("Save and reload")) {
					onSave(); onLoad();
				}
				ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();
				if (info[index].has[0] && info[index].has[1]) {
					ImGui::RadioButton("vertex", &currentShaderType, 0);
					ImGui::SameLine();
					ImGui::RadioButton("fragment", &currentShaderType, 1);
				}
				else if (info[index].has[0]) {
					ImGui::RadioButton("vertex", &currentShaderType, 0);
				}
				else if (info[index].has[1]) {
					ImGui::RadioButton("fragment", &currentShaderType, 1);
				}
				ImGui::Columns(2);
				ImGui::InputTextMultiline("", contentBuffer[currentShaderType], sizeof(contentBuffer[currentShaderType]), { -1.0f, -1.0f });
				ImGui::NextColumn();
				ImGui::Text("Uniforms:");
				for (auto& [ t, n, v ] : uniforms[currentShaderType]) {
					switch (t) {
						case fnv1a_32("bool"): {
							if (ImGui::Checkbox(n.data(), std::any_cast<bool>(&v))) {
								info[index].ptr->setUniform(n, std::any_cast<bool>(v));
							}
						}
						break;
						case fnv1a_32("int"): {
							if (ImGui::InputInt(n.data(), std::any_cast<int>(&v))) {
								info[index].ptr->setUniform(n, std::any_cast<int>(v));
							}
						}
						break;
						case fnv1a_32("uint"): {
							if (ImGui::InputInt(n.data(), std::any_cast<int>(&v))) {
								info[index].ptr->setUniform(n, std::any_cast<int>(v));
							}
						}
						break;
						case fnv1a_32("float"): {
							if (ImGui::InputFloat(n.data(), std::any_cast<float>(&v))) {
								info[index].ptr->setUniform(n, std::any_cast<float>(v));
							}
						}
						break;
						case fnv1a_32("vec2"): {
							if (ImGui::InputFloat2(n.data(), &std::any_cast<sf::Glsl::Vec2>(&v)->x, 3)) {
								info[index].ptr->setUniform(n, std::any_cast<sf::Glsl::Vec2>(v));
							}
						}
						break;
						case fnv1a_32("vec3"): {
							if (ImGui::InputFloat3(n.data(), &std::any_cast<sf::Glsl::Vec3>(&v)->x, 3)) {
								info[index].ptr->setUniform(n, std::any_cast<sf::Glsl::Vec3>(v));
							}
							if (ImGui::ColorEdit3("", &std::any_cast<sf::Glsl::Vec3>(&v)->x)) {
								info[index].ptr->setUniform(n, std::any_cast<sf::Glsl::Vec3>(v));
							}
						}
						break;
					}

				}
				ImGui::Columns();
			}
			ImGui::End();
		}
	}
	#endif
}

ShaderManager& SFX::getManager()
{
	return _manager;
}

const ShaderManager& SFX::getManager() const
{
	return _manager;
}


}
