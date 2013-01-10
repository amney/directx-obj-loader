#pragma once
#include "tobject.h"
class TObject2D :
	public TObject
{
public:
	TObject2D(ID3D10Device *device, TEffect *effect, ID3D10EffectTechnique *technique, LPCTSTR textureFileName);
	~TObject2D(void);
	void Render(D3DMATRIX viewProjection, D3DMATRIX context);

	ID3D10RenderTargetView	  *pRenderTargetView;
	ID3D10Buffer			  *pVertexBuffer;
	ID3D10Buffer			  *pIndexBuffer;
	ID3D10ShaderResourceView  *ptxtResourceViewInShader;
};

