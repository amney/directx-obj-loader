#include "DXUT.h"
#include "TBall.h"


TBall::TBall(ID3D10Device *device, TEffect *effect, ID3D10EffectTechnique *technique, CDXUTSDKMesh *mesh) : TObject3D(device, effect, technique, mesh) 
{
	groundY    = 0.3;
	bouncyness = 0.8f;
	g		   = 9.8f;
	xSpeed = ySpeed = zSpeed = 0;	//Initially still.
	r = 0.3;
}


TBall::~TBall(void)
{
}

void TBall::update(float frameTime)
{

	FLOAT deltaX = xSpeed*frameTime;  //Calculate distance travelled in
	FLOAT deltaY = ySpeed*frameTime;  //x, y, z directions.
	FLOAT deltaZ = zSpeed*frameTime;

	ySpeed -= g*frameTime;	  //Acceleration downwards due to gravity.

	position->m_x += deltaX;	position->m_y += deltaY;	position->m_z += deltaZ;   // Add distances travelled.
	
	if (position->m_y < (groundY+r))
		{
		ySpeed = -ySpeed*bouncyness;

		if(ySpeed < 1.5) ySpeed = 0; //If ySpeed is less than one, just sit on the ground
		
		position->m_y = groundY+r;					//Ensure above test false next frame.
		}

}
