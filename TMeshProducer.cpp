#include "DXUT.h"
#include "TMeshProducer.h"


TMeshProducer::TMeshProducer(ID3D10Device *device)
{
	m_device = device;
}


TMeshProducer::~TMeshProducer(void)
{
	m_tiger.Destroy();
	m_wing.Destroy();
	m_skybox.Destroy();
	m_pipebomb.Destroy();
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

CDXUTSDKMesh* TMeshProducer::ProducePipebomb()
{
	if(!m_pipebomb.IsLoaded()){
		m_pipebomb.Create(m_device, L"Media\\Dynamite\\dynamite.sdkmesh", true );
	}

	return &m_pipebomb;
}
