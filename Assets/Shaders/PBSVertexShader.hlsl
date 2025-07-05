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
    float3 normal : NORMAL;
};

cbuffer ObjectConstants : register(b0)
{
    float4x4 model;
    int id;
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

    output.texcoord = float2(input.texcoord.x, -input.texcoord.y);
    output.positionWS = worldPos.xyz;
    
    float3 worldNormal = normalize(mul(input.normal, (float3x3) model));
    float3 worldTangent = normalize(mul(input.tangent, (float3x3) model));
    float3 worldBitangent = normalize(cross(worldNormal, worldTangent));
    
    output.TBN = float3x3(worldTangent, worldBitangent, worldNormal);
    output.normal = normalize(mul(input.normal, (float3x3) model));

    return output;
}