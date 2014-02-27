//
//  HullManager.h
#pragma once

#include <map>
#include <vector>
#include <SFML/Graphics.hpp>

namespace ltbl
{
    class LightSystem;
    class ConvexHull;
}



class HullManager
{
public:
    HullManager(ltbl::LightSystem* lightSystem);
    ~HullManager();
    
    //länka en hull till en vägg (som rimligen borde ha samma form)
    bool AddHull(ltbl::ConvexHull* hull, sf::Shape* wall);
    //hämta väggens hull, om den har någon, annars returnera nullptr
    ltbl::ConvexHull* GetHull(sf::Shape* wall);
    //ta bort en hull om vägen har en
    void RemoveHull(sf::Shape* wall);
    
    //ladda från fil och returnera formerna att användas i WallManager
    std::vector<sf::ConvexShape*> LoadFromFile(std::string filename);
    //ta bort alla hulls och rensa m_hulls
    void Clear();
    
private:
    ltbl::LightSystem* m_lightSystem;
    std::map<sf::Shape*, ltbl::ConvexHull*> m_hulls;
    std::vector<ltbl::ConvexHull*> m_staticHulls;
    
};