//
//  WallManager.cpp

#include "WallManager.h"
#include "HullManager.h"


WallManager::WallManager(HullManager* hullManager)
:mp_hullManager(hullManager)
{
    
}
WallManager::~WallManager()
{
    Clear();
}

bool WallManager::AddWall(sf::ConvexShape* wall)
{
    auto it = m_walls.begin();
    if(wall == nullptr)
    {
        return false;
    }
    while (it != m_walls.end())
    {
        if ((*it) == wall)
        {
            return false;
        }
        it++;
    }
    m_walls.push_back(wall);
    return true;
}
sf::ConvexShape* WallManager::GetWall(int index)
{
    if(!(index < 0) && !(static_cast<unsigned int>(index) >= m_walls.size()))
    {
        return m_walls[index];
    }
    else
    {
        return nullptr;
    }
}
int WallManager::GetCount()
{
    return m_walls.size();
}
void WallManager::RemoveWall(int index)
{
    if(!(index < 0) && !(static_cast<unsigned int>(index) >= m_walls.size()))
    {
        mp_hullManager->RemoveHull(m_walls[index]);
        delete m_walls[index];
        m_walls[index] = nullptr;
    }
}

bool WallManager::LoadFromFile(std::string filename)
{
    if (m_walls.size() != 0)
    {
        Clear();
    }
    m_walls = mp_hullManager->LoadFromFile(filename);
    if(m_walls.size()==0)
    {
        return false;
    }
    else
    {
        return true;
    }
}
void WallManager::Clear()
{
    auto it = m_walls.begin();
    while (it < m_walls.end())
    {
        if ((*it) != nullptr)
        {
            mp_hullManager->RemoveHull((*it));
            delete (*it);
            (*it) = nullptr;
            it++;
        }
    }
    m_walls.clear();
}