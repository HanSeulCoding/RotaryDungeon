#include "Header.hlsli"

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
    float4 color = diffuseMap.Sample(samp, input.uv);
    
    float value = (color.r + color.g + color.b) / 3;
    
    return float4(value.xxx, 1);
}