#include "Fullscreen.hlsli"

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

struct VignetteOption
{
    float scale;
    float power;
};

ConstantBuffer<VignetteOption> gOption : register(b0);

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    output.color = gTexture.Sample(gSampler, input.texcoord);
    
    // 周囲を0に、中心になるほど明るくなるようにする
    float2 correct = input.texcoord * (1.0f - input.texcoord.yx);

    // 中心の最大値が暗すぎるためScaleで調整する (cbv)
    float vignette = correct.x * correct.y * gOption.scale;
    
    // サチュレーションで超えないようにしながら、いい感じに
    vignette = saturate(pow(vignette, gOption.power));
    
    // 係数として乗算
    output.color.rgb *= vignette;
    output.color.a = 1.0f;

    return output;
}