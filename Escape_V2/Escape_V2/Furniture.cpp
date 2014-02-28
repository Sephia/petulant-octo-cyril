//
//  Furniture.cpp

#include "Furniture.h"

Furniture::Furniture()
{
    
}
Furniture::Furniture(const sf::Vector2f& size)
:sf::RectangleShape(size)
{
    
}

float Furniture::GetNoise()
{
    return m_noise;
}
void Furniture::SetNoise(float noise)
{
    m_noise = noise;
}
int Furniture::GetLightProperty()
{
    return m_lightProperty;
}
void Furniture::SetLightProperty(int lightProperty)
{
    m_lightProperty = lightProperty;
}