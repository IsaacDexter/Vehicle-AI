#include "FuelPickup.h"
#include "Vehicle.h"

HRESULT FuelPickup::initMesh(ID3D11Device* pd3dDevice)
{
	m_type = PickupType::FUEL;
	setTextureName(L"Resources\\pickup_fuel.dds");
	return PickupItem::initMesh(pd3dDevice);
}

void FuelPickup::pickup(Vehicle* collector)
{
	collector->Refuel(m_fuel);
}
