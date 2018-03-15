#include "Shader.hpp"

namespace rat
{

Shader::Shader()
{
	_manager.loadFromConfig("Assets/Shader/shader.json");

	#ifdef EDITOR
	{
		gVar->create<sf::Texture*>("templates", new sf::Texture);
		sf::Image img; img.loadFromFile("Assets/Texture/templates.png"); img.flipVertically();
		auto p = gVar->get<sf::Texture*>("templates");
		p->loadFromImage(img);
		previewRTex.create(p->getSize().x, p->getSize().y);
	}
	#endif

	LOG_INFO(this, " : Module Shader constructed");
}

Shader::~Shader()
{
	LOG_INFO(this, " : Module Shader destructed");
}

void Shader::update()
{
	#ifdef EDITOR
	{
		if (isEditorOpen) {
			auto& info = _manager._shaderInfo;
			static size_t shaderIndex = 0;
			static int shaderType = 0;
			static bool showPreview = true;
			if (ImGui::Begin("Shader composer", &isEditorOpen)) {
				static char contentBuffer[ShaderInfo::shadersCount][1024 * 8] = {};
				static std::vector<std::tuple<Hash32_t, std::string, std::any>> uniforms[ShaderInfo::shadersCount];
				static std::regex mainRegex{ R"(uniform\s+(\w+)\s+(\w+)\s*\=\s*(.+)\s*\;)" };
				static const char* shaderName = nullptr;

				static auto onLoad = [&] {
					if (shaderName == nullptr) return;
					info[shaderIndex].reload();
					for (size_t i = 0; i < ShaderInfo::shadersCount; ++i) {
						if (!info[shaderIndex].hasType(static_cast<ShaderInfo::ShaderType_e>(i))) continue;
						shaderType = i;
						uniforms[i].clear();
						std::fill(contentBuffer[i], contentBuffer[i] + sizeof(contentBuffer[i]), '\0');
						std::copy(info[shaderIndex].content[i].begin(), info[shaderIndex].content[i].end(), contentBuffer[i]);
						for (auto it = std::sregex_iterator(info[shaderIndex].content[i].begin(), info[shaderIndex].content[i].end(), mainRegex); it != std::sregex_iterator(); ++it) {
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
					if (shaderName == nullptr) return;
					for (size_t i = 0; i < ShaderInfo::shadersCount; ++i) {
						if (info[shaderIndex].hasType(static_cast<ShaderInfo::ShaderType_e>(i))) {
							std::ofstream out{ info[shaderIndex].filePath[i] };
							out.write(contentBuffer[i], std::strlen(contentBuffer[i]));
						}
					}
				};

				if (ImGui::Button("Load")) onLoad();
				ImGui::SameLine();
				if (ImGui::Button("Save")) onSave();
				ImGui::SameLine();
				if (ImGui::BeginCombo("##3.14159", shaderName)) {
					for (size_t v = 0; v < info.size(); ++v) {
						if (ImGui::Selectable(info[v].name.data(), shaderIndex == v)) {
							shaderName = info[v].name.data();
							shaderIndex = v;
							onLoad();
						}
					}
					ImGui::EndCombo();
				}
				ImGui::SameLine();
				ImGui::Checkbox("Show preview", &showPreview);
				ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();
				if (shaderName != nullptr) {
					if (info[shaderIndex].hasType(ShaderInfo::Vertex)) {
						ImGui::RadioButton("vert", &shaderType, ShaderInfo::Vertex);
						ImGui::SameLine();
					}
					if (info[shaderIndex].hasType(ShaderInfo::Geometry)) {
						ImGui::RadioButton("geom", &shaderType, ShaderInfo::Geometry);
						ImGui::SameLine();
					}
					if (info[shaderIndex].hasType(ShaderInfo::Fragment)) {
						ImGui::RadioButton("frag", &shaderType, ShaderInfo::Fragment);
						ImGui::SameLine();
					}
				}
				ImGui::Text(" ");
				ImGui::Columns(2);
				ImGui::InputTextMultiline("", contentBuffer[shaderType], sizeof(contentBuffer[shaderType]), { -1.0f, -1.0f });
				ImGui::NextColumn();
				ImGui::Text("Uniforms:");
				for (auto& [ t, n, v ] : uniforms[shaderType]) {
					switch (t) {
						case fnv1a_32("bool"): {
							if (ImGui::Checkbox(n.data(), std::any_cast<bool>(&v))) {
								info[shaderIndex].ptr->setUniform(n, std::any_cast<bool>(v));
							}
						}
						break;
						case fnv1a_32("int"): {
							if (ImGui::InputInt(n.data(), std::any_cast<int>(&v))) {
								info[shaderIndex].ptr->setUniform(n, std::any_cast<int>(v));
							}
						}
						break;
						case fnv1a_32("uint"): {
							if (ImGui::InputInt(n.data(), std::any_cast<int>(&v))) {
								info[shaderIndex].ptr->setUniform(n, std::any_cast<int>(v));
							}
						}
						break;
						case fnv1a_32("float"): {
							if (ImGui::InputFloat(n.data(), std::any_cast<float>(&v))) {
								info[shaderIndex].ptr->setUniform(n, std::any_cast<float>(v));
							}
						}
						break;
						case fnv1a_32("vec2"): {
							if (ImGui::InputFloat2(n.data(), &std::any_cast<sf::Glsl::Vec2>(&v)->x, 3)) {
								info[shaderIndex].ptr->setUniform(n, std::any_cast<sf::Glsl::Vec2>(v));
							}
						}
						break;
						case fnv1a_32("vec3"): {
							if (ImGui::InputFloat3(n.data(), &std::any_cast<sf::Glsl::Vec3>(&v)->x, 3)) {
								info[shaderIndex].ptr->setUniform(n, std::any_cast<sf::Glsl::Vec3>(v));
							}
							if (ImGui::ColorEdit3("", &std::any_cast<sf::Glsl::Vec3>(&v)->x)) {
								info[shaderIndex].ptr->setUniform(n, std::any_cast<sf::Glsl::Vec3>(v));
							}
						}
						break;
					}

				}
				ImGui::Columns();
			}
			ImGui::End();

			if (showPreview) {
				if (ImGui::Begin("Shader preview", &showPreview, ImGuiWindowFlags_HorizontalScrollbar)) {
					static bool showShader = true;
					static float previewScale = 1.0f;
					ImGui::Checkbox("Show shader", &showShader);
					ImGui::SliderFloat("Scale", &previewScale, 0.1f, 5.0f);
					sf::Sprite spr{ *gVar->get<sf::Texture*>("templates") };
					previewRTex.clear();
					previewRTex.draw(spr, showShader ? info[shaderIndex].ptr : nullptr);
					previewRTex.display();
					sf::Vector2f scale{ previewRTex.getTexture().getSize().x * previewScale, previewRTex.getTexture().getSize().y * previewScale };
					ImGui::Image(previewRTex.getTexture(), scale, sf::Color::White, sf::Color::Magenta);
				}
				ImGui::End();
			}
		}
	}
	#endif
}

ShaderManager& Shader::getManager()
{
	return _manager;
}

const ShaderManager& Shader::getManager() const
{
	return _manager;
}


}
