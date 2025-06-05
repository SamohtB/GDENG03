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
    float3 positionWS : POSITION1;
};

static float4x4 world =
{
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
};

static float4x4 view =
{
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 1, 1
};

static float4x4 projection =
{
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 1,
    0, 0, -1, 0
};


VSOutput VSMain(VSInput input)
{
    VSOutput output;

    float4 worldPos = mul(float4(input.position, 1.0), world);
    float4 viewPos = mul(worldPos, view);
    float4 projPos = mul(viewPos, projection);
    output.position = projPos;

    output.texcoord = input.texcoord;
    output.positionWS = worldPos.xyz;
    
    float3 worldNormal = normalize(mul(input.normal, (float3x3) world));
    float3 worldTangent = normalize(mul(input.tangent, (float3x3) world));
    float3 worldBitangent = normalize(mul(input.bitangent, (float3x3) world));
    
    output.TBN = float3x3(worldTangent, worldBitangent, worldNormal);

    return output;
}
