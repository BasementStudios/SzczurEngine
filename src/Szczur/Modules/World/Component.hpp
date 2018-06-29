#pragma once

#include <memory>
#include <vector>

#include <nlohmann/json.hpp>
using Json = nlohmann::json;

#include "UniqueID.hpp"
#include "Szczur/Utility/Convert/Hash.hpp"


namespace rat
{

// FWD
class Entity;
class ScenesManager;

class Component
{
public:

	enum Feature_e
	{
		Drawable = 1
	};

	///
	Component(Entity* parent, Hash64_t id, const std::string& name, size_t features = 0);

	///
	Component(const Component&) = default;

	///
	Component& operator = (const Component&) = default;

	///
	Component(Component&&) = default;

	///
	Component& operator = (Component&&) = default;

	///
	virtual ~Component() = default;

	///
	virtual std::unique_ptr<Component> copy(Entity* newParent) const = 0;

	///
	Hash64_t getComponentID() const;

	///
	virtual void* getFeature(Component::Feature_e);

	///
	virtual const void* getFeature(Component::Feature_e) const;

	///
	size_t getFeatures() const;

	///
	const std::string& getName() const;

	///
	void setEntity(Entity* entity);

	///
	Entity* getEntity();

	///
	const Entity* getEntity() const;

	///
	virtual void loadFromConfig(Json& config);

	///
	virtual void saveToConfig(Json& config) const;

	///
	void updateIDs();

	///
	virtual void renderHeader(ScenesManager& scenes, Entity* object);

protected:
	template<typename T>
	void drawOriginSetter(std::function<void(T*, int, int)> func)
	{
		// origin
		float size = 16.f;

		T* thisa = static_cast<T*>(this);

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0.f, 0.f });
		{
			// top
			{
				if (ImGui::Button("##left_top", { size, size }))
					func(thisa, -1, -1);

				ImGui::SameLine();

				if (ImGui::Button("##top", { size, size }))
					func(thisa, 0, -1);

				ImGui::SameLine();

				if (ImGui::Button("##right_top", { size, size }))
					func(thisa, 1, -1);
			}

			// center
			{
				if (ImGui::Button("##left", { size, size }))
					func(thisa, -1, 0);

				ImGui::SameLine();

				if (ImGui::Button("##center", { size, size }))
					func(thisa, 0, 0);

				ImGui::SameLine();

				if (ImGui::Button("##right", { size, size }))
					func(thisa, 1, 0);
			}

			// bottom
			{
				if (ImGui::Button("##left_bottom", { size, size }))
					func(thisa, -1, 1);

				ImGui::SameLine();

				if (ImGui::Button("##bottom", { size, size }))
					func(thisa, 0, 1);

				ImGui::SameLine();

				if (ImGui::Button("##right_bottom", { size, size }))
					func(thisa, 1, 1);
			}
		}
		ImGui::PopStyleVar();

		ImGui::Spacing();
	}

private:

	Hash64_t _id;
	size_t _features;
	std::string _name;
	Entity* _parent;

};

}
