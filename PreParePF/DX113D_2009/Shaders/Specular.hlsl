#include "Header.hlsli"

struct PixelInput
{
    float4 pos : SV_Position;
    float2 uv : UV;
    float3 normal : Normal;
    float3 viewDir : ViewDir;
};

PixelInput VS(VertexUVNormal input)
{
    PixelInput output;
    
    output.pos = mul(input.pos, world);
    
    output.viewDir = normalize(output.pos.xyz - viewPos);
    
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
        
    output.normal = normalize(mul(input.normal, (float3x3) world));
    output.uv = input.uv;
    
    return output;
}

float4 PS(PixelInput input) : SV_Target
{
    float4 albedo = diffuseMap.Sample(samp, input.uv);
    
    float3 light = normalize(lightDirection);        
    
    float diffuseIntensity = saturate(dot(input.normal, -light));
    
    float4 specular = 0;
    if(diffuseIntensity > 0)
    {
        float3 reflection = normalize(reflect(light, input.normal));
        specular = saturate(dot(-reflection, input.viewDir));
        
        float4 specualrIntensity = specualrMap.Sample(samp, input.uv);
        
        specular = pow(specular, shininess) * specualrIntensity;
    }
    
    float4 diffuse = albedo * diffuseIntensity;
    
    return diffuse + specular;
}