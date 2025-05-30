struct PSINPUT
{
    float4 position : SV_POSITION;
    float3 color1 : COLOR0;
    float3 color2 : COLOR1;
};

float4 PSMain(PSINPUT input) : SV_TARGET
{
    return float4(input.color1, 1.0f);
}