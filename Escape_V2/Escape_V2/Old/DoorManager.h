//
//  DoorManager.h

#pragma once

#include <SFML/Graphics.hpp>
#include <LTBL/Light/LightSystem.h>
#include "HullManager.h"


class Door;

class DoorManager
{
public:
    DoorManager(HullManager* hullManager, ltbl::LightSystem* lightsystem);
    ~DoorManager();
    
    //hämta dörren att kolla
    sf::CircleShape* GetUseRadius(int index);
    //försök öppna dörren
    bool OpenDoor(sf::CircleShape* useRadius, sf::Color key = sf::Color(255,255,255,50));
    //hämta plakat
    //Plaque* GetPlaque(sf::CircleShape* useRadius);
    //kolla om dörren är öppen
    bool IsOpen(sf::CircleShape* useRadius);
    
    //hämta antalet dörrar i managern
    int GetCount();
    //uppdatera ljuset på use radius
    void Update(float deltatime);
    //rita ut alla sprites från m_furniture
    void Draw(sf::RenderWindow* win);
    
    //ladda dörrar från fil
    bool LoadFromFile(std::string filename);
    //töm m_doors och m_textures
    void Clear();
    
    
private:
    HullManager* mp_hullManager;
    ltbl::LightSystem* mp_lightSystem;
    std::map<sf::CircleShape*, Door*> m_doors;
    std::map<std::string, sf::Texture*> m_textures;
};