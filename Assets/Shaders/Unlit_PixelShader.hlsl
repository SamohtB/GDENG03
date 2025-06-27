Texture2D Textures[] : register(t0, space0);
SamplerState Samplers[] : register(s0, space0);

struct PSINPUT
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
    float3x3 TBN : TBN;
    float3 positionWS : POSITION1;
};

cbuffer MaterialBuffer : register(b2)
{
    uint albedoTextureIndex;
    float3 albedoColor;
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

struct SampledTextureMaps
{
    float3 albedo;
};

SampledTextureMaps SampleTextures(PSINPUT input)
{
    SampledTextureMaps samples;

    if ((materialFlags & HasAlbedoMap) != 0)
    {
        samples.albedo = pow(Textures[albedoTextureIndex].Sample(Samplers[0], input.texcoord).rgb, 2.2) * albedoColor;
    }
    else
    {
        samples.albedo = albedoColor;
    }

    return samples;
}

float4 PSMain(PSINPUT input) : SV_TARGET
{
    SampledTextureMaps samples = SampleTextures(input);

    float3 color = samples.albedo;
    color = color / (color + 1.0);
    color = pow(color, 1.0 / 2.2);

    return float4(color, 1.0);
}