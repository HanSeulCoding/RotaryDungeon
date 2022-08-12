//TerrainSplatting
#include "Header.hlsli"

cbuffer Brush : register(b10)
{
    int type;
    float3 location;
    
    float range;
    float3 color;
}

struct PixelInput
{
    float4 pos : SV_Position;
    float2 uv : UV;
    float3 normal : Normal;
    float3 tangent : Tangnet;
    float3 binormal : Binormal;
    float3 viewDir : ViewDir;
    float3 worldPos : Position;
    float4 alpha : Alpha;
};

PixelInput VS(VertexUVNormalTangentAlpha input)
{
    PixelInput output;
    
    output.pos = mul(input.pos, world);
    
    output.viewDir = normalize(output.pos.xyz - viewPos);
    output.worldPos = output.pos;
    
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
        
    output.normal = mul(input.normal, (float3x3) world);
    output.tangent = mul(input.tangent, (float3x3) world);
    output.binormal = cross(output.normal, output.tangent);
    
    output.uv = input.uv;
    
    output.alpha = input.alpha;
    
    return output;
}

float3 BrushColor(float3 pos)
{
    if (type == 0)
    {
        float x = pos.x - location.x;
        float z = pos.z - location.z;
        
        float distance = sqrt(x * x + z * z);
        
        if (distance <= range)
            return color;
    }
    else if (type == 1)
    {
        float size = range * 0.5f;
        
        if (abs(pos.x - location.x) < size && abs(pos.z - location.z) < size)
        {
            return color;
        }
    }
    
    return float3(0, 0, 0);
}

//Texture2D alphaMap : register(t10);
Texture2D secondMap : register(t11);
Texture2D thirdMap : register(t12);

float4 PS(PixelInput input) : SV_Target
{
    float4 albedo = float4(1, 1, 1, 1);
    
    if (hasDiffuseMap)
        albedo = diffuseMap.Sample(samp, input.uv);
    
    //float4 alpha = alphaMap.Sample(samp, input.uv);
    float4 second = secondMap.Sample(samp, input.uv);
    float4 third = thirdMap.Sample(samp, input.uv);
    
    albedo = lerp(albedo, second, input.alpha.r);
    albedo = lerp(albedo, third, input.alpha.g);
    
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
    
    float4 brushColor = float4(BrushColor(input.worldPos), 1.0f);
    
    return diffuse + specular + ambient + brushColor;
}