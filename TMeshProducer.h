#pragma once
#include "TObject3D.h"
class TMeshProducer
{
public:
	TMeshProducer(ID3D10Device *device);
	~TMeshProducer(void);

	CDXUTSDKMesh* ProduceTiger();
	CDXUTSDKMesh* ProduceWing();
	CDXUTSDKMesh* ProduceSkyBox();

private:
    CDXUTSDKMesh m_tiger;
	CDXUTSDKMesh m_wing;
	CDXUTSDKMesh m_skybox;

	ID3D10Device *m_device;
};

