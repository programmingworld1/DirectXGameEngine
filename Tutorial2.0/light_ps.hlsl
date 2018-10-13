////////////////////////////////////////////////////////////////////////////////
// Filename: light.ps
////////////////////////////////////////////////////////////////////////////////

/*The texture pixel shader has two global variables. The first is Texture2D shaderTexture 
which is the texture resource. This will be our texture resource that will be used for 
rendering the texture on the model. 

The second new variable is the SamplerState SampleType. 
The sampler state allows us to modify how the pixels are written to the polygon face when 
shaded. For example if the polygon is really far away and only makes up 8 pixels on the 
screen then we use the sample state to figure out which pixels or what combination of 
pixels will actually be drawn from the original texture. The original texture may be 
256 pixels by 256 pixels so deciding which pixels get drawn is really important to 
ensure that the texture still looks decent on the really small polygon face.	
We will setup the sampler state in the TextureShaderClass also and then attach 
it to the resource pointer so this pixel shader can use it to determine which sample of pixels to draw.*/

/////////////
// GLOBALS //
/////////////
Texture2D shaderTexture; // this is the texture resource
SamplerState SampleType;  

// We have two new global variables inside the LightBuffer that hold the diffuse color and the direction of the light.These two variables will be set from values in the new LightClass object.

cbuffer LightBuffer
{
	float4 diffuseColor;
	float3 lightDirection;
	float padding;
};

// The PixelInputType for the texture pixel shader is also modified using texture coordinates instead of the color values.
//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

/*The pixel shader has been modified so that it now uses the HLSL sample function. 
The sample function uses the sampler state we defined above and the texture coordinates 
for this pixel. It uses these two variables to determine and return the 
pixel value for this UV location on the polygon face.*/

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 LightPixelShader(PixelInputType input) : SV_TARGET
{
	float4 textureColor;
	float3 lightDir;
	float lightIntensity;
	float4 color;

	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
	textureColor = shaderTexture.Sample(SampleType, input.tex);

	/*This is where the lighting equation that was discussed earlier is now implemented.
	The light intensity value is calculated as the dot product between the normal vector of triangle and the light direction vector.*/

	// Invert the light direction for calculations.
	lightDir = -lightDirection;
	
	// Calculate the amount of light on this pixel.
	lightIntensity = saturate(dot(input.normal, lightDir));
	
	// And finally the diffuse value of the light is combined with the texture pixel value to produce the color result.

	// Determine the final amount of diffuse color based on the diffuse color combined with the light intensity.
	color = saturate(diffuseColor * lightIntensity);

	// Multiply the texture pixel and the final diffuse color to get the final pixel color result.
	color = color * textureColor;

	return color;
}