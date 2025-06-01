Texture2D myTexture : register(t0);
SamplerState mySampler : register(s0);

struct PSINPUT
{
    float4 position : SV_POSITION;
    float3 color : COLOR;
};

float4 PSMain(PSINPUT input) : SV_TARGET
{
    return float4(input.color, 1.0f);
}