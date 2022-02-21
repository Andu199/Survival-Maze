#version 430

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float time;

// Output
layout(location = 0) out vec2 texture_coord;
layout(location = 1) out vec4 world_position;
layout(location = 2) out vec3 world_normal;

float rand(vec3 co){
    return fract(sin(dot(co, vec3(12.9898, 78.233, 24.1453))) * 4.5453);
}

void main()
{
    gl_Position = vec4(v_position, 1.0);
    world_position = Model * gl_Position;
    world_normal = mat3(Model) * v_normal;

    float res = rand(v_normal * time / 10000);
    gl_Position.z += v_normal.z * res;
    gl_Position.y += v_normal.y * res;
    gl_Position.x += v_normal.x * res;

    gl_Position = Projection * View * Model * gl_Position;
}
