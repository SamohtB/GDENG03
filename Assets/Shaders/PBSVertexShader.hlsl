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
    float3x3 TBN : TBN;
};

VSOutput VSMain(VSInput input)
{
    VSOutput output;

    output.position = float4(input.position, 1.0f);
    output.texcoord = float2(input.texcoord.x, 1.0 - input.texcoord.y);
    
    float3x3 TBN = float3x3(input.tangent, input.bitangent, input.normal);
    output.TBN = transpose(TBN);
    
    return output;
}
