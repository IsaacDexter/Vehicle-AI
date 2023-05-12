#pragma once
#include "DrawableGameObject.h"
class Waypoint :
    public DrawableGameObject
{
public:
	
    virtual HRESULT init(ID3D11Device* pd3dDevice, bool isOnTrack, bool checkPointID);

    bool    isOnTrack()     { return m_isOnTrack; }
    void    setVisible(const bool visible) { m_visible = visible; }
    bool    isVisible()     { return m_visible;}
    int     getCheckpointID() { return m_checkpointID; }
	float   distanceToWaypoint(Waypoint* waypoint);
    

private:
    bool m_isOnTrack;
    bool m_visible;
    int  m_checkpointID;

    const std::wstring m_blueTexture = L"Resources\\blue.dds";
    const std::wstring m_greenTexture = L"Resources\\green.dds";
    const std::wstring m_redTexture = L"Resources\\red.dds";

    void setTexture(std::wstring texture);
    std::wstring m_currentTexture = m_redTexture;
};

