struct VSInput
{
    float3 position : POSITION;
    float2 texcoord : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 bitangent : BITANGENT;
};

struct VSOutput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
    float3 T : TANGENT;
    float3 B : BITANGENT;
    float3 N : NORMAL;
};

VSOutput VSMain(VSInput input)
{
    VSOutput output;

    output.position = float4(input.position, 1.0f);
    output.texcoord = input.texcoord;
    output.T = input.tangent;
    output.B = input.bitangent;
    output.N = input.normal;

    return output;
}
