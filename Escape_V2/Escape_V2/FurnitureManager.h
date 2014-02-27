//
//  FurnitureManager.h

#pragma once

#include <SFML/Graphics.hpp>

class HullManager;
namespace ltbl {class LightSystem;}

class FurnitureManager
{
public:
    FurnitureManager(HullManager* hullManager);
    ~FurnitureManager();
    
    //hämta formen att rita ut
    sf::RectangleShape* GetFurniture(int index);
    //hämta antalet väggar i managern
    int GetCount();
    //rita ut alla sprites från m_furniture
    void Draw(sf::RenderWindow* win);
    
    //ladda furnitures från fil
    bool LoadFromFile(std::string filename, ltbl::LightSystem* lm);
    //töm m_furniture och m_textures
    void Clear();
    
    
private:
    HullManager* mp_hullManager;
    std::vector<sf::RectangleShape*> m_furniture;
    std::map<std::string, sf::Texture*> m_textures;
    
};