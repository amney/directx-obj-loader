#pragma once
#include "tobject3d.h"
class TBall :
	public TObject3D
{
public:
	TBall(ID3D10Device *device, TEffect *effect, ID3D10EffectTechnique *technique, CDXUTSDKMesh *mesh);
	~TBall(void);
	void update(float frameTime);

	public:

		FLOAT groundY;		//Position of the ground.
		FLOAT bouncyness;
		FLOAT r;			//Radius..
		FLOAT g;			//Gravity.
		FLOAT xSpeed, ySpeed, zSpeed;   //Separate speed into these 3 components.
};

