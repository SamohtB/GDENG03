struct VSInput
{
    float3 position : POSITION;
    float2 texcoord : TEXCOORD;
    float3 color : COLOR;
};

struct VSOutput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
    float3 color : COLOR;
};

VSOutput VSMain(VSInput input)
{
    VSOutput output;

    output.position = float4(input.position, 1.0f);;
    output.texcoord = input.texcoord;
    output.color = input.color;

    return output;
}

