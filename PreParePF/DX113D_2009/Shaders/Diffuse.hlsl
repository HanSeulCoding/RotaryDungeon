#include "Header.hlsli"

struct PixelInput
{
    float4 pos : SV_Position;
    float2 uv : UV;
    float3 normal : Normal;
};

PixelInput VS(VertexUVNormal input)
{
    PixelInput output;
    
    output.pos = mul(input.pos, world);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
        
    output.normal = normalize(mul(input.normal, (float3x3) world));
    output.uv = input.uv;
    
    return output;
}

float4 PS(PixelInput input) : SV_Target
{
    float3 light = normalize(lightDirection);
    float diffuse = dot(input.normal, -light);
    
    return diffuseMap.Sample(samp, input.uv) * diffuse;
}