#include "WaypointManager.h"
#include "Waypoint.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <d3d11_1.h>
#include "CollisionHelper.h"
#include <array>



#define NEAR_WAYPOINT_DISTANCE 200.0f

struct waypointCoord {
	float x, y;
	bool quad;
};

WaypointManager::~WaypointManager()
{
	destroyWaypoints();
	printf("done");
}

// load the waypoints from a file
// waypoints are positions round the track for pathfinding
// this also loads 'quadpoints' which are points which denote the corners of buildings
HRESULT WaypointManager::createWaypoints(ID3D11Device* pd3dDevice)
{
	std::vector<waypointCoord> wpVec;

	HRESULT hr = S_OK;
	m_waypoints.clear();
	string filename = "Resources\\waypoints.txt"; 
	ifstream fin(filename.c_str());
	if (!fin.is_open())
	{
		cout << "Could not open file: " << filename << endl;
		return 1;
	}

	
	string str;
	while (getline(fin, str)) {
		// Output the text from the file
		stringstream ss;
		ss << str;
		waypointCoord wc;
		ss >> wc.x;
		ss >> wc.y;
		ss >> wc.quad;
		wpVec.push_back(wc);
	}

	fin.close();

	int checkpointID = 0;
	for (unsigned int i = 0; i < wpVec.size(); i++) {
		Waypoint* wp = new Waypoint();
			
		hr = wp->init(pd3dDevice, !wpVec[i].quad, checkpointID++);

		wp->setPosition(Vector2D(wpVec[i].x, wpVec[i].y));

		if (wp->isOnTrack())
		{
			m_waypoints.push_back(wp);
		}
		else
		{
			//m_waypoints.push_back(wp);
			m_quadpoints.push_back(wp);
		}
	}

	// pathfinding
	// create the bounding boxes
	if (m_quadpoints.size() % 4 != 0)
	{
		OutputDebugStringA("Something has gone wrong when loading the bounding box points");
	}

	for (unsigned int i=0; i< m_quadpoints.size(); i+= 4)
	{
		BoundingBox bb = CollisionHelper::createBoundingBoxFromPoints(m_quadpoints[i]->getPosition(), 
			m_quadpoints[i+1]->getPosition(), 
			m_quadpoints[i+2]->getPosition(), 
			m_quadpoints[i+3]->getPosition());

		m_boundingBoxes.push_back(new BoundingBox(bb));
	}


	return hr;
}

void WaypointManager::destroyWaypoints()
{
	for (Waypoint* wp : m_waypoints)
	{
		delete wp;
	}

	m_waypoints.clear();

	
	for (Waypoint* wp : m_quadpoints)
	{
		delete wp;
	}

	m_quadpoints.clear();
	
}


Waypoint* WaypointManager::getWaypoint(const unsigned int index)
{
	if (index >= m_waypoints.size())
		return nullptr;

	return m_waypoints[index];
}

Waypoint* WaypointManager::getRandomWaypoint()
{
	unsigned int index = rand() % getWaypointCount();	//Get a random int in the range 0 - size
	return getWaypoint(index);	//Get the waypoint associated with that random index.
}

Waypoint* WaypointManager::getQuadpoint(const unsigned int index)
{
	if (index >= m_quadpoints.size())
		return nullptr;

	return m_quadpoints[index];
}

Waypoint* WaypointManager::getNearestWaypoint(Vector2D position)
{
	float shortestDistance = FLT_MAX;
	Waypoint* nearestWP = nullptr;

	for (Waypoint* wp : m_waypoints)
	{
		Vector2D d = wp->getPosition() - position;
		float l = (float)d.LengthSq();
		if (l < shortestDistance)
		{
			shortestDistance = l;
			nearestWP = wp;
		}
	}

	return nearestWP;
}

const BoundingBox* WaypointManager::doesLineCrossBuilding(Line line)
{
	if (line.first == line.second)
		return nullptr;

	bool collision = false;
	for (const BoundingBox* bb : m_boundingBoxes)
	{
		collision = CollisionHelper::doesLineIntersectBoundingBox(*bb, line.first, line.second);
		if (collision == true)
		{
			return bb;
		}
	}

	return nullptr;
}

const BoundingBox* WaypointManager::doesLineCrossVehicle(Line line)
{
	return nullptr;
}

//Line WaypointManager::getNearestEdge(BoundingBox* bb, Vector2D position)
//{
//	const unsigned int corners3DCount = 8;
//	XMFLOAT3 corners3D[corners3DCount];
//	bb->GetCorners(corners3D);
//
//	std::array<Vector2D, 4> corners2D = std::array<Vector2D, 4>();
//	std::array<Vector2D, 4>::iterator corners2Diterator = corners2D.begin();
//
//	for (XMFLOAT3 corner3D : corners3D)	//For each of the 8 corners
//	{
//		Vector2D corner = Vector2D(corner3D.x, corner3D.y);	//Convert the corner to a 2D variant
//		
//		bool present = false;	//Check if we already have that corner
//		for (Vector2D corner2D : corners2D)
//		{
//			if (corner2D == corner)
//			{
//				present = true;
//				break;
//			}
//		}
//
//		if (!present)	//If we don't, add it in.
//		{
//			*corners2Diterator = corner;
//			corners2Diterator++;
//		}
//	}
//
//	Line edges[4];	//pair up the corners into edges
//	
//
//}

float WaypointManager::getDistanceBetweenLineAndPoint(Line line, Vector2D point)
{
	Vector2D AB = line.second - line.first;
	Vector2D BP = point - line.second;
	Vector2D AP = point - line.first;

	Vector2D toPoint;
	//if the dot product of (LineA to LineB) and (point) > 0, the point must be b itself
	if (AB.Dot(BP) > 0)
	{
		return line.second.Distance(toPoint);
	}
	//if the dot product of (LineA to LineB) and (point) < 0, the point must be A itself
	else if(AB.Dot(AP) < 0)
	{
		return line.first.Distance(toPoint);
	}
	//if the dot product of (LineA to LineB) and (point) == 0, the point is perpendicular to  the line segement, and the distance can be calculated with:
	//line to point = (lineA to LineB) * (LineA to point) / Length(LineA to LineB)
	else 
	{
		//Find perpendicular distance
		return abs(AB.x * AP.y - AB.y * AP.x) / AB.Length();
	}
}

float WaypointManager::getSquaredDistanceBetweenLineAndPoint(Line line, Vector2D point)
{
	Vector2D AB = line.second - line.first;
	Vector2D BP = point - line.second;
	Vector2D AP = point - line.first;

	Vector2D toPoint;
	//if the dot product of (LineA to LineB) and (point) > 0, the point must be b itself
	if (AB.Dot(BP) > 0)
	{
		return line.second.DistanceSq(toPoint);
	}
	//if the dot product of (LineA to LineB) and (point) < 0, the point must be A itself
	else if(AB.Dot(AP) < 0)
	{
		return line.first.DistanceSq(toPoint);
	}
	//if the dot product of (LineA to LineB) and (point) == 0, the point is perpendicular to  the line segement, and the distance can be calculated with:
	//line to point = (lineA to LineB) * (LineA to point) / Length(LineA to LineB)
	else 
	{
		//Find perpendicular distance
		return abs(AB.x * AP.y - AB.y * AP.x) / AB.LengthSq();
	}
}






vecWaypoints WaypointManager::getNeighbouringWaypoints(Waypoint* waypoint)
{
	// not very efficient, should ideally be pre-cached. 
	// should also return a pointer to a vector, not a copy
	vecWaypoints nearest;
	for (Waypoint* wp : m_waypoints)
	{
		float d = waypoint->distanceToWaypoint(wp);
		if (d < NEAR_WAYPOINT_DISTANCE)
		{
			// A line between waypoint and a nearby waypoint may cross a building (and so the car would go through the building). This makes sure this doesn't happen. 
			if (doWaypointsCrossBuilding(waypoint, wp) == false)
			{
				nearest.push_back(wp);
			}
		}
	}

	return nearest;
}

bool WaypointManager::doWaypointsCrossBuilding(Waypoint* wp1, Waypoint* wp2)
{
	if (wp1 == wp2)
		return false; 

	bool collision = false;
	for (const BoundingBox* bb : m_boundingBoxes)
	{
		collision = CollisionHelper::doesLineIntersectBoundingBox(*bb, wp1->getPosition(), wp2->getPosition());
		if (collision == true)
		{
			return true;
		}
	}

	return collision;
}