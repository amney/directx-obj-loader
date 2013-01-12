#include "DXUT.h"
#include "TObject3D.h"




TObject3D::TObject3D( ID3D10Device *device, TEffect *effect, ID3D10EffectTechnique *technique, CDXUTSDKMesh *mesh) : TObject(device, effect, technique)
{
	this->mesh = mesh;
}

TObject3D::~TObject3D(void)
{
}

void TObject3D::Render(D3DMATRIX viewProjection, D3DMATRIX context){

	SetupForRender();
	FastRender(viewProjection,context);

	//Render all of my children objects using my context
	for(std::vector<TObject *>::const_iterator it = children.begin(); it != children.end(); it++){
		(*it)->Render(viewProjection,matWorld);
	}
}

void TObject3D::SetupForRender()
{
	device->IASetInputLayout(effect->g_pVertexLayout);
}

void TObject3D::FastRender( D3DMATRIX viewProjection, D3DMATRIX context )
{
	setupWorldMatrices();

	matWorld = getWorldMatrices() * context;
	matWVP = matWorld * viewProjection;

	effect->g_p_MatWorldViewProjInShader->SetMatrix( ( float* )&matWVP );
	effect->g_p_MatWorldInShader->SetMatrix(         ( float* )&matWorld );

	//This seems to be sufficient to replace...
	mesh->Render(device,technique,effect->g_p_txDiffuseInShader);

}
