#pragma once

#include "DXUT.h"
#include "DXUTmisc.h"
#include "SDKmisc.h"
#include "SDKmesh.h"

class TEffect
{
public:
	TEffect(ID3D10Device *device, LPCTSTR fileName);
	~TEffect(void);

	ID3D10Effect	*g_p_Effect;	//The effect we hold.
	ID3D10Device	*device;
	ID3D10InputLayout      *g_pVertexLayout;
	ID3D10EffectTechnique  *g_p_TechniqueRenderScene;	//The technique within the shader.
	ID3D10EffectTechnique  *g_p_TechniqueRenderWing;	//The technique within the shader.
	ID3D10EffectTechnique  *g_p_TechniqueRenderSkyBox;	//The technique within the shader.


	//**************************************************************************//
	// This isn't really as it should be.  Lighting (light colour and direction)//
	// are surely global things, but the tiger's material is not.  But it is	//
	// here, I'll leave that for you to fix.									//
	//**************************************************************************//

	D3DXVECTOR4		g_lightDirection;				//w value unused.
	D3DXVECTOR4		g_lightDiffuseColour;			//w value unused.
	D3DXVECTOR4		g_materialDiffuseColour;		//w value unused.


	//**************************************************************************//
	// There are all variables we will use to assess variables within the		//
	// shader (.fx) file.													  //
	//**********************************************************************//

	ID3D10EffectMatrixVariable		    *g_p_MatWorldViewProjInShader;
	ID3D10EffectMatrixVariable          *g_p_MatWorldInShader;
	ID3D10EffectScalarVariable		    *g_p_fTimeInShader;
	ID3D10EffectScalarVariable		    *g_p_fWingAngleInShader;
	ID3D10EffectShaderResourceVariable  *g_p_txDiffuseInShader;
	ID3D10EffectVectorVariable		    *g_p_LightDiffuseColourInShader;
	ID3D10EffectVectorVariable	        *g_p_LightDirectionInShader;
	ID3D10EffectVectorVariable	        *g_p_MaterialDiffuseColourInShader;

private:
	void charStrToWideChar(WCHAR *dest, char *source);
};

