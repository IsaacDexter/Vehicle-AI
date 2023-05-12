#include "PassengerPickup.h"
#include "Vehicle.h"
#include "Passenger.h"

HRESULT PassengerPickup::initMesh(ID3D11Device* pd3dDevice)
{
	m_pd3dDevice = pd3dDevice;
	m_type = PickupType::PASSENGER;
	setTextureName(L"Resources\\pickup_passenger.dds");
	return PickupItem::initMesh(pd3dDevice);
}

void PassengerPickup::pickup(Vehicle* collector)
{
	Passenger* passenger = new Passenger(m_pd3dDevice);
	passenger->Pickup(collector);
}
