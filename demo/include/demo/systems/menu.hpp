#pragma once

#include "pancake/ecs/logic_system.hpp"

namespace demo {
class Menu : public pancake::LogicSystem {
 public:
  using LogicSystem::LogicSystem;

  virtual std::string_view name() const override;
  virtual pancake::SystemId id() const override;

  virtual const pancake::ComponentAccess& getComponentAccess() const override;

 protected:
  virtual void _run(const pancake::SessionWrapper& session,
                    const pancake::WorldWrapper& world) const override;
};
}  // namespace demo