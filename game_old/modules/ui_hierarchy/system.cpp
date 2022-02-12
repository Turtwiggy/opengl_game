#include "modules/ui_hierarchy/system.hpp"

// components
#include "modules/editor_camera/components.hpp"
#include "modules/physics/components.hpp"
#include "modules/renderer/components.hpp"
#include "modules/ui_hierarchy/components.hpp"

// other lib headers
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

void
game2d::init_ui_hierarchy_system(game2d::Coordinator& registry)
{
  registry.set<SINGLETON_HierarchyComponent>(SINGLETON_HierarchyComponent());
}

void
game2d::update_ui_hierarchy_system(game2d::Coordinator& registry, engine::Application& app)
{
  auto& d = registry.ctx<SINGLETON_HierarchyComponent>();

  ImGui::Begin("Hierarchy", NULL, ImGuiWindowFlags_NoFocusOnAppearing);
  {
    // List all entities...
    registry.each([&registry, &d](auto entity) {
      const std::string& tag = registry.get<TagComponent>(entity).tag;
      const auto& eid = d.selected_entity;

      if (registry.all_of<EditorCameraComponent>(entity))
        return; // skip editor camera from hierarchy

      ImGuiTreeNodeFlags flags = ((eid == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
      flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
      bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());

      if (ImGui::IsItemClicked())
        d.selected_entity = entity;

      // Right click on the dropdown entry to delete it
      bool delete_entity = false;
      if (ImGui::BeginPopupContextItem()) {
        if (ImGui::MenuItem("Delete Entity")) {
          delete_entity = true;
        }
        ImGui::EndPopup();
      }

      if (opened) {
        // ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
        // bool opened = ImGui::TreeNodeEx((void*)9817239, flags, tag.c_str());
        // if (opened)
        //   ImGui::TreePop();
        ImGui::Text("a node");
        ImGui::TreePop();
      }

      if (delete_entity && eid != entt::null) {
        registry.destroy(eid);
        if (eid == d.selected_entity) {
          d.selected_entity = entt::null;
        }
      }
    });

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

  ImGui::Begin("Properties", NULL, ImGuiWindowFlags_NoFocusOnAppearing);
  if (d.selected_entity != entt::null) {
    const auto& eid = d.selected_entity;

    // Display TagComponent
    if (registry.all_of<TagComponent>(eid)) {
      TagComponent& t = registry.get<TagComponent>(eid);

      // Able to change the value of the tag component
      char buffer[256];
      memset(buffer, 0, sizeof(buffer));
      strcpy_s(buffer, t.tag.c_str());

      ImGui::Text("Tag: ");
      ImGui::SameLine();
      if (ImGui::InputText("##tagbox", buffer, sizeof(buffer)))
        t.tag = std::string(buffer);
    }

    // Display PositionIntComponent
    if (registry.all_of<PositionIntComponent>(eid)) {
      PositionIntComponent& pi = registry.get<PositionIntComponent>(eid);
      glm::ivec2 pos = { pi.x, pi.y };

      ImGui::Text("Pos: ");
      ImGui::SameLine();
      if (ImGui::DragInt2("##position", glm::value_ptr(pos), 0.5f)) {
        pi.x = pos.x;
        pi.y = pos.y;
      }
    }

    // Display RenderSizeComponent
    if (registry.all_of<RenderSizeComponent>(eid)) {
      RenderSizeComponent& sc = registry.get<RenderSizeComponent>(eid);

      // Able to change the value of PositionInt component
      glm::ivec2 size = { sc.w, sc.h };

      ImGui::Text("Render Size: ");
      ImGui::SameLine();
      if (ImGui::DragInt2("##rendersize", glm::value_ptr(size), 0.5f)) {
        sc.w = size.x;
        sc.h = size.y;
      }
    }

    // Display PhysicsSizeComponent
    if (registry.all_of<PhysicsSizeComponent>(eid)) {
      PhysicsSizeComponent& psc = registry.get<PhysicsSizeComponent>(eid);

      // Able to change the value of PositionInt component
      glm::ivec2 size = { psc.w, psc.h };

      ImGui::Text("Physics Size: ");
      ImGui::SameLine();
      if (ImGui::DragInt2("##physicssize", glm::value_ptr(size), 0.5f)) {
        psc.w = size.x;
        psc.h = size.y;
      }
    }

    // Display ColourComponent
    if (registry.all_of<ColourComponent>(eid)) {
      ColourComponent& c = registry.get<ColourComponent>(eid);

      ImGui::Text("Colour: ");
      ImGui::SameLine();
      ImGui::ColorEdit4("##colour", glm::value_ptr(c.colour));
    }

    // Display VelocityComponent
    if (registry.all_of<VelocityComponent>(eid)) {
      VelocityComponent& c = registry.get<VelocityComponent>(eid);

      // Able to change the value of PositionInt component
      glm::ivec2 vel = { c.x, c.y };

      ImGui::Text("Vel: ");
      ImGui::SameLine();
      if (ImGui::DragInt2("##velocity", glm::value_ptr(vel), 0.5f)) {
        c.x = vel.x;
        c.y = vel.y;
      }
    }

    // Add component
    if (ImGui::Button("Add component")) {
      ImGui::OpenPopup("AddComponent");
    }

    if (ImGui::BeginPopup("AddComponent")) {
      if (ImGui::MenuItem("PositionIntComponent")) {
        registry.emplace<PositionIntComponent>(eid);
        ImGui::CloseCurrentPopup();
      }
      if (ImGui::MenuItem("RenderSizeComponent")) {
        registry.emplace<RenderSizeComponent>(eid);
        ImGui::CloseCurrentPopup();
      }
      if (ImGui::MenuItem("PhysicsSizeComponent")) {
        registry.emplace<PhysicsSizeComponent>(eid);
        ImGui::CloseCurrentPopup();
      }
      if (ImGui::MenuItem("ColourComponent")) {
        registry.emplace<ColourComponent>(eid);
        ImGui::CloseCurrentPopup();
      }
      if (ImGui::MenuItem("SpriteComponent")) {
        registry.emplace<SpriteComponent>(eid);
        ImGui::CloseCurrentPopup();
      }
      ImGui::EndPopup();
    }
  }
  ImGui::End();
};