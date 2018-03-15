#include "ShaderManager.hpp"

#include <fstream>

namespace rat
{

bool ShaderManager::loadFromConfig(const std::string& filePath)
{
	Json config;
	std::ifstream{ filePath } >> config;

	for (const auto& j : config) {
		bool hasVertex = j.find("vert") != j.end();
		bool hasFragment = j.find("frag") != j.end();

		if(hasVertex && hasFragment) {
			_loadVertFrag(j["name"], j["vert"].get<std::string>(), j["frag"].get<std::string>());
		}
		else if(hasVertex) {
			_loadVert(j["name"], j["vert"].get<std::string>());
		}
		else if(hasFragment) {
			_loadFrag(j["name"], j["frag"].get<std::string>());
		}
	}

	return false;
}

ShaderManager::Pointer_t ShaderManager::getPtr(const Key_t& key)
{
	if (auto it = _holder.find(key); it != _holder.end()) {
		return it->second.get();
	}

	return nullptr;
}

ShaderManager::ConstPointer_t ShaderManager::getPtr(const Key_t& key) const
{
	if (auto it = _holder.find(key); it != _holder.end()) {
		return it->second.get();
	}

	return nullptr;
}

ShaderManager::Reference_t ShaderManager::getRef(const Key_t& key)
{
	return *_holder.at(key);
}

ShaderManager::ConstReference_t ShaderManager::getRef(const Key_t& key) const
{
	return *_holder.at(key);
}

void ShaderManager::_loadVertFrag(const std::string& name, const std::string& vertexFilePath, const std::string& fragFilePath)
{
	auto ptr = std::make_unique<sf::Shader>();

	if (ptr->loadFromFile(vertexFilePath, fragFilePath)) {
		#ifdef EDITOR
		{
			std::ifstream vertIn{ vertexFilePath };
			std::ifstream fragIn{ fragFilePath };
			_shaderInfo.emplace_back(ShaderDebugInfo{
				ptr.get(),
				name,
				{ vertexFilePath, fragFilePath },
				{ std::string{ std::istreambuf_iterator<char>{ vertIn }, std::istreambuf_iterator<char>{} }, std::string{ std::istreambuf_iterator<char>{ fragIn }, std::istreambuf_iterator<char>{} } },
				{ true, true }
			});
		}
		#endif
		_holder.emplace(fnv1a_32(name.data()), std::move(ptr));
	}
}

void ShaderManager::_loadVert(const std::string& name, const std::string& vertexFilePath)
{
	auto ptr = std::make_unique<sf::Shader>();

	if (ptr->loadFromFile(vertexFilePath, sf::Shader::Vertex)) {
		#ifdef EDITOR
		{
			std::ifstream vertIn{ vertexFilePath };
			_shaderInfo.emplace_back(ShaderDebugInfo{
				ptr.get(),
				name,
				{ vertexFilePath, {} },
				{ std::string{ std::istreambuf_iterator<char>{ vertIn }, std::istreambuf_iterator<char>{} }, {} },
				{ true, false }
			});
		}
		#endif
		_holder.emplace(fnv1a_32(name.data()), std::move(ptr));
	}
}

void ShaderManager::_loadFrag(const std::string& name, const std::string& fragFilePath)
{
	auto ptr = std::make_unique<sf::Shader>();

	if (ptr->loadFromFile(fragFilePath, sf::Shader::Fragment)) {
		#ifdef EDITOR
		{
			std::ifstream fragIn{ fragFilePath };
			_shaderInfo.emplace_back(ShaderDebugInfo{
				ptr.get(),
				name,
				{ {}, fragFilePath },
				{ {}, std::string{ std::istreambuf_iterator<char>{ fragIn }, std::istreambuf_iterator<char>{} } },
				{ false, true }
			});
		}
		#endif
		_holder.emplace(fnv1a_32(name.data()), std::move(ptr));
	}
}

}
