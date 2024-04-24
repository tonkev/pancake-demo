#include "systems/pick_mouse_entity.hpp"

#include "components/viewer.hpp"

#include "pancake/core/input.hpp"
#include "pancake/core/renderer.hpp"
#include "pancake/core/session_access.hpp"
#include "pancake/core/session_wrapper.hpp"
#include "pancake/ecs/components.hpp"
#include "pancake/ecs/world_wrapper.hpp"

using namespace pancake;
using namespace demo;

DrawSystem::StaticAdder<PickMouseEntity> pick_mouse_entity_adder{};

void PickMouseEntity::_run(const SessionWrapper& session, const WorldWrapper& world) const {
  Renderer& renderer = session.renderer();
  const Vec2f mouse_position = renderer.screenToRenderPosition(session.input().getMousePosition());

  for (const auto& [base, fb_info, viewer] :
       world.getComponents<const Base, const FramebufferInfo, Viewer>()) {
    if (const auto& framebuffer_opt = renderer.getFramebuffer(base->guid);
        framebuffer_opt.has_value()) {
      Vec4u pixel;
      framebuffer_opt.value().get().readPixel(1, mouse_position, pixel);
      viewer->under_mouse.id = (static_cast<uint64_t>(pixel.y()) << 32) + pixel.x();
      viewer->under_mouse.gen = (static_cast<uint64_t>(pixel.w()) << 32) + pixel.z();
    }
  }
}

std::string_view PickMouseEntity::name() const {
  return "PickMouseEntity";
}

SystemId PickMouseEntity::id() const {
  return System::id<PickMouseEntity>();
}

const SessionAccess& PickMouseEntity::getSessionAccess() const {
  static const SessionAccess session_access = SessionAccess().addRenderer();
  return session_access;
}

const ComponentAccess& PickMouseEntity::getComponentAccess() const {
  static const ComponentAccess component_access =
      Components::getAccess<const Base, const FramebufferInfo, Viewer>();
  return component_access;
}
