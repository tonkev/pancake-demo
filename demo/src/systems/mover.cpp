#include "systems/mover.hpp"

#include "components/viewer.hpp"

#include "pancake/components/3d.hpp"
#include "pancake/core/input.hpp"
#include "pancake/core/session_wrapper.hpp"
#include "pancake/ecs/components.hpp"
#include "pancake/ecs/world_wrapper.hpp"
#include "pancake/resources/material_resource.hpp"
#include "pancake/resources/resources.hpp"

using namespace pancake;
using namespace demo;

LogicSystem::StaticAdder<Mover> mover_adder{};

void Mover::_configure(pancake::Session& session) {
  _regular_material = session.resources()
                          .getOrCreate<MaterialResource>("materials/gooch_forward.material", false)
                          .guid();

  _select_material =
      session.resources()
          .getOrCreate<MaterialResource>("materials/gooch_forward_select.material", false)
          .guid();
}

void Mover::_run(const SessionWrapper& session, const WorldWrapper& world) const {
  const Input& input = session.input();

  const float mouse_sensitivity = 0.01f;

  bool move = false;
  bool rotate = false;
  float flip = 1.f;
  Vec4f move_x_dir = Vec4f(-1.f, 0.f, 0.f, 0.f);
  Vec4f move_y_dir = Vec4f::zeros();

  const Vec2f mouse_delta = input.getMouseDelta() * mouse_sensitivity;
  if (input.isMousePressed(MouseCode::Left)) {
    move = true;
    move_y_dir = Vec4f(0.f, 1.f, 0.f, 0.f);
  } else if (input.isMousePressed(MouseCode::Middle)) {
    move = true;
    move_y_dir = Vec4f(0.f, 0.f, -1.f, 0.f);
  } else if (input.isMousePressed(MouseCode::Right)) {
    rotate = true;
  }

  for (const auto& [transform, viewer] : world.getComponents<Transform3D, Viewer>()) {
    Transform3D* target_transform = nullptr;

    if (viewer->mode == ViewerMode::Camera) {
      target_transform = transform;

      if (input.isKeyJustPressed(KBCode::Space)) {
        viewer->mode = ViewerMode::Selecting;
      }
    } else {
      flip = -1.f;

      if (viewer->target == Entity::null) {
        viewer->target = viewer->under_mouse;
      }

      MaterialInstance* target_material_instance = nullptr;
      if (viewer->target != Entity::null) {
        EntityWrapper target = world.getEntityWrapper(viewer->target);
        if (target.isValid() && target.hasComponents<Transform3D, MaterialInstance>()) {
          target_material_instance = &target.getComponent<MaterialInstance>();

          target_material_instance->material = _select_material;

          if (viewer->mode == ViewerMode::Selecting) {
            if (move || rotate) {
              viewer->mode = ViewerMode::Moving;
            } else if (viewer->target != viewer->under_mouse) {
              target_material_instance->material = _regular_material;
              target_material_instance = nullptr;

              viewer->target = Entity::null;
            }
          } else if (viewer->mode == ViewerMode::Moving) {
            if (move || rotate) {
              target_transform = &target.getComponent<Transform3D>();
            } else {
              viewer->mode = ViewerMode::Selecting;
            }
          }
        } else {
          viewer->target = Entity::null;
        }
      }

      if (input.isKeyJustPressed(KBCode::Space)) {
        viewer->mode = ViewerMode::Camera;

        if (nullptr != target_material_instance) {
          target_material_instance->material = _regular_material;
        }
      }
    }

    if (nullptr != target_transform) {
      if (move) {
        Mat4f matrix = transform->matrix();
        move_x_dir = matrix * move_x_dir * flip;
        move_y_dir = matrix * move_y_dir * flip;

        target_transform->modify().translation() +=
            (move_x_dir.xyz() * mouse_delta.x()) + (move_y_dir.xyz() * mouse_delta.y());
      } else if (rotate) {
        if (transform == target_transform) {
          viewer->yaw += mouse_delta.x();
          viewer->pitch += mouse_delta.y();

          target_transform->modify().rotation() = QuaternionF(viewer->yaw, Vec3f(0.f, -1.f, 0.f)) *
                                                  QuaternionF(viewer->pitch, Vec3f(-1.f, 0.f, 0.f));
        } else {
          target_transform->modify().rotation() *=
              QuaternionF(mouse_delta.x(), Vec3f(0.f, 1.f, 0.f)) *
              QuaternionF(mouse_delta.y(), Vec3f(1.f, 0.f, 0.f));
        }
      }
    }
  }
}

std::string_view Mover::name() const {
  return "Mover";
}

SystemId Mover::id() const {
  return System::id<Mover>();
}

const ComponentAccess& Mover::getComponentAccess() const {
  static const ComponentAccess component_access =
      Components::getAccess<Transform3D, MaterialInstance, Viewer>();
  return component_access;
}
