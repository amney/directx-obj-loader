#include "DXUT.h"
#include "TObject3D.h"




TObject3D::TObject3D( ID3D10Device *device, TEffect *effect, ID3D10EffectTechnique *technique, CDXUTSDKMesh *mesh) : TObject(device, effect, technique)
{
	this->mesh = mesh;
	xSpeed = ySpeed = zSpeed = 0;
	g		   = 2.0f;
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

	mesh->Render(device,technique,effect->g_p_txDiffuseInShader);
}


void TObject3D::Update(float frameTime)
{

	FLOAT deltaX = xSpeed*frameTime;  //Calculate distance travelled in
	FLOAT deltaY = ySpeed*frameTime;  //x, y, z directions.
	FLOAT deltaZ = zSpeed*frameTime;


	if(position->m_y > 0.8){

	ySpeed -= g*frameTime;	  //Acceleration downwards due to gravity.

	}
	else{
		ySpeed = 0;
	}

	position->m_x += deltaX;	position->m_y += deltaY;	position->m_z += deltaZ;   // Add distances travelled.
}