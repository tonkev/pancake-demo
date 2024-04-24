#pragma once

#include "pancake/ecs/draw_system.hpp"
#include "pancake/util/guid.hpp"

namespace demo {
class PickMouseEntity : public pancake::DrawSystem {
 public:
  using DrawSystem::DrawSystem;

  virtual std::string_view name() const override;
  virtual pancake::SystemId id() const override;

  virtual const pancake::SessionAccess& getSessionAccess() const override;
  virtual const pancake::ComponentAccess& getComponentAccess() const override;

 protected:
  virtual void _run(const pancake::SessionWrapper& session,
                    const pancake::WorldWrapper& world) const override;
};
}  // namespace demo