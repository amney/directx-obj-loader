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
	if (device == NULL)
	{
		ShowMessage(L"Flat Thing3D: Gimme a rendering device!");
		exit(1);	//Brutal exit!  No cleaning up.
	}


	setupWorldMatrices();

	matWorld = getWorldMatrices() * context;
	matWVP = matWorld * viewProjection;

	effect->g_p_MatWorldViewProjInShader->SetMatrix( ( float* )&matWVP );
    effect->g_p_MatWorldInShader->SetMatrix(         ( float* )&matWorld );
	device->IASetInputLayout(effect->g_pVertexLayout);

	m_texture->Render();
}
