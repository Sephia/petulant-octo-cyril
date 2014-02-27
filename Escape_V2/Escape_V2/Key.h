//
//  Key.h

#pragma once

#include <SFML/Graphics.hpp>

class Key : public sf::Sprite
{
public:
    Key(int x, int y, sf::Texture* texture, sf::Color* keyColor);
    ~Key();
    
    void Draw(sf::RenderWindow* win);
    sf::CircleShape* GetPickUpRadius();
    
private:
    sf::CircleShape* mp_pickUpRadius;
    sf::Color* mp_keyColor;
    
};