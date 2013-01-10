//**************************************************************************//
// Shader file for the coursework skeleton.  It is very heavily	based on the//
// shader provided in Microsoft's "SimpleSample".							//
//																			//
// This (poggled) code is copyright of Dr Nigel Barlow, lecturer in			//
// computing, University of Plymouth, UK.  email: nigel@soc.plymouth.ac.uk.	//
//																			//
// You may use, modify and distribute this (rather cack-handed in places)	//
// code subject to the following conditions:								//
//																			//
//	1:	You may not use it, or sell it, or use it in any adapted form for	//
//		financial gain, without my written premission.						//
//																			//
//	2:	You must not remove the copyright messages.							//
//																			//
//	3:	You should correct at least 10% of the typig abd spekking errirs.   //
//																			//
// Poggeling ©Nigel Barlow nigel@soc.plymouth.ac.uk.						//
// "Simple Sample" ©Microsoft Corporation. All rights reserved.				//
//**************************************************************************//



//**************************************************************************//
// Global variables.  They are all in the same constant buffer, cb0, but we //
// could optimise performance by using different buffers for things which   //			
// change less frequently.  We could when I get my head around it!			//
//**************************************************************************//

cbuffer cb0
{
	float4 g_MaterialAmbientColor;      // Material's ambient color
	float4 g_MaterialDiffuseColor;      // Material's diffuse color
	float3 g_LightDir;                  // Light's direction in world space
	float4 g_LightDiffuse;              // Light's diffuse color

	float    g_fTime;                   // App's time in seconds
	float    g_fWingAngle;				// Wing angle
	matrix   g_mWorld;                  // World matrix for object
	matrix   g_mWorldViewProjection;    // World * View * Projection matrix
}


Texture2D g_MeshTexture;				// Color texture, only a single texture
										//in this shader file.


//**************************************************************************//
// Texture samplers.  A simple texture sampler is all we need here.			//
//**************************************************************************//

SamplerState MeshTextureSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};




//**************************************************************************//
// Vertex shader input structure.	The semantics (the things after the		//
// colon) look a little weird.  The semantics are used (so Microsoft tell	//
// us) used by the compiler to link shader inputs and outputs. 				//
//																			//
// For this to work, you must ensure that the vertex structure you use in	//
// any program that uses this shader is the same as below, vertex position,	//
// normal vector and the texture UV coordinates, in that order!				//
//**************************************************************************//

struct VS_INPUT
{
	float4 vPos       : POSITION;	
    float3 vNormal    : NORMAL;		
    float2 vTexCoord0 : TEXCOORD0;
};



//**************************************************************************//
// Vertex shader output structure.	This is also the imput to the pixel		//
//**************************************************************************//

struct VS_OUTPUT
{
    float4 Position   : SV_Position;// vertex position 
    float4 Diffuse    : COLOR0;     // vertex diffuse color (note that COLOR0 is clamped from 0..1)
    float2 TextureUV  : TEXCOORD0;  // vertex texture coords 
};


//**************************************************************************//
// This shader computes standard transform and lighting.   Calculating		//
// lighting in the vertex xhader is termed "per vertex shading" and, as the //
// term suggests, performs one lighting calculation on each verted.			//
// Performing the lighting calculations in the pixel shader is more accurate//
// and gives better results, but it slower.									//
//**************************************************************************//

VS_OUTPUT RenderSceneVS(VS_INPUT input)
{
    VS_OUTPUT Output;
    float3 vNormalWorldSpace;
    
    // Transform the position from object space to homogeneous projection space
    Output.Position = mul(input.vPos, g_mWorldViewProjection);
    
    // Whatever we do to the object we must do to its normal vector.    
    vNormalWorldSpace = normalize(mul(input.vNormal, (float3x3)g_mWorld)); // normal (world space)

    // Calc diffuse color    
    Output.Diffuse.rgb = g_MaterialDiffuseColor * g_LightDiffuse * max(0,dot(vNormalWorldSpace, g_LightDir)) + 
                         g_MaterialAmbientColor;   
        
    Output.Diffuse.a = 1.0f; 
    
    // Just copy the texture coordinate through
    Output.TextureUV = input.vTexCoord0; 
    
    return Output;    
}


VS_OUTPUT RenderWingVS(VS_INPUT input)
{
    VS_OUTPUT Output;
    float3 vNormalWorldSpace;

	//float distance = input.vPos.x;
	//input.vPos.y += g_fWingAngle*((distance/3)+((distance*distance)/4));

	float time = g_fTime * 5;
	float distance = input.vPos.x;
	float ss = cos(time+distance);

	if(distance < 0.2) distance = 0;

	input.vPos.y += (distance/2)*ss;
    
    // Transform the position from object space to homogeneous projection space
    Output.Position = mul(input.vPos, g_mWorldViewProjection);
    
    // Whatever we do to the object we must do to its normal vector.    
    vNormalWorldSpace = normalize(mul(input.vNormal, (float3x3)g_mWorld)); // normal (world space)

    // Calc diffuse color    
    Output.Diffuse.rgb = g_MaterialDiffuseColor * g_LightDiffuse * max(0,dot(vNormalWorldSpace, g_LightDir)) + 
                         g_MaterialAmbientColor;   
        
    Output.Diffuse.a = 1.0f; 
    
    // Just copy the texture coordinate through
    Output.TextureUV = input.vTexCoord0; 
    
    return Output;    
}

VS_OUTPUT RenderSkyBoxVS(VS_INPUT input)
{
    VS_OUTPUT Output;
    float3 vNormalWorldSpace;
    
    // Transform the position from object space to homogeneous projection space
    Output.Position = mul(input.vPos, g_mWorldViewProjection);
    
    // Whatever we do to the object we must do to its normal vector.    
    vNormalWorldSpace = normalize(mul(input.vNormal, (float3x3)g_mWorld)); // normal (world space)

    // Calc diffuse color    
    Output.Diffuse.rgb =  float4(1, 1, 1, 1);//1;//g_MaterialDiffuseColor + 
                         //g_MaterialAmbientColor;   
        
    Output.Diffuse.a = 1.0f; 
    
    // Just copy the texture coordinate through
    Output.TextureUV = input.vTexCoord0; 
    
    return Output;    
}




//**************************************************************************//
// The pixel shader.  This shader outputs the pixel's color by modulating   //
// the texture's color with diffuse material color.  As above, this is "per //
// vertex lighting, which doen't gove as good results as "per pixel			//
// lighting, which we aren't doing here.									//
//**************************************************************************//

float4 RenderScenePS( VS_OUTPUT In ) : SV_Target
{ 
    // Lookup mesh texture and modulate it with diffuse
    return g_MeshTexture.Sample(MeshTextureSampler, In.TextureUV) * In.Diffuse;
}


//**************************************************************************//
// Renders scene.  In shader jargon this is a technique.  Typically, a		//
// technique involes a vertex shader and a pixel shader.  You MUST have one //
// technique, but you can also have many techniques in a shader.			//
//																			//
// You call the rechnique what you like, but you must know its name in the  //
// program that invokes the shader.											//
//																			//
// You can see that we are using shader model 4.0 here (though I haven't got//
// my head around the geometry shader at this point).						//
//**************************************************************************//

technique10 RenderScene
{
    pass P0
    {       
		SetVertexShader( CompileShader( vs_4_0, RenderSceneVS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, RenderScenePS() ) );
    }
}	

technique10 RenderWing
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_4_0, RenderWingVS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, RenderScenePS() ) );
	}
}


technique10 RenderSkyBox
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_4_0, RenderSkyBoxVS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, RenderScenePS() ) );
	}
}



//And that's all folks.
