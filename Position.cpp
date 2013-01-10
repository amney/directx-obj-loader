#include "DXUT.h"
#include "Position.h"


Position::Position(void)
{
	initVecDir = D3DXVECTOR3(0,0,-1);
	MoveTo(0,0,0);
	RotateToDeg(0,0,0);
	ScaleTo(1,1,1);
}


Position::~Position(void)
{
}

void Position::MoveTo(float x, float y, float z)
{
	m_x = x;
	m_y = y;
	m_z = z;
}

void Position::MoveBy(float x, float y, float z)
{
	m_x += x;
	m_y += y;
	m_z += z;
}

void Position::RotateToDeg(float rx, float ry, float rz)
{
	m_rx = D3DXToRadian(rx);
	m_ry = D3DXToRadian(ry);
	m_rz = D3DXToRadian(rz);
}

void Position::RotateByDeg(float rx, float ry, float rz)
{
	m_rx += D3DXToRadian(rx);
	m_ry += D3DXToRadian(ry);
	m_rz += D3DXToRadian(rz);
}

void Position::RotateToRad(float rx, float ry, float rz)
{
	m_rx = rx;
	m_ry = ry;
	m_rz = rz;
}

void Position::RotateByRad(float rx, float ry, float rz)
{
	m_rx += rx;
	m_ry += ry;
	m_rz += rz;
}

void Position::ScaleTo(float sx, float sy, float sz)
{
	m_sx = sx;
	m_sy = sy;
	m_sz = sz;
}

void Position::ScaleBy(float sx, float sy, float sz)
{
	m_sx += sx;
	m_sy += sy;
	m_sz += sz;
}

void Position::MoveForward(float frameTime)
{
	//*******************************************************************//
	// Help!   I need to re-create the rotation matrices here; I have no //
	// idea why.   The whole point of making the matrices members of this//
	// class was so that I need calculate them only once.   Remove the   //
	// lines to the ------------- and this should still work, but doesn't//
	// - Nigel.                                                          //
	//*******************************************************************//
	D3DXMATRIX matRotateXYZ;

	D3DXMatrixRotationYawPitchRoll(&matRotateXYZ,m_ry,m_rx,m_rz);

	//-------------------------------------------------------------------//

	D3DXVECTOR3 newVecDir;

	D3DXVECTOR3 currentVecDir;

	D3DXVec3TransformCoord(&newVecDir, &initVecDir, &matRotateXYZ);
	D3DXVec3Normalize( &currentVecDir, &newVecDir );


	MoveBy(currentVecDir.x*frameTime,currentVecDir.y*frameTime,currentVecDir.z*frameTime);
}

xyz Position::GetRotationXYZ()
{
	xyz pos;
	pos.x = m_rx;
	pos.y = m_ry;
	pos.z = m_rz;

	return pos;
}

xyz Position::GetScaleXYZ()
{
	xyz pos;
	pos.x = m_sx;
	pos.y = m_sy;
	pos.z = m_sz;

	return pos;
}

xyz Position::GetPositionXYZ()
{
	xyz pos;
	pos.x = m_x;
	pos.y = m_y;
	pos.z = m_z;

	return pos;
}

void Position::SetRotationXYZ( xyz Rotation )
{
	m_rx = Rotation.x;
	m_ry = Rotation.y;
	m_rz = Rotation.z;
}

void Position::SetScaleXYZ( xyz Scale )
{
	m_sx = Scale.x;
	m_sy = Scale.y;
	m_sz = Scale.z;
}

void Position::SetPositionXYZ( xyz Position )
{
	m_x = Position.x;
	m_y = Position.y;
	m_z = Position.z;
}
