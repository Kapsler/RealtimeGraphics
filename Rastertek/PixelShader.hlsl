Texture2D shaderTexture;
SamplerState SampleType;

cbuffer LightBuffer
{
    float4 ambientColor;
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

    //Set default color to ambient
    color = ambientColor;

    //Invert Light
    lightDir = -lightDirection;

    //Calculate amount of light on the pixel
    lightIntensity = saturate(dot(input.normal, lightDir));

    //Dont allow negative diffuse color to be added
    if(lightIntensity > 0.0f){
        color += (diffuseColor * lightIntensity);
    }

    color = saturate(color);

    color = color * textureColor;

    return color;
}