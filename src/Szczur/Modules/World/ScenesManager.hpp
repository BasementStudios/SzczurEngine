#pragma once

#include <memory>
#include <vector>

#include <SFML/Window/Event.hpp>

#include "Szczur/Utility/SFML3D/RenderLayer.hpp"
#include "Szczur/Utility/SFML3D/RenderTarget.hpp"
#include "Szczur/Utility/SFML3D/ShaderProgram.hpp"

namespace rat {
	class Scene;
	class Entity;
}
#include "Szczur/Modules/Window/Window.hpp"
#include "Data/ArmatureDisplayData.hpp"
#include "Data/TextureDataHolder.hpp"

#include <nlohmann/json.hpp>

namespace rat
{

class ScenesManager
{
public:

	using ScenesHolder_t = std::vector<std::unique_ptr<Scene>>;
	
	using ArmatureDisplayDataHolder_t = std::vector<std::unique_ptr<ArmatureDisplayData>>;

	///
	ScenesManager();

	/// Non-copyable
	ScenesManager(const ScenesManager&) = delete;
	ScenesManager& operator = (const ScenesManager&) = delete;

	/// Non-movable
	ScenesManager(ScenesManager&&) = delete;
	ScenesManager& operator = (ScenesManager&&) = delete;

	///
	void init(rat::Window& windowModule);

	///
	void render(sf3d::RenderTarget& target);

	///
	void processEvent(sf::Event event);

	///
	Scene* addScene();

	///
	bool removeScene(size_t id);

	///
	void removeAllScenes();

	///
	Scene* getScene(size_t id) const;

	///
	Scene* getScene(const std::string& name);

	///
	ScenesHolder_t& getScenes();

	///
	const ScenesHolder_t& getScenes() const;

	///
	bool hasScene(size_t id) const;

	///
	bool setCurrentScene(size_t id);

	///
	Scene* getCurrentScene() const;

	///
	size_t getCurrentSceneID() const;

	///
	bool isCurrentSceneValid() const;

	///
	ArmatureDisplayDataHolder_t& getArmatureDisplayDataHolder();

	///
	const ArmatureDisplayDataHolder_t& getArmatureDisplayDataHolder() const;

	///
	void loadFromConfig(nlohmann::json& config);

	///
	void saveToConfig(nlohmann::json& config);

	///
  	void saveEntityToConfig(Entity* entity, nlohmann::json& config);

	///
	void loadFromFile(const std::string& filepath);

	///
	void saveToFile(const std::string& filepath);

	///
	void loadScenesFromFile(const std::string& filepath);

	///
	void addPlayer();

	///
	void addCamera();

	///
	void addSun();

	/// Run game and save current config
	void runGame();

	/// Stop game and load saved config
	void stopGame();

	///
	bool isGameRunning();

	///
	nlohmann::json& getRunConfig();

	///
	TextureDataHolder& getTextureDataHolder();

	///
	const sf3d::RenderLayer& getHelperRenderLayer() const;
	sf3d::RenderLayer& getHelperRenderLayer();

#ifdef EDITOR
	std::string currentFilePath;
	std::string getRelativePathFromExplorer(const std::string& title, const std::string& directory, const std::string& filter="", bool saveButton = false);
	bool menuSave();
#endif

private:

	///
	typename ScenesHolder_t::iterator _find(size_t id);

	///
	typename ScenesHolder_t::const_iterator _find(size_t id) const;

	ScenesHolder_t _holder;
	size_t _currentSceneID = 0u;

	ArmatureDisplayDataHolder_t _armatureDisplayDataHolder;

	/// Static shared helper layer to avoid unnecessary recreations.
	sf3d::RenderLayer _helperRenderLayer;

	/// Shader program used to render all objects (projeciton + world).
	sf3d::ShaderProgram _shaderProgram;

// Running state

	nlohmann::json _configBeforeRun;
	bool _gameIsRunning = false;

// Data

	TextureDataHolder _textureDataHolder;
};

}
