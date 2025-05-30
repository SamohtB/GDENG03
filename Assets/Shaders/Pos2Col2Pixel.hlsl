struct PSINPUT
{
    float4 position : SV_POSITION;
    float3 color1 : COLOR0;
    float3 color2 : COLOR1;
};

cbuffer SceneConstantBuffer : register(b0)
{
    float time;
};

float4 PSMain(PSINPUT input) : SV_TARGET
{
    float3 lerpedCol = lerp(input.color1, input.color2, (sin(time) + 1.0f) / 2.0f);
    return float4(lerpedCol, 1.0f);
}