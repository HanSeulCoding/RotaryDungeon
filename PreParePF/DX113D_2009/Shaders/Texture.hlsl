#include "Header.hlsli"

struct PixelInput
{
    float4 pos : SV_Position;
    float2 uv : UV;
   // float4
};

cbuffer BlackWhiteBuffer : register(b13)
{
    int hasBlack;
}

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

    if (hasBlack)
        return diffuseMap.Sample(samp, input.uv) * mDiffuse * float4(0.3, 0.3, 0.3, 1);
    
    return diffuseMap.Sample(samp, input.uv) * mDiffuse;
}