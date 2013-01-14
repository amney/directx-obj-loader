#include "DXUT.h"
#include "TMeshProductionLimiter.h"


TMeshProductionLimiter::TMeshProductionLimiter(float Limit)
{
	m_limit = Limit;
	m_totalFrameTime = 0;
}


TMeshProductionLimiter::~TMeshProductionLimiter(void)
{
}

void TMeshProductionLimiter::Update(float frameTime)
{
	m_totalFrameTime += frameTime;
}

bool TMeshProductionLimiter::CanProduce()
{
	if(m_totalFrameTime > m_limit) {
		m_totalFrameTime = 0;	
		return true;
	}
	return false;
}