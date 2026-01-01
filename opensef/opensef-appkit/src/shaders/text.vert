#version 450

layout(location = 0) in vec2 inPos;
layout(location = 1) in vec2 inTexCoord;
layout(location = 2) in vec4 inColor;

layout(location = 0) out vec2 fragTexCoord;
layout(location = 1) out vec4 fragColor;

// Push constants for projection view matrix
layout(push_constant) uniform PushConstants {
    mat4 projection;
    mat4 view;
} pc;

void main() {
    gl_Position = pc.projection * pc.view * vec4(inPos, 0.0, 1.0);
    fragTexCoord = inTexCoord;
    fragColor = inColor;
}
