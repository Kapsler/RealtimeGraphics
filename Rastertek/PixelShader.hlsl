Texture2D shaderTextures[2] : register(t0);
Texture2D depthMapTexture : register (t2);
Texture2D depthMapTexture2 : register (t3);

SamplerState SampleTypeWrap;
SamplerState SampleTypeClamp;

cbuffer LightBuffer
{
    float4 ambientColor;
    float4 diffuseColor;
    float4 diffuseColor2;
}

struct PixelInputType
{
    float4 position : SV_Position;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float4 lightViewPosition : TEXCOORD2;
    float3 lightPos : TEXCOORD3;
    float4 lightViewPosition2 : TEXCOORD4;
    float3 lightPos2 : TEXCOORD5;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float bumpiness : BUMP;
    float4 color : COLOR;
};

float4 main(PixelInputType input) : SV_Target
{
    float bias;
    float4 color;
    float2 projectTexCoord;
    float depthValue;
    float lightDepthValue;
    float lightIntensity;
    float4 textureColor;
    float4 bumpMap;
    float3 bumpNormal;

    //Bias is set to fix floating point precision issues
    bias = 0.000001f;

    //Set default color to ambient
    color = ambientColor;

    //BUMPMAPPING
    //Sample pixel
    bumpMap = shaderTextures[1].Sample(SampleTypeWrap, input.tex);
    //Change range to -1.0 to 1.0 instead of 0 to 1
    bumpMap = (bumpMap * 2.0f) - 1.0f;
    //Calculate Normal
    bumpNormal = (bumpMap.x * input.tangent) + (bumpMap.y * input.binormal) + (bumpMap.z * input.normal);
    bumpNormal = normalize(bumpNormal * input.bumpiness + input.normal);
    
    //BUMPMAPPING

    ////Depth Buffer (Shadow Map)
    ////Calculate Projected texture coordinates
    //projectTexCoord.x = input.lightViewPosition.x / input.lightViewPosition.w / 2.0f + 0.5f;
    //projectTexCoord.y = -input.lightViewPosition.y / input.lightViewPosition.w / 2.0f + 0.5f;

    ////Check if projected coords are in view
    //if ((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y));
    //{
    //    //Depth Value of pixel in Shadow map - only red channel because greyscale
    //    depthValue = depthMapTexture.Sample(SampleTypeClamp, projectTexCoord).r;

    //    //Distance to light
    //    lightDepthValue = input.lightViewPosition.z / input.lightViewPosition.w;

    //    //Use bias
    //    lightDepthValue = lightDepthValue - bias;
        
    //    //Compare depths to shadow or light the pixel
    //    if (lightDepthValue < depthValue)
    //    {
    //        lightIntensity = saturate(dot(bumpNormal, input.lightPos));

    //        if (lightIntensity > 0.0f)
    //        {
    //            //Diffuse color
    //            color += (diffuseColor * lightIntensity);
    //        }
    //    }
    //}

    ////Second Light
    //projectTexCoord.x = input.lightViewPosition2.x / input.lightViewPosition2.w / 2.0f + 0.5f;
    //projectTexCoord.y = -input.lightViewPosition2.y / input.lightViewPosition2.w / 2.0f + 0.5f;

    //if ((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y))
    //{
    //    depthValue = depthMapTexture2.Sample(SampleTypeClamp, projectTexCoord).r;

    //    lightDepthValue = input.lightViewPosition2.z / input.lightViewPosition2.w;
    //    lightDepthValue = lightDepthValue - bias;

    //    if (lightDepthValue < depthValue)
    //    {
    //        lightIntensity = saturate(dot(bumpNormal, input.lightPos2));
    //        if (lightIntensity > 0.0f)
    //        {
    //            color += (diffuseColor2 * lightIntensity);
    //        }
    //    }
    //}
    
    //Saturate to final light color
    color = saturate(color);

    //Sample Texturet
    textureColor = shaderTextures[0].Sample(SampleTypeWrap, input.tex);

    //Calculate lighting with texture
    input.color = float4(1,1,1,1);
    color = color * input.color;
    color = color * textureColor;

    return color;
}