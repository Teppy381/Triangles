#version 450

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
// layout (location = 2) in vec3 normal;

layout (location = 0) out vec3 frag_color;

layout (push_constant) uniform Push
{
    mat4 transform;
    vec3 color;
} push;

// const vec3 DIRECTION_TO_LIGHT = normalize(vec3(1.0, -3.0, -1.0));

void main()
{
    gl_Position = push.transform * vec4(position, 1.0);

    // float light_intensity = max(dot(normal, DIRECTION_TO_LIGHT), 0.1);
    // frag_color = light_intensity * color;
    frag_color = color;
}
