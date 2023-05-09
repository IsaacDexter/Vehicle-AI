#pragma once

struct ID3D11Device;


#include <d3d11_1.h> // this has the appropriate directx structures / objects
#include <DirectXCollision.h> // this is the dx collision class helper
using namespace DirectX; // this means you don't need to put DirectX:: in front of objects like XMVECTOR and so on. 


#include <vector>
using namespace std;

class Waypoint;
class Vehicle;

#include "Vector2D.h"
#define WAYPOINT_RESOLUTION 20

struct DynamicBoundingBox
{
public:
	DynamicBoundingBox()
	{
		centre = nullptr;
		corners = std::array<Vector2D, 4>();
	};
	DynamicBoundingBox(Vector2D* centre, Vector2D halfExtents)
	{
		this->centre = centre;
		corners = std::array<Vector2D, 4>();
		corners[0] = Vector2D(+ halfExtents.x, + halfExtents.y);	//Top Right
		corners[1] = Vector2D(- halfExtents.x, + halfExtents.y);	//Top Left
		corners[2] = Vector2D(- halfExtents.x, - halfExtents.y);	//Bottom Left
		corners[3] = Vector2D(+ halfExtents.x, - halfExtents.y);	//Bottom Right
	};
	BoundingBox GetBoundingBox()
	{
		BoundingBox bb = CollisionHelper::createBoundingBoxFromPoints(	corners[0] + *centre,
																		corners[1] + *centre,
																		corners[2] + *centre,
																		corners[3] + *centre	);	//Create a bounding box from the corners plus the centre.
		return bb;
	};
private:
	Vector2D* centre;
	std::array<Vector2D, 4> corners;
};

typedef vector<Waypoint*> vecWaypoints;
typedef vector <BoundingBox*> vecBoundingBox;
typedef std::pair<Vector2D, Vector2D> Line;
typedef std::map<Vehicle*, DynamicBoundingBox> mapDynamicBoundingBox;

class WaypointManager
{
public:
	WaypointManager() { m_numCheckpoints = 0; };
	~WaypointManager();

	HRESULT createWaypoints(ID3D11Device* pd3dDevice);
	/// <summary>Creates a dynamic bounding box that updates itself according to the position reference passed in</summary>
	/// <param name="vehicle">The vehicle to use as the key in the map</param>
	/// <param name="centre">a reference to the vehicles position to update the bounding box</param>
	/// <param name="extents">The width and height of the vehicle, halved</param>
	void createDynamicBoundingBox(Vehicle* vehicle, Vector2D& centre,  Vector2D extents);
	void destroyWaypoints();

	Waypoint* getWaypoint(const unsigned int index);
	Waypoint* getRandomWaypoint();
	size_t getWaypointCount() { return m_waypoints.size(); }

	// get the points which make a building on the map
	Waypoint* getQuadpoint(const unsigned int index);
	size_t getQuadpointCount() { return m_quadpoints.size(); }
	
	const BoundingBox* doesLineCrossBuilding(Line line);
	const BoundingBox* doesLineCrossVehicle(Line line);
	//Line getNearestEdge(BoundingBox* bb, Vector2D position);
	float getDistanceBetweenLineAndPoint(Line line, Vector2D point);
	float getSquaredDistanceBetweenLineAndPoint(Line line, Vector2D point);
	
	
	vecWaypoints getNeighbouringWaypoints(Waypoint* waypoint);
	Waypoint* getNearestWaypoint(Vector2D position);

protected: // methods
	// given two waypoints, if a line were to be drawn between them, would that cross a building?
	bool	doWaypointsCrossBuilding(Waypoint* wp1, Waypoint* wp2);


protected: // properties
	vecWaypoints	m_waypoints;
	vecWaypoints	m_quadpoints;
	int				m_numCheckpoints;
	vecBoundingBox	m_boundingBoxes;
	mapDynamicBoundingBox	m_vehicleBoundingBoxes;

};

