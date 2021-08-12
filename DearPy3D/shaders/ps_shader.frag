#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 inViewPos;
layout(location = 1) in vec3 inViewNormal;
layout(location = 2) in vec3 inWorldNormal;
layout(location = 3) in vec2 inTexCoord;
layout(location = 4) in mat3 inTangentBasis;

layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 0) uniform sampler2D texSampler;

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

float Attenuate(float attConst, float attLin, float attQuad, in float distFragToL)
{
    return 1.0 / (attConst + attLin * distFragToL + attQuad * (distFragToL * distFragToL));
}

void main() 
{
//    vec3 diffuse = { 0.0, 0.0, 0.0 };
//    vec3 specularReflected = { 0.0, 0.0, 0.0 };
//    vec3 specularReflectedColor = { 1.0, 1.0, 1.0 };
//    vec4 materialColor = texture(texSampler, inTexCoord);
//    vec3 viewNormal = inViewNormal;
//
//    // flip normal when backface
//    if (dot(viewNormal, inViewPos) >= 0.0f)
//    {
//        viewNormal = -inViewNormal;
//    }
//
//    // normalize the mesh normal
//    viewNormal = normalize(viewNormal);
//
//    // specular parameter determination (mapped or uniform)
//    float specularPowerLoaded = 0.0;
//
//    // fragment to light vector data
//    vec3 lightVec = pointlight.viewLightPos - inViewPos;
//    float lightDistFromFrag = length(lightVec);
//    vec3 lightDirVec = lightVec / lightDistFromFrag;
//    
//	// attenuation
//    const float att = Attenuate(pointlight.attConst, pointlight.attLin, pointlight.attQuad, lightDistFromFrag);
//    
//	// diffuse
//    diffuse += pointlight.diffuseColor * pointlight.diffuseIntensity * att * max(0.0, dot(lightDirVec, viewNormal));
//    
//    // specular
//        
//    // calculate reflected light vector
//    const vec3 w = viewNormal * dot(lightVec, viewNormal);
//    const vec3 r = normalize(w * 2.0f - lightVec);
//        
//    // vector from camera to fragment
//    const vec3 viewCamToFrag = normalize(inViewPos);
//        
//    specularReflected += att * pointlight.diffuseColor * pointlight.diffuseIntensity * specularReflectedColor * 1.0 * pow(max(0.0, dot(-r, viewCamToFrag)), specularPowerLoaded);
//         
//    outColor = clamp((vec4(diffuse, 1.0) + vec4(0.04, 0.04, 0.04, 1.0)) * materialColor + vec4(specularReflected, 1.0), 0.0, 1.0);   
//
      //outColor = texture(texSampler, inTexCoord);
      outColor = vec4(materialColor, 1.0);
}