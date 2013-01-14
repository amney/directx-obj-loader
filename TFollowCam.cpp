#include "DXUT.h"
#include "TFollowCam.h"


TFollowCam::TFollowCam(TObject3D *ObjectToFollow, CModelViewerCamera *Camera)
{
	m_ObjectToFollow = ObjectToFollow;
	m_Camera = Camera;
}


TFollowCam::~TFollowCam(void)
{
}

void TFollowCam::Update()
{
	//**************************************************************//
	//Put the camera on the thing.   We have to create a new viewer //
	// matrix.  Here the viewer matris is hidden in the camera.  In //
	// fact the camera class Microsoft give us can follow objects	//
	// anyway, if somebody can find out how!						//
	//**************************************************************//

	D3DXVECTOR3 viewerPos;
	D3DXVECTOR3 lookAtThis;
	D3DXVECTOR3 up         ( 0.0f, 1.0f, 0.0f );
	D3DXVECTOR3 newUp;

	//Set the viewer's position to the position of the thing.

	viewerPos.x = m_ObjectToFollow->position->m_x;   viewerPos.y = m_ObjectToFollow->position->m_y;
	viewerPos.z = m_ObjectToFollow->position->m_z;

	//*****************************************************************//
	// Create a new vector for the direction for the virwer to look in.//
	// We do this by getting the object's initial direction vector,    //
	// and multiplying it by the thing's combined x, y, z rotation     //
	// matrices.   Up is still up; I'll leave that to you!             //
	//*****************************************************************//

	m_ObjectToFollow->setupWorldMatrices();

	D3DXVECTOR3 newDir, lookAtPoint;
	D3DXVec3TransformCoord(&newDir, &m_ObjectToFollow->position->initVecDir, &m_ObjectToFollow->getRotationMatrix());

	// The viewer should now be looking at a point a little in front//
	// of the object.   I hope.   Nigel.                            //

	D3DXVec3Normalize(&newDir, &newDir);

	newDir *= -10;

	viewerPos += newDir;
	viewerPos.y += 3;

	lookAtPoint.x = m_ObjectToFollow->position->m_x;
	lookAtPoint.y = m_ObjectToFollow->position->m_y;
	lookAtPoint.z = m_ObjectToFollow->position->m_z;

	m_Camera->SetViewParams(&viewerPos, &lookAtPoint);

}
