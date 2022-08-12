#include "Header.hlsli"

struct PixelInput
{
    float4 pos : SV_Position;
    float3 originPos : Position;
};

PixelInput VS(Vertex input)
{
    PixelInput output;
    
    output.pos.xyz = mul(input.pos.xyz, (float3x3) view);
    output.pos.w = 1.0f;
    
    output.pos = mul(output.pos, projection);
    
    output.originPos = input.pos.xyz;
    
    return output;
}

TextureCube cubeMap : register(t10);

float4 PS(PixelInput input) : SV_Target
{
    return cubeMap.Sample(samp, input.originPos);
}