#include "ShaderManager.hpp"

#include <fstream>

namespace rat
{

bool ShaderManager::loadFromConfig(const std::string& filePath)
{
	Json config;
	std::ifstream{ filePath } >> config;

	for (const auto& j : config) {
		bool hasVert = j.find("vert") != j.end();
		bool hasGeom = j.find("geom") != j.end();
		bool hasFrag = j.find("frag") != j.end();

		if(hasVert && hasGeom && hasFrag) {
			_loadVertGeomFrag(j["name"], j["vert"], j["geom"], j["frag"]);
		}
		else if(hasVert && hasFrag) {
			_loadVertFrag(j["name"], j["vert"], j["frag"]);
		}
		else if(hasVert) {
			_loadVert(j["name"], j["vert"]);
		}
		else if(hasGeom) {
			_loadGeom(j["name"], j["geom"]);
		}
		else if(hasFrag) {
			_loadFrag(j["name"], j["frag"]);
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

#ifdef EDITOR
ShaderManager::Pointer_t ShaderManager::getPtr(const std::string& name)
{
	return getPtr(fnv1a_32(name.begin(), name.end()));
}

ShaderManager::ConstPointer_t ShaderManager::getPtr(const std::string& name) const
{
	return getPtr(fnv1a_32(name.begin(), name.end()));
}

ShaderManager::Reference_t ShaderManager::getRef(const std::string& name)
{
	return getRef(fnv1a_32(name.begin(), name.end()));
}

ShaderManager::ConstReference_t ShaderManager::getRef(const std::string& name) const
{
	return getRef(fnv1a_32(name.begin(), name.end()));
}

ShaderInfo& ShaderManager::getShaderInfo(const std::string& name)
{
	auto it = std::find_if(_shaderInfo.begin(), _shaderInfo.end(), [&n = name](const auto& info) {
		return info.name == n;
	});

	return *it;
}

const ShaderInfo& ShaderManager::getShaderInfo(const std::string& name) const
{
	auto it = std::find_if(_shaderInfo.begin(), _shaderInfo.end(), [&n = name](const auto& info) {
		return info.name == n;
	});

	return *it;
}

ShaderInfo& ShaderManager::getShaderInfo(const sf::Shader* ptr)
{
	auto it = std::find_if(_shaderInfo.begin(), _shaderInfo.end(), [p = ptr](const auto& info) {
		return info.ptr == p;
	});

	return *it;
}

const ShaderInfo& ShaderManager::getShaderInfo(const sf::Shader* ptr) const
{
	auto it = std::find_if(_shaderInfo.begin(), _shaderInfo.end(), [p = ptr](const auto& info) {
		return info.ptr == p;
	});

	return *it;
}

ShaderInfo& ShaderManager::getShaderInfo(size_t index)
{
	return _shaderInfo[index];
}

const ShaderInfo& ShaderManager::getShaderInfo(size_t index) const
{
	return _shaderInfo[index];
}
#endif

void ShaderManager::_loadVert(const std::string& name, const std::string& vertFilePath)
{
	auto ptr = std::make_unique<sf::Shader>();

	if (ptr->loadFromFile(vertFilePath, sf::Shader::Vertex)) {
		#ifdef EDITOR
		{
			_shaderInfo.emplace_back(ShaderInfo::Vert, ptr.get(), name, vertFilePath, std::string{}, std::string{});
		}
		#endif
		_holder.emplace(fnv1a_32(name.data()), std::move(ptr));
	}
}

void ShaderManager::_loadGeom(const std::string& name, const std::string& geomFilePath)
{
	auto ptr = std::make_unique<sf::Shader>();

	if (ptr->loadFromFile(geomFilePath, sf::Shader::Geometry)) {
		#ifdef EDITOR
		{
			_shaderInfo.emplace_back(ShaderInfo::Geom, ptr.get(), name, std::string{}, geomFilePath, std::string{});
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
			_shaderInfo.emplace_back(ShaderInfo::Frag, ptr.get(), name, std::string{}, std::string{}, fragFilePath);
		}
		#endif
		_holder.emplace(fnv1a_32(name.data()), std::move(ptr));
	}
}

void ShaderManager::_loadVertFrag(const std::string& name, const std::string& vertFilePath, const std::string& fragFilePath)
{
	auto ptr = std::make_unique<sf::Shader>();

	if (ptr->loadFromFile(vertFilePath, fragFilePath)) {
		#ifdef EDITOR
		{
			_shaderInfo.emplace_back(ShaderInfo::VertFrag, ptr.get(), name, vertFilePath, std::string{}, fragFilePath);
		}
		#endif
		_holder.emplace(fnv1a_32(name.data()), std::move(ptr));
	}
}

void ShaderManager::_loadVertGeomFrag(const std::string& name, const std::string& vertFilePath, const std::string& geomFilePath, const std::string& fragFilePath)
{
	auto ptr = std::make_unique<sf::Shader>();

	if (ptr->loadFromFile(vertFilePath, geomFilePath, fragFilePath)) {
		#ifdef EDITOR
		{
			_shaderInfo.emplace_back(ShaderInfo::VertGeomFrag, ptr.get(), name, vertFilePath, geomFilePath, fragFilePath);
		}
		#endif
		_holder.emplace(fnv1a_32(name.data()), std::move(ptr));
	}
}

}
