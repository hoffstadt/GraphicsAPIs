#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 viewPos;
layout(location = 1) in vec3 viewNormal;
layout(location = 2) in vec3 worldNormal;
layout(location = 3) in vec2 texCoord;
layout(location = 4) in mat3 tangentBasis;

layout(location = 0) out vec4 outColor;

layout(binding = 1) uniform sampler2D texSampler;

//layout(binding = 2) uniform mvPointLight 
//{
//    vec3 viewLightPos;
//    //-------------------------- ( 16 bytes )
//    
//    vec3 diffuseColor;
//    float diffuseIntensity;
//    //-------------------------- ( 16 bytes )
//    
//    float attConst;
//    float attLin;
//    float attQuad;
//    //-------------------------- ( 16 bytes )
//    
//    //-------------------------- ( 4*16 = 64 bytes )
//} pointlight;

void main() 
{
    outColor = vec4(1.0, 1.0, 1.0, 1.0);
}