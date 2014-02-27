//
//  WallManager.h

#pragma once

#include <SFML/Graphics.hpp>

class HullManager;

class WallManager
{
public:
    WallManager(HullManager* hullManager);
    ~WallManager();
    
    //lägg till en vägg
    bool AddWall(sf::ConvexShape* wall);
    sf::ConvexShape* GetWall(int index);
    //hämta antalet väggar i managern
    int GetCount();
    //ta bort en vägg och hullen som tillhör den
    void RemoveWall(int index);
    
    //ladda väggar och hulls från fil
    bool LoadFromFile(std::string filename);
    //ta bort alla väggar och dess hulls
    void Clear();
    
    //möjligtvis kolla efter kollision här
    
private:
    HullManager* mp_hullManager;
    std::vector<sf::ConvexShape*> m_walls;
    
};