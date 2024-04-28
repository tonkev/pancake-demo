#include "demo_session.hpp"

#include "pancake/core/session.hpp"

using namespace pancake;
using namespace demo;

Session* Session::create(SessionConfig&& config) {
  return new DemoSession(std::move(config));
}