#version 450

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 normal;

layout (location = 0) out vec3 frag_color;

layout (push_constant) uniform Push
{
    mat4 transform;
    vec3 color;
} push;

const vec3 DIRECTION_TO_LIGHT = normalize(vec3(1.0, -3.0, 1.0));
const float minimum_light_intensity = 0.005;
const float N = 16; // the smaller the N, the lighter the shadows

void main()
{
    gl_Position = push.transform * vec4(position, 1.0);

    float dot_product_clamped = (dot(normal, DIRECTION_TO_LIGHT) + 1) / 2;
    // X is ranging from 0 to 1
    float X = (dot_product_clamped * dot_product_clamped * dot_product_clamped * dot_product_clamped * dot_product_clamped * dot_product_clamped
        + sqrt(dot_product_clamped)/N) / (1 + 1/N);

    float light_intensity = max(X, minimum_light_intensity);
    frag_color = light_intensity * color;
}
