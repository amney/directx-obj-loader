#include "DXUT.h"
#include "TObject.h"




TObject::TObject( ID3D10Device *device, TEffect *effect, ID3D10EffectTechnique *technique)
{
	this->device = device;
	this->effect = effect;
	this->technique = technique;
	position = new Position();
}


TObject::~TObject(void)
{
}

void TObject::setupWorldMatrices()
{
	D3DXMatrixTranslation(&TranslationXYZ,position->m_x,position->m_y,position->m_z);
	D3DXMatrixScaling(&ScaleXYZ,position->m_sx,position->m_sy,position->m_sz);
	D3DXMatrixRotationYawPitchRoll(&RotationXYZ,position->m_ry,position->m_rx,position->m_rz);
}

D3DXMATRIX TObject::getWorldMatrices()
{
	return RotationXYZ * ScaleXYZ * TranslationXYZ;
}

D3DXMATRIX TObject::getRotationMatrix()
{
	return RotationXYZ;
}