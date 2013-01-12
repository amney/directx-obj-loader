#include "DXUT.h"
#include "TObject2D.h"
#include "Nig.h"
#include "TTexture.h"

//**************************************************************************//
// Structure to define a vertex.  Thid is linked in with the shader we use, //
// and this won't work unless you have a shader with a posotion, normal		//
// vector and textureUV coordinates.										//
//**************************************************************************//




TObject2D::TObject2D(ID3D10Device *device, TEffect *effect, ID3D10EffectTechnique *technique, TTexture *texture) : TObject(device, effect, technique)
{
	m_texture = texture;
}


TObject2D::~TObject2D(void)
{
}

void TObject2D::Render(D3DMATRIX viewProjection, D3DMATRIX context){
	SetupForRender();
	FastRender(viewProjection, context);
}

void TObject2D::SetupForRender()
{
	device->IASetInputLayout(effect->g_pVertexLayout);
	m_texture->SetupForRender();
}

void TObject2D::FastRender( D3DMATRIX viewProjection, D3DMATRIX context )
{
	setupWorldMatrices();

	matWorld = getWorldMatrices() * context;
	matWVP = matWorld * viewProjection;

	effect->g_p_MatWorldViewProjInShader->SetMatrix( ( float* )&matWVP );
	effect->g_p_MatWorldInShader->SetMatrix(         ( float* )&matWorld );

	m_texture->FastRender();
}
