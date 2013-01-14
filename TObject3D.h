#pragma once
#include "tobject.h"
#include "SDKmesh.h"


class TObject3D :
	public TObject
{
public:
	TObject3D( ID3D10Device *device, TEffect *effect, ID3D10EffectTechnique *technique, CDXUTSDKMesh *mesh);
	~TObject3D(void);
	void Render(D3DMATRIX viewProjection, D3DMATRIX context);
	void SetupForRender();
	void FastRender(D3DMATRIX viewProjection, D3DMATRIX context);
	void Update(float frameTime);

	CDXUTSDKMesh *mesh;
	std::vector<TObject *> children;

private:
	float xSpeed, ySpeed, zSpeed, g;   //Separate speed into these 3 components.
};

