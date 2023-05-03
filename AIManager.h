#pragma once

#include "WaypointManager.h"

using namespace std;

class Vehicle;
class PickupItem;
typedef vector<PickupItem*> vecPickups;
class TaskManager;

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
	void	setRandomPickupPosition(PickupItem* pickup);

private:
	vecPickups              m_pickups;
	Vehicle*				m_pRedCar = nullptr;
	Vehicle*				m_pBlueCar = nullptr;
	Vehicle*				m_pCurrentCar = nullptr;
	Vehicle*				m_pOtherCar = nullptr;
	WaypointManager			m_waypointManager;
	TaskManager*			m_pTaskManager = nullptr;

	WPARAM					m_currentMode = NULL;
};

