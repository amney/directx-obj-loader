#pragma once

struct xyz{
	float x;
	float y;
	float z;
};

class Position
{
public:
	Position(void);
	~Position(void);
	void MoveTo(float x, float y, float z);
	void MoveBy(float x, float y, float z);

	void RotateToDeg(float rx, float ry, float rz);
	void RotateByDeg(float rx, float ry, float rz);

	void RotateToRad(float rx, float ry, float rz);
	void RotateByRad(float rx, float ry, float rz);

	void ScaleTo(float sx, float sy, float sz);
	void ScaleBy(float sx, float sy, float sz);

	xyz GetRotationXYZ();
	xyz GetScaleXYZ();
	xyz GetPositionXYZ();

	void SetRotationXYZ(xyz Rotation);
	void SetScaleXYZ(xyz Scale);
	void SetPositionXYZ(xyz Position);

	void MoveForward(float frameTime);

public:
	float m_x, m_y, m_z;
	float m_rx, m_ry, m_rz;
	float m_sx, m_sy, m_sz;

	D3DXVECTOR3 initVecDir;


};

