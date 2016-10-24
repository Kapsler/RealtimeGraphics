Texture2D shaderTexture;
SamplerState SampleType;

cbuffer LightBuffer
{
    float4 diffuseColor;
    float3 lightDirection;
    float padding;
}

struct PixelInputType
{
    float4 position : SV_Position;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

float4 main(PixelInputType input) : SV_Target
{
    float4 textureColor;
    float3 lightDir;
    float lightIntensity;
    float4 color;

    //Sample Texture
    textureColor = shaderTexture.Sample(SampleType, input.tex);

    //Invert Light
    lightDir = -lightDirection;

    lightIntensity = saturate(dot(input.normal, lightDir));

    color = saturate(diffuseColor * lightIntensity);

    color = color * textureColor;

    return color;
}