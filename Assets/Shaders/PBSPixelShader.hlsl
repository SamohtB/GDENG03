Texture2D Textures[] : register(t0, space0);
SamplerState Samplers[] : register(s0, space0);

struct PSINPUT
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
    float3x3 TBN : TBN;
    float3 positionWS : POSITION1;
};

cbuffer MaterialBuffer : register(b0)
{
    uint albedoTextureIndex;
    uint normalTextureIndex;
    float normalStr;
    uint metallicTextureIndex;
    float metallicStr;
    uint roughTextureIndex;
    float roughStr;
    uint ambientOcclussionTextureIndex;
    float ambientOcclussionStr;
    uint materialFlags;
}

static const uint HasAlbedoMap = 1 << 0;
static const uint HasNormalMap = 1 << 1;
static const uint HasMetallicMap = 1 << 2;
static const uint HasRoughnessMap = 1 << 3;
static const uint HasAOMap = 1 << 4;

struct SampledTextureMaps
{
    float3 albedo;
    float3 normal;
    float3 MRAO;
};

static const float3 defaultLightPositions[1] =
{
    float3(-5.0, 5.0, 5.0),
};

static const float3 defaultLightColors[1] =
{
    float3(200.0, 200.0, 200.0),
};

static const float3 defaultCamPos = float3(0.0, 0.0, -1.0);
static const float PI = 3.14159265359;

float DistributionGGX(float3 N, float3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;
	
    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float num = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}

float GeometrySmith(float3 N, float3 V, float3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}

float3 fresnelSchlick(float cosTheta, float3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

SampledTextureMaps SampleTextures(PSINPUT input)
{
    // === Default Values ===
    float3 d_albedo = float3(1, 1, 1);
    float3 d_normal = float3(0, 0, 1); 
    float d_metallic = 0.0f;
    float d_roughness = 1.0f;
    float d_ao = 1.0f;
    
    SampledTextureMaps samples;
    
    // === Check Flags ===
    if ((materialFlags & HasAlbedoMap) != 0)
    {
        samples.albedo = pow(Textures[albedoTextureIndex].Sample(Samplers[0], input.texcoord).rgb, 2.2);
    }
    else
    {
        samples.albedo = d_albedo;
    }
    
    if ((materialFlags & HasNormalMap) != 0)
    {
        float3 normal = Textures[normalTextureIndex].Sample(Samplers[0], input.texcoord).rgb;
        samples.normal = lerp(d_normal, normal, normalStr);
    }
    else
    {
        samples.normal = d_normal;
    }
    
    if ((materialFlags & HasMetallicMap) != 0)
    {
        float metal = Textures[metallicTextureIndex].Sample(Samplers[0], input.texcoord).r;
        samples.MRAO.r = lerp(d_metallic, metal, metallicStr);
    }
    else
    {
        samples.MRAO.r = d_metallic;
    }
    
    if ((materialFlags & HasRoughnessMap) != 0)
    {
        float rough = Textures[roughTextureIndex].Sample(Samplers[0], input.texcoord).r;
        samples.MRAO.g = lerp(d_roughness, rough, roughStr);
    }
    else
    {
        samples.MRAO.g = d_roughness;
    }
    
    if ((materialFlags & HasAOMap) != 0)
    {
        float ao = Textures[ambientOcclussionTextureIndex].Sample(Samplers[0], input.texcoord).r;
        samples.MRAO.b = lerp(d_ao, ao, ambientOcclussionStr);
    }
    
    else
    {
        samples.MRAO.b = d_ao;
    }
    
    return samples;
}

float4 PSMain(PSINPUT input) : SV_TARGET
{
    SampledTextureMaps samples = SampleTextures(input);
    
    float3 N = normalize(mul(input.TBN, normalize(samples.normal * 2.0 - 1.0)));
    float3 V = normalize(defaultCamPos - input.positionWS);
    float3 F0 = lerp(float3(0.04, 0.04, 0.04), samples.albedo, samples.MRAO.r);
    float3 Lo = float3(0.0, 0.0, 0.0);
    
    // === Direct Lighting ===
    for (int i = 0; i < 4; ++i)
    {
        float3 L = normalize(defaultLightPositions[i] - input.positionWS);
        float3 H = normalize(V + L);
        float distance = length(defaultLightPositions[i] - input.positionWS);
        float attenuation = 1.0 / (distance * distance);
        float3 radiance = defaultLightColors[i] * attenuation;

        float NDF = DistributionGGX(N, H, samples.MRAO.g);
        float G = GeometrySmith(N, V, L, samples.MRAO.g);
        float3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

        float3 kS = F;
        float3 kD = (1.0 - kS) * (1.0 - samples.MRAO.r);

        float3 numerator = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
        float3 specular = numerator / denominator;

        float NdotL = max(dot(N, L), 0.0);
        Lo += (kD * samples.albedo / PI + specular) * radiance * NdotL;
    }

    float3 ambient = float3(0.03, 0.03, 0.03) * samples.albedo * samples.MRAO.b;
    float3 color = ambient + Lo;

    color = color / (color + 1.0);
    color = pow(color, 1.0 / 2.2);
    
    return float4(color, 1.0);
}