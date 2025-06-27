struct VSInput
{
    float3 position : POSITION;
    float2 texcoord : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
};

struct VSOutput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
    float3x3 TBN : TBN;
    float3 positionWS : POSITION1;
};

cbuffer ObjectConstants : register(b0)
{
    float4x4 model;
}

cbuffer FrameConstants : register(b1)
{
    float4x4 view;
    float4x4 projection;
}

VSOutput VSMain(VSInput input)
{
    VSOutput output;
    
    float4 worldPos = mul(float4(input.position, 1.0), model);
    float4 viewPos = mul(worldPos, view);
    float4 projPos = mul(viewPos, projection);
    output.position = projPos;
    output.texcoord = input.texcoord;
    output.positionWS = worldPos.xyz;
    
    float3 normal = normalize(mul(input.normal, (float3x3) model));
    float3 tangent = normalize(mul(input.tangent, (float3x3) model));
    float3 bitangent = normalize(cross(normal, tangent));
    
    output.TBN = float3x3(tangent, bitangent, normal);

    return output;
}
