//=============================================================================
//// Shader uses position and texture
//=============================================================================
SamplerState samPoint
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = Mirror;
    AddressV = Mirror;
};

float gThreshold = 0;
float gStrength = 1;

Texture2D gTexture;

/// Create Depth Stencil State (ENABLE DEPTH WRITING)
DepthStencilState depthStencilState
{
	DepthEnable = TRUE;
	DepthWriteMask = ALL;
};

/// Create Rasterizer State (Backface culling) 
RasterizerState rasterizerState
{
	FillMode = SOLID;
	CullMode = NONE;
};


//IN/OUT STRUCTS
//--------------
struct VS_INPUT
{
    float3 Position : POSITION;
	float2 TexCoord : TEXCOORD0;

};

struct PS_INPUT
{
    float4 Position : SV_POSITION;
	float2 TexCoord : TEXCOORD1;
};


//VERTEX SHADER
//-------------
PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	// Set the Position
	output.Position = float4(input.Position, 1.0);

	// Set the TexCoord
	output.TexCoord = input.TexCoord;

	return output;
}


//PIXEL SHADER
//------------
float4 PS(PS_INPUT input): SV_Target
{
	// Step 1: find the dimensions of the texture (the texture has a method for that)
	int mipLevel = 0;
	int width = 0;
	int height = 0;
	int numberOfLevels = 0;
	gTexture.GetDimensions(mipLevel, width, height, numberOfLevels);

	// Step 2: calculate dx and dy (UV space for 1 pixel)
	float dx = width * input.TexCoord.x;
	float dy = height * input.TexCoord.y;

	float4 color = gTexture.Sample(samPoint, input.TexCoord);

	// Step 3: convert rgb to grayscale/brightness
	float brightness = dot(color.rgb, float3(0.2126, 0.7152, 0.0722));

	float4 finalColor = float4(int(brightness > gThreshold) * float3(gStrength * color.rgb),1.0);

	// Step 5: return the final color
	return finalColor;
}


//TECHNIQUE
//---------
technique11 Blur
{
    pass P0
    {
		// Set states...
		SetRasterizerState(rasterizerState);
		SetDepthStencilState(depthStencilState, 0);

        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}