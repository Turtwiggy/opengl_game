#include "modules/ui_hierarchy/system.hpp"

// components
#include "create_entities.hpp"
#include "modules/physics/components.hpp"
#include "modules/renderer/components.hpp"
#include "modules/sprites/components.hpp"
#include "modules/ui_hierarchy/components.hpp"
#include "modules/ui_hierarchy/helpers.hpp"

// other lib headers
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

void
game2d::update_ui_hierarchy_system(entt::registry& registry)
{
  auto& h = registry.ctx<SINGLETON_HierarchyComponent>();
  const auto& hroot = registry.get<EntityHierarchyComponent>(h.root_node);
  size_t root_entity_count = hroot.children.size();

  ImGui::Begin("Hierarchy", NULL, ImGuiWindowFlags_NoFocusOnAppearing);
  {
    ImGui::Text("Entity Count: %i", root_entity_count);

    // let root hierarchy entity be dropped on
    drop_accept_entity(registry, h.root_node);

    // skip showing the root node, go to children
    for (const auto& child : hroot.children) {
      const auto& tag = registry.get<TagComponent>(child).tag;
      imgui_draw_entity(registry, tag, child, h.selected_entity);
    }

    // If select anywhere in the window, make entity unselected
    if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
      h.selected_entity = entt::null;

    // Right click on menu
    if (ImGui::BeginPopupContextWindow(0, 1, false)) {
      if (ImGui::MenuItem("A menu item...!")) {
        //
      }
      ImGui::EndPopup();
    }
  }
  ImGui::End();

  //
  // If an entity is selected draw it's properties
  //

  ImGui::Begin("Properties", NULL, ImGuiWindowFlags_NoFocusOnAppearing);
  if (h.selected_entity != entt::null) {

    if (!registry.valid(h.selected_entity)) {
      ImGui::End();
      return; // make sure selected entity is valid
    }

    const auto& eid = h.selected_entity;

    if (registry.all_of<TagComponent>(eid)) {
      TagComponent& t = registry.get<TagComponent>(eid);
      imgui_draw_string(registry, "Tag: ", t.tag);
    }

    if (registry.all_of<TransformComponent>(eid)) {
      auto& transform = registry.get<TransformComponent>(eid);
      imgui_draw_ivec2(registry, "Pos: ", transform.position.x, transform.position.y);
      imgui_draw_ivec2(registry, "Render Size: ", transform.scale.x, transform.scale.y);
      imgui_draw_float(registry, "Render Angle:", transform.rotation.z);
    }

    if (registry.all_of<PhysicsSizeComponent>(eid)) {
      PhysicsSizeComponent& psc = registry.get<PhysicsSizeComponent>(eid);
      imgui_draw_ivec2(registry, "Physics Size: ", psc.w, psc.h);
    }

    if (registry.all_of<SpriteComponent>(eid)) {
      SpriteComponent& sc = registry.get<SpriteComponent>(eid);
      // ImGui::Text("Colour: ");
      // ImGui::SameLine();
      // ImGui::ColorEdit4("##colour", sc.colour.r, );
      imgui_draw_ivec2(registry, "Sprite: ", sc.x, sc.y);
    }

    if (registry.all_of<VelocityComponent>(eid)) {
      const VelocityComponent& c = registry.get<VelocityComponent>(eid);
      float x = c.x;
      float y = c.y;
      imgui_draw_vec2(registry, "Vel: ", x, y);
    }

    // Add component
    // if (ImGui::Button("Add component")) {
    //   ImGui::OpenPopup("AddComponent");
    // }

    // if (ImGui::BeginPopup("AddComponent")) {
    //   if (ImGui::MenuItem("PhysicsSizeComponent")) {
    //     registry.emplace<PhysicsSizeComponent>(eid);
    //     ImGui::CloseCurrentPopup();
    //   }
    //   if (ImGui::MenuItem("ColourComponent")) {
    //     registry.emplace<ColourComponent>(eid);
    //     ImGui::CloseCurrentPopup();
    //   }
    //   ImGui::EndPopup();
    // }
  }
  ImGui::End();
};