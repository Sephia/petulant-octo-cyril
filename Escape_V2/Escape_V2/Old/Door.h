//
//  Door.h

#pragma once

#include <SFML/Graphics.hpp>

class Door : public sf::Shape
{
public:
    Door(int x, int y, sf::Texture* texture, int degree, bool open = false, sf::Color* key = new sf::Color(255, 255, 255, 50));
    ~Door();
    
    void Update(float deltatime);
    void Draw(sf::RenderWindow* win);
    bool Open(sf::Color key);
    bool IsOpen();
    sf::CircleShape* GetUseRadius();
    
private:
    bool Unlock(sf::Color key);
    
private:
    sf::CircleShape* mp_useRadius;
    sf::Color* mp_key;
    float m_transparency;
    bool m_open;
    bool m_locked;
    int m_degree;
    
};