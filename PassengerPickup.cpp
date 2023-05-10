#include "PassengerPickup.h"
#include "Vehicle.h"
#include "Passenger.h"

HRESULT PassengerPickup::initMesh(ID3D11Device* pd3dDevice)
{
	setTextureName(L"Resources\\pickup_passenger.dds");
	//// set the graphic for the type of pickup
	//if (m_type == pickuptype::Fuel)
	//	setTextureName(L"Resources\\pickup_fuel.dds");
	//else if (m_type == pickuptype::Passenger)
	//	setTextureName(L"Resources\\pickup_passenger.dds");
	//else
	//	setTextureName(L"Resources\\pickup_speedboost.dds");
	return PickupItem::initMesh(pd3dDevice);
}

void PassengerPickup::pickup(Vehicle* collector)
{
	Passenger* passenger = new Passenger();
	passenger->Pickup(collector);
}
