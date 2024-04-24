#include "systems/menu.hpp"

#include "components/viewer.hpp"

#include "pancake/components/3d.hpp"
#include "pancake/core/session_wrapper.hpp"
#include "pancake/ecs/components.hpp"
#include "pancake/ecs/world_wrapper.hpp"

#include "imgui/imgui.h"

using namespace pancake;
using namespace demo;

LogicSystem::StaticAdder<Menu> menu_adder{};

void Menu::_run(const SessionWrapper& session, const WorldWrapper& world) const {
  for (const auto& [viewer] : world.getComponents<Viewer>()) {
    ImGui::Begin("Menu");
    if (viewer->mode == ViewerMode::Camera) {
      ImGui::Text("Mode: Camera");
    } else {
      ImGui::Text("Mode: Object");
    }
    if ((viewer->mode == ViewerMode::Selecting) && (viewer->target == Entity::null)) {
      ImGui::Text("Hover mouse over an object");
    } else {
      ImGui::Text("Left Click: Move along view x/y axes");
      ImGui::Text("Middle Click: Move along view x/z axes");
      ImGui::Text("Right Click: Rotate");
    }
    ImGui::Text("Space: Change modes");
    ImGui::End();
  }
}

std::string_view Menu::name() const {
  return "Menu";
}

SystemId Menu::id() const {
  return System::id<Menu>();
}

const ComponentAccess& Menu::getComponentAccess() const {
  static const ComponentAccess component_access = Components::getAccess<Viewer>();
  return component_access;
}
