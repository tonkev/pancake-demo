#version 330 core
layout(location = 0) in vec4 v_position;
layout(location = 1) in vec4 v_normal;
layout(location = 4) in vec2 v_tex_coords;
layout(location = 6) in mat4 i_mvp_transform;
layout(location = 10) in mat4 i_model_transform;
layout(location = 14) in uvec4 i_entity;

out vec3 position;
out vec3 normal;
out vec2 tex_coords;

out vec3 light_dir;
out vec3 view_dir;

flat out uvec4 entity;

uniform vec4 base_color_texture_transform;

uniform vec3 light_position;
uniform vec3 view_position;

void main()
{
    gl_Position = i_mvp_transform * vec4(v_position.xyz, 1.0);
    
    position = (i_model_transform * vec4(v_position.xyz, 1.0)).xyz;
    normal = (i_model_transform * vec4(v_normal.xyz, 0.0)).xyz;
    tex_coords = (v_tex_coords * base_color_texture_transform.zw) + base_color_texture_transform.xy;

    light_dir = normalize(light_position - position.xyz);
    view_dir = normalize(view_position - position.xyz);
    
    entity = i_entity;
}