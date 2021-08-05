#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 viewPos;
layout(location = 1) in vec3 viewNormal;
layout(location = 2) in vec3 worldNormal;
layout(location = 3) in vec2 texCoord;
layout(location = 4) in mat3 tangentBasis;

layout(location = 0) out vec4 outColor;

layout(binding = 1) uniform sampler2D texSampler;

void main() 
{
    vec4 color = texture(texSampler, texCoord);
    if (color.w < 1) { discard; }
    outColor = color;
}