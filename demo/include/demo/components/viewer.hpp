#pragma once

#include "pancake/ecs/common.hpp"
#include "pancake/util/pstruct.hpp"

namespace demo {
enum class ViewerMode { Camera, Selecting, Moving };

PSTRUCT(Viewer)
PSTRUCT_MEMBER_INITIALISED(ViewerMode, mode, ViewerMode::Camera)
PSTRUCT_MEMBER_INITIALISED(float, yaw, 0.f)
PSTRUCT_MEMBER_INITIALISED(float, pitch, 0.f)
PSTRUCT_MEMBER_INITIALISED(pancake::Entity, target, pancake::Entity::null)
PSTRUCT_MEMBER_INITIALISED(pancake::Entity, under_mouse, pancake::Entity::null)
PSTRUCT_END()
};  // namespace demo