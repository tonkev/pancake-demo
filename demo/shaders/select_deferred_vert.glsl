#version 330 core
layout(location = 0) in vec4 v_position;
layout(location = 4) in vec2 v_tex_coords;
layout(location = 6) in mat4 i_mvp_transform;

out vec2 tex_coords;

void main()
{
    gl_Position = i_mvp_transform * vec4(v_position.xyz, 1.0);
    tex_coords = v_tex_coords;
}