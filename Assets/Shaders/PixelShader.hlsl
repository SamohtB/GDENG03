Texture2D myTexture : register(t0);
SamplerState mySampler : register(s0);

struct PSINPUT
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
    float4 color : COLOR;
};

float4 PSMain(PSINPUT input) : SV_TARGET
{
    float4 col = myTexture.Sample(mySampler, input.texcoord);
    
    return col;
}