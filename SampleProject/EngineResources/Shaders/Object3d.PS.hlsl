#include "object3d.hlsli"


struct Material
{
    float4 color;
    int lightingType;
    int enableLighting;
    float4x4 uvTransform;
};

struct DirectionalLight
{
    float4 color; //!< ライトの色
    float3 direction; //!< ライトの向き(正規化必須)
    float intensity; //!< 輝度
};

struct UVTiling
{
    float2 tilingMultiply; //!< UVのタイリング係数
};

ConstantBuffer<Material> gMaterial : register(b0);
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);
ConstantBuffer<UVTiling> gUVTiling : register(b2);

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;

    float4 transformedUV = mul(float4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);

    transformedUV.xy *= gUVTiling.tilingMultiply;

    float4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);


    if (gMaterial.enableLighting != 0)
    {
        float cos;
        if (gMaterial.lightingType == 0)
        {
            cos = saturate(dot(normalize(input.normal), -gDirectionalLight.direction));
        }
        else if (gMaterial.lightingType == 1)
        {
            // half lambert
            float NdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
            cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
        }

        output.color.rgb = gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
        output.color.a = gMaterial.color.a * textureColor.a;
    }
    else
    {
        output.color = gMaterial.color * textureColor;
    }

    if (textureColor.a <= 0.f || output.color.a <= 0.f)
    {
        discard;
    }

    return output;
}