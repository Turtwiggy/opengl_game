#include "modules/ui_hierarchy/system.hpp"

// components
#include "helpers.hpp"
#include "modules/physics/components.hpp"
#include "modules/renderer/components.hpp"
#include "modules/ui_hierarchy/components.hpp"

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
    const auto& tag = registry.get<TagComponent>(h.root).tag;
    imgui_draw_entity(registry, tag, h.root, h.selected_entity);

    // If select anywhere in the window, make entity unselected
    if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
      d.selected_entity = entt::null;

    // Right click on menu
    if (ImGui::BeginPopupContextWindow(0, 1, false)) {
      if (ImGui::MenuItem("Create Entity")) {
        entt::entity r = registry.create();
        registry.emplace<TagComponent>(r, std::string("Empty Entity"));
      }
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

    if (registry.all_of<PositionIntComponent>(eid)) {
      PositionIntComponent& pi = registry.get<PositionIntComponent>(eid);
      imgui_draw_ivec2(registry, "Pos: ", pi.x, pi.y);
    }

    if (registry.all_of<RenderSizeComponent>(eid)) {
      RenderSizeComponent& sc = registry.get<RenderSizeComponent>(eid);
      imgui_draw_ivec2(registry, "Render Size: ", sc.w, sc.h);
    }

    if (registry.all_of<RenderAngleComponent>(eid)) {
      RenderAngleComponent& ra = registry.get<RenderAngleComponent>(eid);
      imgui_draw_float(registry, "Render Angle:", ra.angle_radians);
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