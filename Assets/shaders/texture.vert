#version 450

layout(location = 0) in vec2 vertexPosition;
layout(location = 1) in vec3 vertexColor;
layout(location = 2) in vec2 vertexTexCoord;

layout (push_constant) uniform constants {
    mat4 model;
} objectData;

layout (binding = 0) uniform UniformBufferObject {
    mat4 view;
    mat4 proj;
} ubo;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;

void main() {
    gl_Position = ubo.proj * ubo.view * objectData.model * vec4(vertexPosition, 0.0, 1.0);
    fragColor = vertexColor;
    fragTexCoord = vertexTexCoord;
}