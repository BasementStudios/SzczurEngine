#pragma once

#include <memory>
#include <vector>

#include "Szczur/Modules/World/Data/ArmatureDisplayData.hpp"

#include "Scene.hpp"
#include "Data/TextureDataHolder.hpp"

#include <Json/json.hpp>
using Json = nlohmann::json;

namespace rat
{

class ScenesManager
{
public:

	using ScenesHolder_t = std::vector<std::unique_ptr<Scene>>;
	
	using ArmatureDisplayDataHolder_t = std::vector<ArmatureDisplayData>;

	///
	ScenesManager();

	///
	ScenesManager(const ScenesManager&) = delete;

	///
	ScenesManager& operator = (const ScenesManager&) = delete;

	///
	ScenesManager(ScenesManager&&) = delete;

	///
	ScenesManager& operator = (ScenesManager&&) = delete;

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
	void loadFromConfig(Json& config);

	///
	void saveToConfig(Json& config);

	///
	void saveEntityToConfig(Entity& entity, Json& config);

	///
	void loadFromFile(const std::string& filepath);

	///
	void saveToFile(const std::string& filepath);

	///
	void loadScenesFromFile(const std::string& filepath);

	///
	void addPlayer();

	/// Run game and save current config
	void runGame();

	/// Stop game and load saved config
	void stopGame();

	///
	bool isGameRunning();

	///
	Json& getRunConfig();

	///
	TextureDataHolder& getTextureDataHolder();

	#ifdef EDITOR
		std::string currentFilePath;
		std::string getRelativePathFromExplorer(const std::string& title, const std::string& directory, const std::string& filter="", bool saveButton = false);
		void menuSave();
	#endif

private:

	///
	typename ScenesHolder_t::iterator _find(size_t id);

	///
	typename ScenesHolder_t::const_iterator _find(size_t id) const;

	ScenesHolder_t _holder;
	size_t _currentSceneID = 0u;

	ArmatureDisplayDataHolder_t _armatureDisplayDataHolder;

// Running state

	Json _configBeforeRun;
	bool _gameIsRunning = false;

// Data

	TextureDataHolder _textureDataHolder;
};

}
