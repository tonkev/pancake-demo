#version 330 core
layout (location = 0) out vec4 frag_color;

in vec2 tex_coords;

uniform sampler2D color_texture;
uniform usampler2D pick_texture;

uniform float select_period;
uniform vec4 select_color;
uniform uvec4 select_entity;

uniform float time;

void main()
{
    frag_color = texture2D(color_texture, tex_coords);
    uvec4 pick = texture(pick_texture, tex_coords);

    float select_blend = mod(time, select_period * 0.5);
    if ((select_period * 0.5) < mod(time, select_period)) {
      select_blend = 1 - select_blend;
    }

    if ((0 < select_period) && (pick == select_entity)) {
      frag_color = mix(frag_color, select_color, select_blend);
    }
}