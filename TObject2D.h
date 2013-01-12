#pragma once
#include "tobject.h"
#include "TTexture.h"
class TObject2D :
	public TObject
{
public:
	TObject2D(ID3D10Device *device, TEffect *effect, ID3D10EffectTechnique *technique, TTexture *textures);
	~TObject2D(void);
	void Render(D3DMATRIX viewProjection, D3DMATRIX context);
	void SetupForRender();
	void FastRender(D3DMATRIX viewProjection, D3DMATRIX context);

	TTexture *m_texture;
};

