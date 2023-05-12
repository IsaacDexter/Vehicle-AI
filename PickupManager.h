#pragma once
#include <vector>
#include "PickupItem.h"

class DrawableGameObject;
class ID3D11Device;
class WaypointManager;
class Vehicle;

class PickupManager
{
public:
	PickupManager(ID3D11Device* pd3dDevice, WaypointManager* waypointManager);
	~PickupManager();
	void Update(float dt);
	bool CheckCollisions(Vehicle* other);
	std::vector<PickupItem*>* GetPickups() const { return m_pickups; };
	PickupItem* GetNearestPickup(Vector2D location);
	PickupItem* GetNearestPickup(Vector2D location, float& distanceSq_out);
	PickupItem* GetNearestPickup(Vector2D location, PickupType type);
	PickupItem* GetNearestPickup(Vector2D location, PickupType type, float& distanceSq_out);
private:
	void CreatePickup();
	void UpdatePickups(float dt);

	std::vector<PickupItem*>* m_pickups;
	ID3D11Device* m_pd3dDevice;
	WaypointManager* m_waypointManager;
	float m_counter = 0.0f;
	/// <summary>How Often to spawn pickups in seconds</summary>
	const float m_spawntime = 4.0f;
};

