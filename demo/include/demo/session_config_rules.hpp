#pragma once

#include "pancake/core/session_config.hpp"

namespace demo {
class GltfScenePathRule : public pancake::SessionConfigRule {
 public:
  virtual ~GltfScenePathRule() = default;
  virtual void operator()(pancake::CmdLineOptions& options, std::string_view option) override;
  virtual const std::set<std::string>& getOptions() const override;
  const std::string& path() const;

 private:
  std::string _path = "";
};
}  // namespace demo