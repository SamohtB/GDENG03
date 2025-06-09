struct PSINPUT
{
    float4 position : SV_POSITION;
    float3 color1 : COLOR0;
    float3 color2 : COLOR1;
};

cbuffer SceneConstantBuffer : register(b0)
{
    float4x4 model;
    float time;
};

float4 PSMain(PSINPUT input) : SV_TARGET
{
    float3 lerpedCol = lerp(input.color1, input.color2, time);
    return float4(lerpedCol, 1.0f);
}