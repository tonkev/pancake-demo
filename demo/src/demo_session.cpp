#include "demo_session.hpp"

#include "components/viewer.hpp"
#include "session_config_rules.hpp"

#include "pancake/components/3d.hpp"
#include "pancake/core/renderer.hpp"
#include "pancake/graphics/material.hpp"
#include "pancake/graphics/mesh.hpp"
#include "pancake/resources/gl3_shader_resource.hpp"
#include "pancake/resources/gltf_resource.hpp"
#include "pancake/resources/material_resource.hpp"
#include "pancake/resources/obj_mesh_resource.hpp"
#include "pancake/resources/obj_resource.hpp"
#include "pancake/resources/resources.hpp"
#include "pancake/util/gltf_importer.hpp"

using namespace pancake;
using namespace demo;

void DemoSession::registerComponents() const {
  Session::registerComponents();
  Components::get().add(TypeDescLibrary::get<Viewer>());
}

void DemoSession::configure() {
  GUID gooch_forwad_shader_guid;
  if (const auto& res =
          resources().getOrCreate<GL3ShaderResource>("shaders/gooch_forward.shader", false);
      res.has_value()) {
    GL3ShaderResource& gooch_forward_shader_res = res.value();

    gooch_forward_shader_res.setVertexSourceGuid(
        resources().getOrCreate<TextResource>("shaders/gooch_forward_vert.glsl").guid());
    gooch_forward_shader_res.setFragmentSourceGuid(
        resources().getOrCreate<TextResource>("shaders/gooch_forward_frag.glsl").guid());

    gooch_forwad_shader_guid = gooch_forward_shader_res.guid();
  }

  GUID gooch_forwad_mat_guid;
  if (const auto& res =
          resources().getOrCreate<MaterialResource>("materials/gooch_forward.material", false);
      res.has_value()) {
    MaterialResource& gooch_forward_mat_res = res.value();

    gooch_forward_mat_res.setShader(gooch_forwad_shader_guid);
    gooch_forward_mat_res.setLightPassInputName("light");
    gooch_forward_mat_res.setViewInputName("view");

    gooch_forwad_mat_guid = gooch_forward_mat_res.guid();
  }

  renderer().matchRenderSizeToScreenSize(true);

  Ptr<World> world{new World()};

  EntityWrapper cam_ent = world->createEntity();
  Transform3D& cam_transform = cam_ent.addComponent<Transform3D>();
  cam_transform.modify().translation() = Vec3f(0.f, 0.f, 2.f);
  Camera3D& camera = cam_ent.addComponent<Camera3D>();
  camera.perspective = true;
  camera.fov = 120.f;
  camera.framebuffer = cam_ent.getComponent<Base>().guid;
  camera.mask = CameraMask().with(0);
  FramebufferInfo& fb_info = cam_ent.addComponent<FramebufferInfo>();
  fb_info.render_targets[0].clear_colour = Vec4f(0.f, 0.f, 0.f, 1.f);
  fb_info.render_targets[1].clear_colour = Vec4f(0.f, 0.f, 0.f, 0.f);
  fb_info.render_targets[1].format = BufferFormat::RGBA32UI;
  fb_info.blit_priority = -1;
  fb_info.num_targets = 2;
  cam_ent.addComponent<Viewer>();
  const GUID cam_ent_guid = cam_ent.getComponent<Base>().guid;

  GUID select_shader_guid;
  if (const auto& res =
          resources().getOrCreate<GL3ShaderResource>("shaders/select_deferred.shader", false);
      res.has_value()) {
    GL3ShaderResource& select_shader_res = res.value();

    select_shader_res.setVertexSourceGuid(
        resources().getOrCreate<TextResource>("shaders/select_deferred_vert.glsl").guid());
    select_shader_res.setFragmentSourceGuid(
        resources().getOrCreate<TextResource>("shaders/select_deferred_frag.glsl").guid());

    select_shader_guid = select_shader_res.guid();
  }

  GUID select_mat_guid;
  if (const auto& res =
          resources().getOrCreate<MaterialResource>("materials/select_deferred.material", false);
      res.has_value()) {
    MaterialResource& select_mat_res = res.value();

    select_mat_res.setStage(20000);
    select_mat_res.setShader(select_shader_guid);
    select_mat_res.addInput(ShaderInput("color_texture", TextureRef(cam_ent_guid, 0)));
    select_mat_res.addInput(ShaderInput("pick_texture", TextureRef(cam_ent_guid, 1)));
    select_mat_res.addInput(ShaderInput("select_period", 0.f));
    select_mat_res.addInput(ShaderInput("select_color", Vec4f(1.0f, 0.5f, 0.0f, 1.f)));
    select_mat_res.setLightPassInputName("light");
    select_mat_res.setViewInputName("view");

    select_mat_guid = select_mat_res.guid();
  }

  EntityWrapper select_ent = world->createEntity();
  select_ent.addComponent<Transform3D>();
  MeshInstance& mesh_inst = select_ent.addComponent<MeshInstance>();
  mesh_inst.mesh = renderer().getUnitSquare()->guid();
  mesh_inst.camera_mask = CameraMask().with(1);
  select_ent.addComponent<MaterialInstance>().material = select_mat_guid;
  Camera3D& select_cam = select_ent.addComponent<Camera3D>();
  select_cam.framebuffer = GUID::null;  // screen framebuffer
  select_cam.mask = CameraMask().with(1);
  select_cam.perspective = false;

  if (const auto* rule = config().getRule<GltfScenePathRule>(); nullptr != rule) {
    if (const auto res = resources().getOrCreate<GltfResource>(rule->path()); res.has_value()) {
      GltfImporter(gooch_forwad_mat_guid).import(*world, resources(), res.value());
    }
  }

  EntityWrapper light_ent = world->createEntity();
  light_ent.addComponent<Transform3D>().modify().translation() = Vec3f(0.f, 20.f, 0.f);
  light_ent.addComponent<PointLight>().color = Vec4f(0.f, 1.f, 1.f, 1.f);

  addWorld(world);
}