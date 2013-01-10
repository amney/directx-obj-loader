#include "DXUT.h"
#include "TBall.h"


TBall::TBall(ID3D10Device *device, TEffect *effect, ID3D10EffectTechnique *technique, CDXUTSDKMesh *mesh) : TObject3D(device, effect, technique, mesh) 
{
	groundY    = 0.3;
	bouncyness = 0.8f;
	g		   = 9.8f;
	xSpeed = ySpeed = zSpeed = 0;	//Initially still.
	r = 0.5;
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
	

	
//Now handle collision with the floor.   Collisions with any walls can //
	//be handled in exactly the same way.								   //

	if (position->m_y < (groundY+r))
		{
		ySpeed = -ySpeed*bouncyness;	//Bounce back.
		position->m_y = groundY+r;					//Ensure above test false next frame.

		//That was the maths.   Now make a sound.
		//playSound();
		}



	//This is art.   If the ball is within 2* radius of the floor, squash//
	//it.																 //

	/*if (position->m_y < (groundY+r*2))
		{
		FLOAT scaleY = (position->m_y - groundY);
		position->ScaleTo(1, scaleY, 1);
		}
	else position->ScaleTo(1, 1, 1); */  //Not very good (fast) to do this every frame.


}
