#pragma once
#include <memory>
#include "Scene.hpp"
#include "Szczur/Modules/Window/Window.hpp"
#include "Szczur/Modules/Input/Input.hpp"
#include "Szczur/Utility/Modules/Module.hpp"

namespace rat
{

class World : public Module<Window, Input>
{
public:

	using ScenesHolder_t = std::vector<std::unique_ptr<Scene>>;

	///
	World();

	///
	World(const World&) = delete;

	///
	World& operator = (const World&) = delete;

	///
	World(World&&) = delete;

	///
	World& operator = (World&&) = delete;

	///
	~World();

	///
	void update();

	///
	void render();

	///
	Scene* addScene();

	///
	bool removeScene(size_t id);

	///
	Scene* getScene(size_t id) const;

	///
	Scene* getCurrentScene() const;

	///
	bool hasScene(size_t id) const;

	///
	bool isCurrentSceneValid() const;

	///
	void removeAllScenes();

	///
	const ScenesHolder_t& getScenes() const;

private:

	///
	typename ScenesHolder_t::iterator _find(size_t id);

	///
	typename ScenesHolder_t::const_iterator _find(size_t id) const;

	ScenesHolder_t _holder;
	size_t _currentSceneID;

};

}
