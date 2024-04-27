#include "session_config_rules.hpp"

using namespace pancake;
using namespace demo;

void GltfScenePathRule::operator()(CmdLineOptions& options, std::string_view option) {
  _path = options.consume();
}

const std::set<std::string>& GltfScenePathRule::getOptions() const {
  static const std::set<std::string> options{"--gltf-scene"};
  return options;
}

const std::string& GltfScenePathRule::path() const {
  return _path;
}

SessionConfigRule::StaticAdder<GltfScenePathRule> _gltf_scene_path_rule_adder;