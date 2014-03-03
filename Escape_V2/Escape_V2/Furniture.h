//
//  Furniture.h

#include <SFML/Graphics.hpp>

class Furniture : public sf::RectangleShape
{
public:
    Furniture(const sf::Vector2f& size = sf::Vector2f(0, 0));
    float GetNoise();
    void SetNoise(float noise);
    int GetLightProperty();
    void SetLightProperty(int lightProperty);
    
private:
    float m_noise;
    int m_lightProperty;
    
};