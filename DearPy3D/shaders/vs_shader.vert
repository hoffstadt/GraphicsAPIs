#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(push_constant) uniform Transforms {
    mat4 model;
    mat4 modelView;
    mat4 modelViewProj;
} PushConstants;

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

layout(set = 0, binding = 1) uniform mvPhongMaterial
{
    vec3 materialColor;
    //-------------------------- ( 16 bytes )
    
    vec3 specularColor;
    float specularGloss;
    //-------------------------- ( 16 bytes )
   
    
    float normalMapWeight;
    bool useTextureMap;
    bool useNormalMap;
    bool useSpecularMap;
    //-------------------------- ( 16 bytes )
    
    
    bool useGlossAlpha;
    bool hasAlpha;
    //-------------------------- ( 16 bytes )
    //-------------------------- ( 4 * 16 = 64 bytes )
};

void main() 
{
    gl_Position = PushConstants.modelViewProj * vec4(inPosition, 1.0);
    viewPos = vec3(PushConstants.modelView * vec4(inPosition, 1.0));
    viewNormal = mat3(PushConstants.modelView) * inNormal;
    worldNormal = mat3(PushConstants.model) * inNormal;
    texCoord = inTexCoord;
    vec3 T = mat3(PushConstants.modelView)*inTangent;
    vec3 B = mat3(PushConstants.modelView)*inBitangent;
    vec3 N = mat3(PushConstants.modelView)*inNormal;
    tangentBasis = mat3(T, B, N);
}