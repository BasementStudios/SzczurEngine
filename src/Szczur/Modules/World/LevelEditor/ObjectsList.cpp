#include "ObjectsList.hpp"

#include <ImGui/imgui.h>

#include "../ScenesManager.hpp"
#include "../Entity.hpp"
#include "../Scene.hpp"

namespace rat {
    ObjectsList::ObjectsList(ScenesManager& scenes) :
    _scenes{scenes} {

    }

    void ObjectsList::render() {
        if(_ifRenderObjectsList) {
            if(ImGui::Begin("Objects", &_ifRenderObjectsList)) {
                ImGui::Separator();
                if(ImGui::BeginChild("Objects")) {
                    int i = 0;
                    auto* scene = _scenes.getCurrentScene();
                    for(auto& group : scene->getAllEntities()) {
                        ImGui::PushID(i);

                        ImGui::PushStyleColor(ImGuiCol_Header, {0.f, 0.f, 0.f, 0.f});
                        if(ImGui::TreeNodeEx(group.first.c_str(), ImGuiTreeNodeFlags_FramePadding|ImGuiTreeNodeFlags_DefaultOpen|ImGuiTreeNodeFlags_Framed)) {
                            ImGui::PopStyleColor();

                            // Context menu for group. Options: Add object
                            if(group.first != "single" && ImGui::BeginPopupContextItem("Group context menu")) {
                                if(ImGui::Selectable("Add object##group_context_menu")) {
                                    Entity* ent = scene->addEntity(group.first);
                                    if(ent) {
                                        scene->focusedObject = ent->getID();
                                    }
                                    scene->anySelected = static_cast<bool>(ent);
                                }
                                ImGui::EndPopup();
                            }


                            for(int i2 = 0; i2<group.second.size(); ++i2) {
                                Entity& object = *group.second[i2];
                                bool temp = object.getID() == scene->focusedObject && scene->anySelected;
                                ImGui::PushID(i2);
                                if(ImGui::Selectable(object.getName().c_str(), temp)) {
                                    if(!temp) {
                                        scene->focusedObject = object.getID();
                                    }
                                    scene->anySelected = !temp;
                                }

                                // Swapping objects in list
                                static int draggedObject = -1;
                                static std::string draggedCategory;
                                if(ImGui::IsMouseReleased(0)) {
                                    draggedObject = -1;
                                }
                                if(ImGui::IsItemClicked()) {
                                    draggedObject = i2;
                                    draggedCategory = group.first;
                                }
                                if(ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem)) {
                                    if(draggedObject != -1 && draggedObject != i2 && draggedCategory == group.first) {
                                        std::swap(group.second[i2], group.second[draggedObject]);
                                        draggedObject = i2;
                                    }
                                }

                                // Context menu for object. Options: Duplicate|Remove
                                if(group.first != "single") {
                                    if(ImGui::BeginPopupContextItem("Object context menu")) {
                                        if(ImGui::Selectable("Duplicate##object_context_menu")) {
                                            Entity* ent = scene->duplicateEntity(group.second[i2]->getID());
                                            if(ent) {
                                                scene->focusedObject = ent->getID();
                                            }
                                            scene->anySelected = static_cast<bool>(ent);
                                        }
                                        if(ImGui::Selectable("Remove##object_context_menu")) {
                                            if(temp)
                                                scene->anySelected = false;
                                            group.second.erase(group.second.begin() + i2);
                                            ImGui::EndPopup();
                                            ImGui::PopID();
                                            break;
                                        }
                                        ImGui::EndPopup();
                                    }
                                }

                                ImGui::PopID();
                            }
                            ImGui::TreePop();
                        }
                        else {
                            ImGui::PopStyleColor();

                            // Context menu for group. Options: Add object
                            if(group.first != "single" && ImGui::BeginPopupContextItem("Group context menu")) {
                                if(ImGui::Selectable("Add object##group_context_menu")) {
                                    Entity* ent = scene->addEntity(group.first);
                                    if(ent) {
                                        scene->focusedObject = ent->getID();
                                    }
                                    scene->anySelected = static_cast<bool>(ent);
                                }
                                ImGui::EndPopup();
                            }

                        }
                        ImGui::Separator();
                        ImGui::PopID();
                        ++i;
                    }
                }
                ImGui::EndChild();

            }
            ImGui::End();
        }
    }

    void ObjectsList::update() {

    }

    bool& ObjectsList::getBool() {
        return _ifRenderObjectsList;
    }
}