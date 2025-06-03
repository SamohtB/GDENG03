// Physically Based Rendering
// Based ON Michal Siejak PBR
// https://github.com/Nadrin/PBR 

// Physically Based shading model: Lambetrtian diffuse BRDF + Cook-Torrance microfacet specular BRDF + IBL for ambient.

static const float PI = 3.141592;
static const float3 Fdielectric = 0.04; // Fresnel at normal incidence for dielectrics

Texture2D AlbedoTexture : register(t0);
Texture2D NormalTexture : register(t1);
Texture2D RoughnessTexture : register(t2);
Texture2D MetallicTexture : register(t3);

SamplerState Sampler : register(s0);

struct PSINPUT
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
    float3x3 TBN : TBN;
};

cbuffer SceneConstantBuffer : register(b0)
{
    float normalStr;
    float roughStr;
    float metalStr;
};

float4 PSMain(PSINPUT input) : SV_TARGET
{
    // === Samples ===
    float3 albedo = AlbedoTexture.Sample(Sampler, input.texcoord).rgb;
    float3 texNormal = NormalTexture.Sample(Sampler, input.texcoord).rgb;
    float texRough = RoughnessTexture.Sample(Sampler, input.texcoord).r;
    float texMetal = MetallicTexture.Sample(Sampler, input.texcoord).r;
    
    float roughness = saturate(texRough * roughStr);
    float metalness = saturate(texMetal * metalStr);

    // === Vectors ===
    float3 lightDir = normalize(float3(0.5, 2.0, 1.0)); // Static fake light direction
    float3 viewDir = normalize(float3(0.0, 0.0, 1.0)); // Assume camera is facing forward
    float3 halfDir = normalize(lightDir + viewDir);
    
    // === Normal Mapping ===
    texNormal = normalize(texNormal * 2.0 - 1.0);
    float3 N = normalize(mul(input.TBN, texNormal));
    N = normalize(lerp(float3(0, 0, 1), N, normalStr));

    // === Dot products for BRDF calculations ===
    float NdotL = max(dot(N, lightDir), 0.0);
    float NdotH = max(dot(N, halfDir), 0.0);

    // === Specular ===
    float shininess = lerp(4.0, 128.0, 1.0 - roughness); // sharper with less roughness
    float specStrength = lerp(0.04, 1.0, metalness); // dielectrics reflect less
    float3 F0 = lerp(float3(0.04, 0.04, 0.04), albedo, metalness);
    float3 specular = specStrength * F0 * pow(NdotH, shininess);
    
    // === Diffuse ===
    float3 kd = 1.0 - specStrength;
    float3 diffuse = kd * albedo * NdotL;

    // === Ambient Light ===
    float3 ambient = 0.1 * albedo;

    float3 finalColor = (ambient + diffuse + specular) * 2.0f;
    
    return float4(finalColor, 1.0);
}