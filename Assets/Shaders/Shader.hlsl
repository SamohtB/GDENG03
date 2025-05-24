struct VSInput
{
    float3 position : POSITION;
    float2 texcoord : TEXCOORD;
    float4 color : COLOR;
};

struct PSInput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
    float4 color : COLOR;
};

Texture2D myTexture : register(t0);
SamplerState mySampler : register(s0);

PSInput VSMain(VSInput input)
{
    PSInput output;

    output.position = float4(input.position, 1.0f);;
    output.texcoord = input.texcoord;
    output.color = input.color;

    return output;
}

float4 PSMain(PSInput input) : SV_TARGET
{
    return input.color;
}