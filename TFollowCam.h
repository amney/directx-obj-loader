#pragma once

#include "TObject3D.h"
#include "DXUT\Optional\DXUTcamera.h"

class TFollowCam
{
public:
	TFollowCam(TObject3D *ObjectToFollow, CModelViewerCamera *Camera);
	~TFollowCam(void);
	void Update();

private:
	TObject3D *m_ObjectToFollow;
	CModelViewerCamera *m_Camera;
};

