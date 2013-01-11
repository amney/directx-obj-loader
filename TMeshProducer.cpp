#include "DXUT.h"
#include "TMeshProducer.h"


TMeshProducer::TMeshProducer(ID3D10Device *device)
{
	m_device = device;
}


TMeshProducer::~TMeshProducer(void)
{
}

CDXUTSDKMesh* TMeshProducer::ProduceTiger()
{
	if(!m_tiger.IsLoaded()){
		m_tiger.Create(m_device, L"Media\\Tiger\\Tiger.sdkmesh", true );
	}
	
	return &m_tiger;
}

CDXUTSDKMesh* TMeshProducer::ProduceWing()
{
	if(!m_wing.IsLoaded()){
		m_wing.Create(m_device, L"Media\\Wing\\Wing.sdkmesh", true );
	}

	return &m_wing;
}

CDXUTSDKMesh* TMeshProducer::ProduceSkyBox()
{
	if(!m_skybox.IsLoaded()){
		m_skybox.Create(m_device, L"Media\\CloudBox\\skysphere.sdkmesh", true );
	}

	return &m_skybox;
}