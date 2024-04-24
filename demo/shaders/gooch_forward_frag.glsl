#version 330 core
layout (location = 0) out vec4 frag_color;
layout (location = 1) out uvec4 pick;

in vec3 position;
in vec3 normal;
in vec3 light_dir;
in vec3 view_dir;
flat in uvec4 entity;

uniform vec4 surface_color;

uniform float select_period;
uniform vec4 select_color;

uniform float time;

void main()
{
    vec4 cool_color = vec4(vec3(0, 0, 0.55) + (0.25 * surface_color.xyz), surface_color.w);
    vec4 warm_color = vec4(vec3(0.3, 0.3, 0) + (0.25 * surface_color.xyz), surface_color.w);
    vec4 highlight_color = vec4(1, 1, 1, 1);

    float normalDotLightDir = dot(normal, light_dir);
    float t = (normalDotLightDir + 1) * 0.5;
    vec3 r = (2 * normalDotLightDir * normal) - light_dir;
    float s = clamp((100 * dot(r, view_dir)) - 97, 0, 1);
    
    float select_blend = mod(time, select_period * 0.5);
    if ((select_period * 0.5) < mod(time, select_period)) {
      select_blend = 1 - select_blend;
    }

    frag_color = mix(mix(cool_color, warm_color, t), highlight_color, s);

    if (0 < select_period) {
      frag_color = mix(frag_color, select_color, select_blend);
    }
    
    pick = entity;
}