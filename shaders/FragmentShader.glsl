#version 430

// Input
layout(location = 0) in vec2 texture_coord;
layout(location = 1) in vec4 world_position;
layout(location = 2) in vec3 world_normal;

// Output
layout(location = 0) out vec4 out_diffuse;
layout(location = 1) out vec4 out_world_pos;
layout(location = 2) out vec4 out_world_normal;

uniform float time;

void main()
{
    vec4 color = world_position;

    out_diffuse = color;
    out_world_pos = world_position;
    out_world_normal.xyz = normalize(world_normal);
}
