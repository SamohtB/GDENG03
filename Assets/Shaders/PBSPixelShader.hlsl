// Physically Based Rendering
// Based ON Michal Siejak PBR
// https://github.com/Nadrin/PBR 

// Physically Based shading model: Lambetrtian diffuse BRDF + Cook-Torrance microfacet specular BRDF + IBL for ambient.

Texture2D AlbedoTexture : register(t0);
Texture2D NormalTexture : register(t1);
Texture2D RoughnessTexture : register(t2);

SamplerState Sampler : register(s0);

struct PSINPUT
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
    float3x3 TBN : TBN;
};

cbuffer MaterialBuffer : register(b0)
{
    float normalStr;
    float roughStr;
    
}

float4 PSMain(PSINPUT input) : SV_TARGET
{
    // Sample albedo (base color)
    float3 albedo = AlbedoTexture.Sample(Sampler, input.texcoord).rgb;

    // === Lighting Vectors ===
    float3 lightDir = normalize(float3(0.5, 2.0, 1.0)); // Static fake light direction
    float3 viewDir = normalize(float3(0.0, 0.0, 1.0)); // Assume camera is facing forward
    
    // === Normal Mapping ===
    float3 normalSample = NormalTexture.Sample(Sampler, input.texcoord).rgb;
    float3 mappedNormal = normalize(2.0 * normalSample - 1.0);
    float3 flatNormal = float3(0.0, 0.0, 1.0);
    float3 finalNormalTangent = normalize(lerp(flatNormal, mappedNormal, normalStr));
    float3 N = normalize(mul(input.TBN, finalNormalTangent));
    
    // === Roughness ===
    float roughnessSample = RoughnessTexture.Sample(Sampler, input.texcoord).r;
    float baseRoughness = 0.5;
    float roughness = lerp(baseRoughness, roughnessSample, roughStr);
    roughness = saturate(roughness);

    // === Diffuse ===
    float diff = saturate(dot(N, lightDir));
    float3 diffuse = albedo * diff;
    
    // === Specular === (Fake using Blinn-Phong model modulated by roughness)
    float3 halfway = normalize(lightDir + viewDir);
    float specAngle = max(dot(N, halfway), 0.0);
    float shininess = lerp(2.0, 256.0, 1.0 - roughness);
    float specularIntensity = pow(specAngle, shininess);
    float3 specularColor = float3(1.0, 1.0, 1.0) * specularIntensity;
    
    // === Ambient ===
    float3 ambient = albedo * 0.2;
    
    float3 finalColor = diffuse + specularColor + ambient;

    return float4(finalColor, 1.0f);
}