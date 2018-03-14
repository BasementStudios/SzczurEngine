#pragma once

#include <memory>

#include <SFML/Graphics/Shader.hpp>

#include <boost/container/flat_map.hpp>

#include "Szczur/Utility/Convert/Hash.hpp"
#include "Szczur/Debug.hpp"
#include "Szczur/Json.hpp"

namespace rat
{

class ShaderManager
{
public:

	using Value_t          = sf::Shader;
	using Reference_t      = Value_t&;
	using ConstReference_t = const Value_t&;
	using Pointer_t        = Value_t*;
	using ConstPointer_t   = const Value_t*;
	using Key_t            = Hash32_t;
	using Held_t           = std::unique_ptr<Value_t>;
	using Holder_t         = boost::container::flat_map<Key_t, Held_t>;

	ShaderManager() = default;

	~ShaderManager() = default;

	ShaderManager(const ShaderManager&) = delete;

	ShaderManager& operator = (const ShaderManager&) = delete;

	ShaderManager(ShaderManager&&) = delete;

	ShaderManager& operator = (ShaderManager&&) = delete;

	bool loadFromConfig(const std::string& filePath);

	Pointer_t getPtr(const Key_t& key);
	ConstPointer_t getPtr(const Key_t& key) const;

	Reference_t getRef(const Key_t& key);
	ConstReference_t getRef(const Key_t& key) const;

private:

	void _loadVertFrag(const std::string& name, const std::string& vertexFilePath, const std::string& fragFilePath);

	void _loadVert(const std::string& name, const std::string& vertexFilePath);

	void _loadFrag(const std::string& name, const std::string& fragFilePath);

	Holder_t _holder;

	#ifdef EDITOR
	struct ShaderDebugInfo
	{
		sf::Shader* ptr;
		const std::string name;
		const std::string filePath[2];
		const std::string content[2];
		const bool has[2];
	};

	std::vector<ShaderDebugInfo> _shaderInfo;
	#endif

};

}
