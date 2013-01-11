#pragma once

#include "DXUT.h"

#include "Position.h"
#include "TEffect.h"
#include <d3d10.h>
#include <vector>


const D3D10_INPUT_ELEMENT_DESC TObjectVertexLayout[] =
{
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
    { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 },
    { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0 },
};

class TObject
{
public:
	TObject(ID3D10Device *device, TEffect *effect, ID3D10EffectTechnique *technique);
	~TObject(void);

	virtual void Render(D3DMATRIX viewProjection, D3DMATRIX context) = 0;

	Position *position;
	TEffect *effect;
	ID3D10EffectTechnique *technique;
	ID3D10Device *device;

	void setupWorldMatrices();
	D3DXMATRIX getWorldMatrices();
	D3DXMATRIX getRotationMatrix();

protected:
	D3DXMATRIX RotationXYZ;
	D3DXMATRIX ScaleXYZ;
	D3DXMATRIX TranslationXYZ;

	D3DXMATRIX matWorld;
	D3DXMATRIX matWVP;


};

