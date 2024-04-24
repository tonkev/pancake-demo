#pragma once

#include "ecs/components.hpp"
#include "pancake/core/session.hpp"

namespace demo {
class DemoSession : public pancake::Session {
 protected:
  using pancake::Session::Session;
  virtual ~DemoSession() = default;

  virtual void registerComponents() const override;
  virtual void configure() override;
};
}  // namespace demo