//
//  KeyManager.h

#pragma once

#include <SFML/Graphics.hpp>

class LightManager;
class Key;
namespace ltbl { class LightSystem;}

class KeyManager
{
public:
    KeyManager(LightManager* lightManager, ltbl::LightSystem* lightSystem);
    ~KeyManager();
    
    sf::CircleShape* GetPickUpRadius(int index);
    //plocka upp nyckeln och ta bort den från managern
    Key* PickUpKey(sf::CircleShape*);
    
    int GetCount();
    //tar för mycket CPU att rendera flera icke-statitiska ljus
    //void Update(float deltatime);
    void Draw(sf::RenderWindow* win);
    
    bool LoadFromFile(std::string filename);
    void Clear();
    
private:
    LightManager* mp_lightManager;
    ltbl::LightSystem* mp_lightSystem;
    std::map<sf::CircleShape*, Key*> m_keys;
    sf::Texture* m_texture;
    bool m_glowRising;
};