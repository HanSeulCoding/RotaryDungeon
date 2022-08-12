#include "Header.hlsli"

cbuffer HP : register(b11)
{
    float value;
}
struct PixelInput
{
    float4 pos : SV_Position;
    float2 uv : UV;
};

PixelInput VS(VertexUV input)
{
    PixelInput output;
    
    output.pos = mul(input.pos, world);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
    
    output.uv = input.uv;
    
    return output;
}
float4 PS(PixelInput input) : SV_Target
{
    if (input.uv.x > value)
        return float4(1, 1, 1, 0);
     
    
    return diffuseMap.Sample(samp, input.uv);
}