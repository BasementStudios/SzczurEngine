#pragma once

#include <vector>
#include <string>

namespace rat {
class ScenesManager;
class Entity;

class ObjectsList {
public:
    ObjectsList(ScenesManager& scenes);
    void render(bool& ifRender);
    void update();

    /// Get id of selected entity (or get -1)
    int getSelectedID();

    ///
    Entity* getSelectedEntity();

    /// Return true if any object is selected
    bool isAnySingleEntitySelected();

	///
	bool isAnyEntitySelected();

	///
	bool isEntitySelected(Entity* entity);

    /// Unselect selected object
    void unselect();

    /// Select specific object
    void select(Entity* object);

    /// Select specific object by id
    void select(int id);

	/// Add to selected entities
	void addSelected(Entity* id);

	/// clear selected entites
	void clearSelected();

	/// 
	const std::vector<Entity*>& getSelectedEntities();

	///
	bool isGroupSelected();

    /// Create new object in group
    void addObject(const std::string& groupName);

    /// Create new object in selected group
    void addObjectToCurrentGroup();

    ///
    void duplicateObject(int id);

    ///
    void removeObject(int id);

private:

    /// Show context menu for object | Return true when number of objects changes
    bool _renderObjectPopup(int id);

private:
    ScenesManager& _scenes;
    std::string _tab;
    int _selectedEntityID = -1;
    int _contextMenuEntityID = -1;

	std::vector<Entity*> _selectedEntities;
};
}
