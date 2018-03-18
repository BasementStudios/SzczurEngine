#pragma once

#include <memory>

#include <SFML/Graphics/Shader.hpp>

#include <boost/container/flat_map.hpp>

#include "Szczur/Utility/Convert/Hash.hpp"
#include "Szczur/Debug.hpp"
#include "Szczur/Json.hpp"

namespace rat
{

#ifdef EDITOR
struct ShaderInfo
{
	static inline constexpr size_t shadersCount = 3;

	enum ShaderType_e { Vertex = 0, Geometry = 1, Fragment = 2 };
	enum ShaderTypeBits_e { Vert = 1, Geom = 2, Frag = 4, VertFrag = Vert | Frag, VertGeomFrag = Vert | Geom | Frag };

	ShaderInfo(ShaderTypeBits_e typeBits, sf::Shader* ptr, const std::string& name, const std::string vertFilePath, const std::string geomFilePath, const std::string fragFilePath) :
		ptr{ ptr }, name{ name }, filePath{ vertFilePath, geomFilePath, fragFilePath }, _typeBits{ typeBits }
	{
		reload();
	}

	void loadShaders()
	{
		switch (getBits()) {
			case Vert: ptr->loadFromFile(filePath[Vertex], sf::Shader::Vertex); break;
			case Geom: ptr->loadFromFile(filePath[Geometry], sf::Shader::Geometry); break;
			case Frag: ptr->loadFromFile(filePath[Fragment], sf::Shader::Fragment); break;
			case VertFrag: ptr->loadFromFile(filePath[Vertex], filePath[Fragment]); break;
			case VertGeomFrag: ptr->loadFromFile(filePath[Vertex], filePath[Geometry], filePath[Fragment]); break;
		}
	}

	void loadContent()
	{
		std::fstream in;
		if (hasBits(Vert)) {
			in.open(filePath[Vertex]);
			content[Vertex] = std::string{ std::istreambuf_iterator<char>{ in }, std::istreambuf_iterator<char>{} };
			in.close();
		}
		if (hasBits(Geom)) {
			in.open(filePath[Geometry]);
			content[Geometry] = std::string{ std::istreambuf_iterator<char>{ in }, std::istreambuf_iterator<char>{} };
			in.close();
		}
		if (hasBits(Frag)) {
			in.open(filePath[Fragment]);
			content[Fragment] = std::string{ std::istreambuf_iterator<char>{ in }, std::istreambuf_iterator<char>{} };
			in.close();
		}
	}

	void reload()
	{
		loadShaders();
		loadContent();
	}

	bool hasType(ShaderType_e code) const
	{
		switch (code) {
			case Vertex: return hasBits(Vert);
			case Geometry: return hasBits(Geom);
			case Fragment: return hasBits(Frag);
		}
		return false;
	}

	unsigned getBits() const
	{
		return _typeBits;
	}

	bool hasBits(ShaderTypeBits_e code) const
	{
		return getBits() & code;
	}

	bool matchBits(ShaderTypeBits_e code) const
	{
		return getBits() == code;
	}

	sf::Shader* ptr;
	const std::string name;
	const std::string filePath[shadersCount];
	std::string content[shadersCount];

	unsigned _typeBits;

};
#endif

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
	std::vector<ShaderInfo> _shaderInfo;

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

	void _loadVert(const std::string& name, const std::string& vertFilePath);

	void _loadGeom(const std::string& name, const std::string& geomFilePath);

	void _loadFrag(const std::string& name, const std::string& fragFilePath);

	void _loadVertFrag(const std::string& name, const std::string& vertFilePath, const std::string& fragFilePath);

	void _loadVertGeomFrag(const std::string& name, const std::string& vertFilePath, const std::string& geomFilePath, const std::string& fragFilePath);

	Holder_t _holder;

};

}
