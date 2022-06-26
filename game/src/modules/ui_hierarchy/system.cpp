#include "modules/ui_hierarchy/system.hpp"

// components
#include "create_entities.hpp"
#include "modules/physics/components.hpp"
#include "modules/renderer/components.hpp"
#include "modules/ui_hierarchy/components.hpp"
#include "modules/ui_hierarchy/helpers.hpp"

// other lib headers
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

void
game2d::update_ui_hierarchy_system(entt::registry& registry, engine::Application& app)
{
  auto& d = registry.ctx<SINGLETON_HierarchyComponent>();

  ImGui::Begin("Hierarchy", NULL, ImGuiWindowFlags_NoFocusOnAppearing);
  {
    auto& h = registry.ctx<SINGLETON_HierarchyComponent>();

    // let root hierarchy entity be dropped on
    drop_accept_entity(registry, h.root_node);

    // skip showing the root node, go to children
    const auto& hroot = registry.get<EntityHierarchyComponent>(h.root_node);
    for (const auto& child : hroot.children) {
      const auto& tag = registry.get<TagComponent>(child).tag;
      imgui_draw_entity(registry, tag, child, h.selected_entity);
    }

    // If select anywhere in the window, make entity unselected
    if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
      d.selected_entity = entt::null;

    // Right click on menu
    if (ImGui::BeginPopupContextWindow(0, 1, false)) {
      if (ImGui::MenuItem("Create Entity"))
        create_empty(registry);
      ImGui::EndPopup();
    }
  }
  ImGui::End();

  //
  // If an entity is selected draw it's properties
  //

  ImGui::Begin("Properties", NULL, ImGuiWindowFlags_NoFocusOnAppearing);
  if (d.selected_entity != entt::null) {
    const auto& eid = d.selected_entity;

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

    if (registry.all_of<ColourComponent>(eid)) {
      ColourComponent& c = registry.get<ColourComponent>(eid);
      ImGui::Text("Colour: ");
      ImGui::SameLine();
      ImGui::ColorEdit4("##colour", glm::value_ptr(c.colour));
    }

    if (registry.all_of<VelocityComponent>(eid)) {
      VelocityComponent& c = registry.get<VelocityComponent>(eid);
      imgui_draw_vec2(registry, "Vel: ", c.x, c.y);
    }

    if (registry.all_of<SpriteSlotComponent>(eid)) {
      SpriteSlotComponent& c = registry.get<SpriteSlotComponent>(eid);
      imgui_draw_ivec2(registry, "Sprite: ", c.x, c.y);
    }

    // Add component
    // if (ImGui::Button("Add component")) {
    //   ImGui::OpenPopup("AddComponent");
    // }

    // if (ImGui::BeginPopup("AddComponent")) {
    //   if (ImGui::MenuItem("PositionIntComponent")) {
    //     registry.emplace<PositionIntComponent>(eid);
    //     ImGui::CloseCurrentPopup();
    //   }
    //   if (ImGui::MenuItem("RenderSizeComponent")) {
    //     registry.emplace<RenderSizeComponent>(eid);
    //     ImGui::CloseCurrentPopup();
    //   }
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