#pragma once
class TMeshProductionLimiter
{
public:
	TMeshProductionLimiter(float Limit);
	~TMeshProductionLimiter(void);
	void Update(float frameTime);
	bool CanProduce();

private:
	float m_totalFrameTime;
	float m_limit;
};

