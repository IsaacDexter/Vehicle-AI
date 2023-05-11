#include "SpeedPickup.h"
#include "Vehicle.h"

HRESULT SpeedPickup::initMesh(ID3D11Device* pd3dDevice)
{
	setTextureName(L"Resources\\pickup_speedboost.dds");
	return PickupItem::initMesh(pd3dDevice);
}

void SpeedPickup::pickup(Vehicle* collector)
{
	collector->SetSpeed(collector->GetSpeed() + m_speedIncrease);
}
