#pragma once
#include "TEffect.h"
class TTexture
{
public:
	TTexture(ID3D10Device *device, TEffect *effect, ID3D10EffectTechnique *technique, LPCTSTR textureFileName);
	~TTexture(void);

	void Render();
	void SetupForRender();
	void FastRender();

private:
	ID3D10RenderTargetView	  *pRenderTargetView;
	
	ID3D10Buffer			  *pVertexBuffer;
	ID3D10Buffer			  *pIndexBuffer;
	ID3D10ShaderResourceView  *ptxtResourceViewInShader;
	
	D3DXVECTOR4				   materialDiffuseColour;
	
	TEffect					  *effect;
	ID3D10EffectTechnique     *technique;
	ID3D10Device		      *device;
};

