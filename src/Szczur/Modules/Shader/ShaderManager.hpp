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

	#ifdef EDITOR
	struct ShaderInfo
	{
		static inline constexpr size_t shaderTypesCount = 3;

		enum ShaderType_e { Vertex = 0, Geometry = 1, Fragment = 2 };
		enum ShaderTypeBits_e { Vert = 1, Geom = 2, Frag = 4, VertFrag = Vert | Frag, VertGeomFrag = Vert | Geom | Frag };

		bool hasType(ShaderType_e code) const { return typeBits & code; }
		bool matchType(ShaderTypeBits_e code) const { return typeBits == code; }

		sf::Shader* ptr;
		const std::string name;
		const std::string filePath[shaderTypesCount];
		std::string content[shaderTypesCount];
		const bool has[shaderTypesCount];

		int typeBits;

	};

	std::vector<ShaderInfo> _shaderInfo;

	void _reload(const std::string& name)
	{
		auto& info = getShaderInfo(name);

		if (info.has[0] && info.has[1]) {
			info.ptr->loadFromFile(info.filePath[0], info.filePath[1]);
			std::ifstream vertIn{ info.filePath[0] };
			info.content[0] = std::string{ std::istreambuf_iterator<char>{ vertIn }, std::istreambuf_iterator<char>{} };
			std::ifstream fragIn{ info.filePath[1] };
			info.content[1] = std::string{ std::istreambuf_iterator<char>{ fragIn }, std::istreambuf_iterator<char>{} };
		}
		else if (info.has[0]) {
			info.ptr->loadFromFile(info.filePath[0], sf::Shader::Vertex);
			std::ifstream vertIn{ info.filePath[0] };
			info.content[0] = std::string{ std::istreambuf_iterator<char>{ vertIn }, std::istreambuf_iterator<char>{} };
		}
		else if (info.has[1]) {
			info.ptr->loadFromFile(info.filePath[1], sf::Shader::Fragment);
			std::ifstream fragIn{ info.filePath[1] };
			info.content[1] = std::string{ std::istreambuf_iterator<char>{ fragIn }, std::istreambuf_iterator<char>{} };
		}
	}

	Pointer_t getPtr(const std::string& name);
	ConstPointer_t getPtr(const std::string& name) const;

	Reference_t getRef(const std::string& name);
	ConstReference_t getRef(const std::string& name) const;

	ShaderInfo& getShaderInfo(const std::string& name);
	const ShaderInfo& getShaderInfo(const std::string& name) const;

	ShaderInfo& getShaderInfo(const sf::Shader* ptr);
	const ShaderInfo& getShaderInfo(const sf::Shader* ptr) const;

	ShaderInfo& getShaderInfo(size_t index);
	const ShaderInfo& getShaderInfo(size_t index) const;
	#endif

private:

	void _loadVertFrag(const std::string& name, const std::string& vertexFilePath, const std::string& fragFilePath);

	void _loadVert(const std::string& name, const std::string& vertexFilePath);

	void _loadFrag(const std::string& name, const std::string& fragFilePath);

	Holder_t _holder;

};

}
