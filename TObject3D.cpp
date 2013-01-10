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

	setupWorldMatrices();

	matWorld = getWorldMatrices() * context;
	matWVP = matWorld * viewProjection;

	effect->g_p_MatWorldViewProjInShader->SetMatrix( ( float* )&matWVP );
    effect->g_p_MatWorldInShader->SetMatrix(         ( float* )&matWorld );
	device->IASetInputLayout(effect->g_pVertexLayout);

	//This seems to be sufficient to replace...
	mesh->Render(device,technique,effect->g_p_txDiffuseInShader);


	//..All of this?
	/*UINT Strides[1];
	UINT Offsets[1];
	ID3D10Buffer* pVB[1];
	pVB[0] = mesh->GetVB10( 0, 0 );
	Strides[0] = ( UINT )mesh->GetVertexStride( 0, 0 );
	Offsets[0] = 0;
	this->device->IASetVertexBuffers( 0, 1, pVB, Strides, Offsets );
	this->device->IASetIndexBuffer( mesh->GetIB10( 0 ), mesh->GetIBFormat10( 0 ), 0 );

	D3D10_TECHNIQUE_DESC techDesc;
	this->technique->GetDesc( &techDesc );
	SDKMESH_SUBSET* pSubset = NULL;
	ID3D10ShaderResourceView* pDiffuseRV = NULL;
	D3D10_PRIMITIVE_TOPOLOGY PrimType;

	for( UINT p = 0; p < techDesc.Passes; ++p )
	{
		for( UINT subset = 0; subset < mesh->GetNumSubsets( 0 ); ++subset )
		{
			pSubset = mesh->GetSubset( 0, subset );

			PrimType = mesh->GetPrimitiveType10( ( SDKMESH_PRIMITIVE_TYPE )pSubset->PrimitiveType );
			this->device->IASetPrimitiveTopology( PrimType );

			pDiffuseRV = mesh->GetMaterial( pSubset->MaterialID )->pDiffuseRV10;
			this->effect->g_p_txDiffuseInShader->SetResource( pDiffuseRV );

			this->technique->GetPassByIndex( p )->Apply( 0 );
			this->device->DrawIndexed( ( UINT )pSubset->IndexCount, 0, ( UINT )pSubset->VertexStart );
		}

	}*/

	//Render all of my children objects using my context
	for(std::vector<TObject *>::const_iterator it = children.begin(); it != children.end(); it++){
		(*it)->Render(viewProjection,matWorld);
	}
}