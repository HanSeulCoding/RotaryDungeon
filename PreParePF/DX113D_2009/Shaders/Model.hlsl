#include "Header.hlsli"

struct PixelInput
{
    float4 pos : SV_Position;
    float2 uv : UV;
    float3 normal : Normal;
    float3 tangent : Tangnet;
    float3 binormal : Binormal;
    float3 viewDir : ViewDir;
};

PixelInput VS(VertexUVNormalTangentBlend input)
{
    PixelInput output;
    
    matrix transform = mul(BoneWorld(input.indices, input.weights), world);
    
    output.pos = mul(input.pos, transform);
    
    output.viewDir = normalize(output.pos.xyz - viewPos);
    
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
        
    output.normal = mul(input.normal, (float3x3) transform);
    output.tangent = mul(input.tangent, (float3x3) transform);
    output.binormal = cross(output.normal, output.tangent);
    
    output.uv = input.uv;
    
    return output;
}

float4 PS(PixelInput input) : SV_Target
{
    float4 albedo = float4(1, 1, 1, 1);
    
    if (hasDiffuseMap)
        albedo = diffuseMap.Sample(samp, input.uv);
    
    float3 light = normalize(lights[0].direction);
    
    float3 T = normalize(input.tangent);
    float3 B = normalize(input.binormal);
    float3 N = normalize(input.normal);
    
    float3 normal = N;
    
    if (hasNormalMap)
    {
        float4 normalMapping = normalMap.Sample(samp, input.uv);
    
        float3x3 TBN = float3x3(T, B, N);
        normal = normalMapping * 2.0f - 1.0f; //0~1 -> -1~1
        normal = normalize(mul(normal, TBN));
    }
    
    float diffuseIntensity = saturate(dot(normal, -light));
    
    float4 specular = 0;
    if (diffuseIntensity > 0)
    {
        float3 reflection = normalize(reflect(light, normal));
        specular = saturate(dot(-reflection, input.viewDir));
        
        float4 specualrIntensity = float4(1, 1, 1, 1);
        if (hasSpecualrMap)
            specualrIntensity = specualrMap.Sample(samp, input.uv);
        
        specular = pow(specular, shininess) * specualrIntensity * mSpecular;
    }
    
    float4 diffuse = albedo * diffuseIntensity * mDiffuse;
    float4 ambient = albedo * mAmbient;
    
    return diffuse + specular + ambient;
}