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

VSOutput VSMain(VSInput input)
{
    VSOutput output;

    output.position = float4(input.position1, 1.0f);
    output.color1 = input.color1;
    output.color2 = input.color2;

    return output;
}

