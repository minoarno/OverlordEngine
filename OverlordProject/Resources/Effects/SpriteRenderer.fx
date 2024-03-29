float4x4 gTransform : WorldViewProjection;
Texture2D gSpriteTexture;
float2 gTextureSize = float2(1,1);

SamplerState samPoint
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = WRAP;
    AddressV = WRAP;
};

BlendState EnableBlending
{
    BlendEnable[0] = TRUE;
    SrcBlend = SRC_ALPHA;
    DestBlend = INV_SRC_ALPHA;
};

DepthStencilState NoDepth
{
    DepthEnable = FALSE;
};

RasterizerState BackCulling
{
    CullMode = BACK;
};

//SHADER STRUCTS
//**************
struct VS_DATA
{
    uint TextureId : TEXCOORD0;
    float4 TransformData : POSITION; //PosX, PosY, Depth (PosZ), Rotation
    float4 TransformData2 : POSITION1; //PivotX, PivotY, ScaleX, ScaleY
    float4 Color : COLOR;
};

struct GS_DATA
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR;
    float2 TexCoord : TEXCOORD0;
};

//VERTEX SHADER
//*************
VS_DATA MainVS(VS_DATA input)
{
    return input;
}

//GEOMETRY SHADER
//***************
void CreateVertex(inout TriangleStream<GS_DATA> triStream, float3 pos, float4 col, float2 texCoord, float rotation, float2 rotCosSin, float2 offset, float2 pivotOffset)
{
    if (rotation != 0)
    {
		//Step 3.
		//Do rotation calculations
		//Transform to origin
		//Rotate
		//Retransform to initial position
		pos.x = pos.x - pivotOffset.x - offset.x;
		pos.y = pos.y - pivotOffset.y - offset.y;
		float x = pos.x;
		float y = pos.y;
		pos.x = x * rotCosSin.x - y * rotCosSin.y;
		pos.y = y * rotCosSin.x + x * rotCosSin.y;
		pos.x = pos.x + offset.x;
		pos.y = pos.y + offset.y;
    }
    else
    {
		//Step 2.
		//No rotation calculations (no need to do the rotation calculations if there is no rotation applied > redundant operations)
		//Just apply the pivot offset
		pos.x = pos.x - pivotOffset.x;
		pos.y = pos.y - pivotOffset.y;
    }

	//Geometry Vertex Output
    GS_DATA geomData = (GS_DATA) 0;
    geomData.Position = mul(float4(pos, 1.0f), gTransform);
    geomData.Color = col;
    geomData.TexCoord = texCoord;
    triStream.Append(geomData);
}

[maxvertexcount(4)]
void MainGS(point VS_DATA vertex[1], inout TriangleStream<GS_DATA> triStream)
{
	//Given Data (Vertex Data)
    float3 position = float3(vertex[0].TransformData.x, vertex[0].TransformData.y, vertex[0].TransformData.z);		//Extract the position data from the VS_DATA vertex struct
    float2 offset = float2(vertex[0].TransformData.x, vertex[0].TransformData.y); 									//Extract the offset data from the VS_DATA vertex struct (initial X and Y position)
    float rotation = vertex[0].TransformData.w; 									//Extract the rotation data from the VS_DATA vertex struct
	
    float2 pivot = float2(vertex[0].TransformData2.x, vertex[0].TransformData2.y); 	//Extract the pivot data from the VS_DATA vertex struct	
    float2 scale = float2(vertex[0].TransformData2.z, vertex[0].TransformData2.w); 	//Extract the scale data from the VS_DATA vertex struct
	//Initial Texture Coordinate
	
	float2 rot;
	if(rotation == 0)
	{
		rot = float2(0,0);
	}
	else
	{
		rot = float2(cos(rotation),sin(rotation));
	}

	// LT----------RT //TringleStrip (LT > RT > LB, LB > RB > RT)
	// |          / |
	// |       /    |
	// |    /       |
	// | /          |
	// LB----------RB

	float4 white = float4(1,1,1,1);
	float2 pivotOffset = float2(pivot.x * scale.x * gTextureSize.x, pivot.y * scale.y * gTextureSize.y);
	
	//VERTEX 1 [LT]
   	CreateVertex(triStream, position, white, float2(0, 0), rotation, rot, offset, pivotOffset); //Change the color data too!
	
	//VERTEX 2 [RT]
    CreateVertex(triStream, float3(position.x + gTextureSize.x * scale.x, position.y, position.z), white, float2(1, 0), rotation, rot, offset, pivotOffset); //Change the color data too!

	//VERTEX 3 [LB]
    CreateVertex(triStream, float3(position.x, position.y + gTextureSize.y * scale.y, position.z), white, float2(0, 1), rotation, rot, offset, pivotOffset); //Change the color data too!

	//VERTEX 4 [RB]
   	CreateVertex(triStream, float3(position.x + gTextureSize.x * scale.x, position.y + gTextureSize.y * scale.y, position.z), white, float2(1, 1), rotation, rot, offset, pivotOffset); //Change the color data too!
}

//PIXEL SHADER
//************
float4 MainPS(GS_DATA input) : SV_TARGET
{
    return gSpriteTexture.Sample(samPoint, input.TexCoord) * input.Color;
}

// Default Technique
technique10 Default
{
    pass p0
    {
        SetRasterizerState(BackCulling);
        SetBlendState(EnableBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		//SetDepthStencilState(NoDepth,0);
        SetVertexShader(CompileShader(vs_4_0, MainVS()));
        SetGeometryShader(CompileShader(gs_4_0, MainGS()));
        SetPixelShader(CompileShader(ps_4_0, MainPS()));
    }
}
