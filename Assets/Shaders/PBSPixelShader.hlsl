Texture2D DiffuseMap : register(t0);
Texture2D NormalMap : register(t1);
SamplerState Sampler : register(s0);

struct PSINPUT
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
    float3 T : TANGENT;
    float3 B : BITANGENT;
    float3 N : NORMAL;
};

float4 PSMain(PSINPUT input) : SV_TARGET
{
    float3 normalMap = NormalMap.Sample(Sampler, input.texcoord).rgb;
    normalMap = normalize(normalMap * 2.0 - 1.0);
    
    float3x3 TBN = float3x3(input.T, input.B, input.N);
    float3 normal = normalize(mul(normalMap, TBN));
    
    float3 albedo = DiffuseMap.Sample(Sampler, input.texcoord).rgb;
    
    float3 lightDir = normalize(float3(0.5f, -1.0f, -0.5f));
    float diff = max(dot(normal, -lightDir), 0.0f);

    return float4(albedo * diff, 1.0f);
    
}