struct VSInput
{
    float3 position1 : POSITION0;
    float3 position2 : POSITION1;
    float3 color1 : COLOR0;
    float3 color2 : COLOR1;
};

struct VSOutput
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

VSOutput VSMain(VSInput input)
{
    VSOutput output;
    
    float3 lerpedPos = lerp(input.position1, input.position2, time);
    output.position = float4(lerpedPos, 1.0f);
    output.color1 = input.color1;
    output.color2 = input.color2;

    return output;
}

