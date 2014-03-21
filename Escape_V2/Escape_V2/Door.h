//
//  Door.h

#pragma once

#include <SFML/Graphics.hpp>

class Door : public sf::RectangleShape
{
public:
    Door(int x, int y, sf::Texture* texture, int degree, sf::Color* key = new sf::Color(255, 255, 255, 50), bool open = false);
    ~Door();
    
    void Update(float deltatime);
    void Draw(sf::RenderWindow* win);
    bool Open(sf::Color key);
    //Plaque* GetPlaque();
    bool IsOpen();
    sf::CircleShape* GetUseRadius();
    sf::Vector2f GetPos();
    
private:
    bool Unlock(sf::Color key);
    
private:
    sf::CircleShape* mp_useRadius;
    sf::Color* mp_key;
    float m_transparency;
    bool m_open, m_locked;
    sf::Vector2f m_pos;
    int m_degree;
    //Plaque* m_plaque
    
};