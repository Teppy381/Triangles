#version 450

layout (location = 0) in vec3 frag_color;
layout (location = 0) out vec4 out_color;

void main()
{
    // outColor = vec4(0.6, 0.9, 0.6, 1.0);
    out_color = vec4(frag_color, 1.0);
}
