//
//  Key.cpp

#include "Key.h"
#include "stdafx.h"


Key::Key(int x, int y, sf::Texture* texture, sf::Color* keyColor)
:mp_keyColor(keyColor)
{
    this->setTexture(*texture);
    sf::Vector2i size = static_cast<sf::Vector2i>(texture->getSize());
    this->setTextureRect(sf::Rect<int>(0, 0, size.x, size.y));
    this->setPosition(static_cast<float>(x), static_cast<float>(y));
    this->setOrigin(this->getLocalBounds().width / 2, this->getLocalBounds().height / 2);
    
    sf::Vector2f circlePosition = sf::Vector2f(this->getOrigin());
    float radius = sqrtf(circlePosition.x * circlePosition.x + circlePosition.y * circlePosition.y);
    mp_pickUpRadius = new sf::CircleShape(radius*3);
    mp_pickUpRadius->setOrigin(mp_pickUpRadius->getLocalBounds().width/2, mp_pickUpRadius->getLocalBounds().height/2);
    mp_pickUpRadius->setPosition(this->getPosition());
    mp_keyColor->a = 50;
    mp_pickUpRadius->setFillColor(*mp_keyColor);
    
}
Key::~Key()
{
    delete mp_pickUpRadius;
    delete mp_keyColor;
}

void Key::Draw(sf::RenderWindow* win)
{
    //win->draw(*mp_pickUpRadius);
    win->draw(*this);
}
sf::CircleShape* Key::GetPickUpRadius()
{
    return mp_pickUpRadius;
}