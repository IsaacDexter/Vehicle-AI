#pragma once

#include "WaypointManager.h"

using namespace std;

class Vehicle;
class TaskManager;
class PickupManager;

class AIManager
{
public:
	AIManager();
	virtual  ~AIManager();
	void	release();
	HRESULT initialise(ID3D11Device* pd3dDevice);
	void	update(const float fDeltaTime);
	void	mouseUp(int x, int y);
	void	keyDown(WPARAM param);
	void	keyUp(WPARAM param);

protected:
	bool	checkForCollisions();
	//void	setRandomPickupPosition(PickupItem* pickup);

private:
	Vehicle*				m_pRedCar = nullptr;
	Vehicle*				m_pBlueCar = nullptr;
	Vehicle*				m_pCurrentCar = nullptr;
	Vehicle*				m_pOtherCar = nullptr;
	WaypointManager			m_waypointManager;
	PickupManager*			m_pickupManager = nullptr;
	TaskManager*			m_pTaskManager = nullptr;

	WPARAM					m_currentMode = NULL;

	Vector2D				m_clickPos = Vector2D(0.0f, 0.0f);
};

