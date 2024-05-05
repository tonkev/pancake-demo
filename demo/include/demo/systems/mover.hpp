#pragma once

#include "pancake/ecs/logic_system.hpp"

#include "pancake/util/guid.hpp"

namespace demo {
class Mover : public pancake::LogicSystem {
 public:
  using LogicSystem::LogicSystem;

  virtual std::string_view name() const override;
  virtual pancake::SystemId id() const override;

  virtual const pancake::SessionAccess& getSessionAccess() const override;
  virtual const pancake::ComponentAccess& getComponentAccess() const override;

 protected:
  virtual void _configure(pancake::Session& session) override;
  virtual void _run(const pancake::SessionWrapper& session,
                    const pancake::WorldWrapper& world) const override;
  pancake::GUID _select_material = pancake::GUID::null;
};
}  // namespace demo