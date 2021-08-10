#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 modelView;
    mat4 modelViewProj;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inTangent;
layout(location = 3) in vec3 inBitangent;
layout(location = 4) in vec2 inTexCoord;

layout(location = 0) out vec3 viewPos;
layout(location = 1) out vec3 viewNormal;
layout(location = 2) out vec3 worldNormal;
layout(location = 3) out vec2 texCoord;
layout(location = 4) out mat3 tangentBasis;

void main() 
{
    gl_Position = ubo.modelViewProj * vec4(inPosition, 1.0);
    viewPos = vec3(ubo.modelView * vec4(inPosition, 1.0));
    viewNormal = mat3(ubo.modelView) * inNormal;
    worldNormal = mat3(ubo.model) * inNormal;
    texCoord = inTexCoord;
    vec3 T = mat3(ubo.modelView)*inTangent;
    vec3 B = mat3(ubo.modelView)*inBitangent;
    vec3 N = mat3(ubo.modelView)*inNormal;
    tangentBasis = mat3(T, B, N);
}