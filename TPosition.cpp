#include "DXUT.h"
#include "TPosition.h"


TPosition::TPosition(void)
{
	initVecDir = D3DXVECTOR3(0,0,-1);
	m_thrust = 0;
	MoveTo(0,0,0);
	RotateToDeg(0,0,0);
	ScaleTo(1,1,1);
}


TPosition::~TPosition(void)
{
}

void TPosition::MoveTo(float x, float y, float z)
{
	m_x = x;
	m_y = y;
	m_z = z;
}

void TPosition::MoveBy(float x, float y, float z)
{
	m_x += x;
	m_y += y;
	m_z += z;
}

void TPosition::RotateToDeg(float rx, float ry, float rz)
{
	m_rx = D3DXToRadian(rx);
	m_ry = D3DXToRadian(ry);
	m_rz = D3DXToRadian(rz);
}

void TPosition::RotateByDeg(float rx, float ry, float rz)
{
	m_rx += D3DXToRadian(rx);
	m_ry += D3DXToRadian(ry);
	m_rz += D3DXToRadian(rz);
}

void TPosition::RotateToRad(float rx, float ry, float rz)
{
	m_rx = rx;
	m_ry = ry;
	m_rz = rz;
}

void TPosition::RotateByRad(float rx, float ry, float rz)
{
	m_rx += rx;
	m_ry += ry;
	m_rz += rz;
}

void TPosition::ScaleTo(float sx, float sy, float sz)
{
	m_sx = sx;
	m_sy = sy;
	m_sz = sz;
}

void TPosition::ScaleBy(float sx, float sy, float sz)
{
	m_sx += sx;
	m_sy += sy;
	m_sz += sz;
}

void TPosition::MoveForward(float frameTime)
{
	//*******************************************************************//
	// Help!   I need to re-create the rotation matrices here; I have no //
	// idea why.   The whole point of making the matrices members of this//
	// class was so that I need calculate them only once.   Remove the   //
	// lines to the ------------- and this should still work, but doesn't//
	// - Nigel.                                                          //
	//*******************************************************************//
	m_thrust *= 0.992;

	D3DXMATRIX matRotateXYZ;

	D3DXMatrixRotationYawPitchRoll(&matRotateXYZ,m_ry,m_rx,m_rz);

	//-------------------------------------------------------------------//

	D3DXVECTOR3 newVecDir;

	D3DXVECTOR3 currentVecDir;

	D3DXVec3TransformCoord(&newVecDir, &initVecDir, &matRotateXYZ);
	D3DXVec3Normalize( &currentVecDir, &newVecDir );

	float adjustedSpeed = m_thrust * frameTime;

	MoveBy(currentVecDir.x*adjustedSpeed,currentVecDir.y*adjustedSpeed,currentVecDir.z*adjustedSpeed);
}



void TPosition::LookUp(float ByAngle, float MaxAngle)
{
	if((m_rx + ByAngle) > MaxAngle) return;
	m_rx += ByAngle;
}

void TPosition::LookDown(float ByAngle, float MaxAngle)
{
	if((m_rx + ByAngle) < MaxAngle) return;
	m_rx += ByAngle;
}

xyz TPosition::GetRotationXYZ()
{
	xyz pos;
	pos.x = m_rx;
	pos.y = m_ry;
	pos.z = m_rz;

	return pos;
}

xyz TPosition::GetScaleXYZ()
{
	xyz pos;
	pos.x = m_sx;
	pos.y = m_sy;
	pos.z = m_sz;

	return pos;
}

xyz TPosition::GetPositionXYZ()
{
	xyz pos;
	pos.x = m_x;
	pos.y = m_y;
	pos.z = m_z;

	return pos;
}

void TPosition::SetRotationXYZ( xyz Rotation )
{
	m_rx = Rotation.x;
	m_ry = Rotation.y;
	m_rz = Rotation.z;
}

void TPosition::SetScaleXYZ( xyz Scale )
{
	m_sx = Scale.x;
	m_sy = Scale.y;
	m_sz = Scale.z;
}

void TPosition::SetPositionXYZ( xyz Position )
{
	m_x = Position.x;
	m_y = Position.y;
	m_z = Position.z;
}

void TPosition::IncreaseThrust( float thrust )
{
	m_thrust += thrust;
	m_thrust = min(m_thrust,30);
}
