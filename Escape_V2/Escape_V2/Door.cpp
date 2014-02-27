//
//  Door.cpp

#include "Door.h"
#include <cmath>

const float M_PI = 3.14159265359;

Door::Door(int x, int y, sf::Texture* texture, int degree, sf::Color* key, bool open/*, Plaque* plaque*/)
:sf::RectangleShape(static_cast<sf::Vector2f>(texture->getSize()))
,m_degree(degree)
,mp_key(key)
,m_open(open)
/*,m_plaque(plaque)*/
{
    this->setTexture(texture);
    sf::Vector2i size = static_cast<sf::Vector2i>(texture->getSize());
    this->setTextureRect(sf::Rect<int>(0, 0, size.x, size.y));
    this->setPosition(x, y);
    this->setRotation(degree);
    if (*mp_key != sf::Color(255, 255, 255, 50))
    {
        m_locked = true;
    }
    else
    {
        m_locked = false;
    }
    sf::Vector2f circlePosition = sf::Vector2f(this->getLocalBounds().width / 2, this->getLocalBounds().height / 2);
    float radius = sqrtf(circlePosition.x * circlePosition.x + circlePosition.y * circlePosition.y);
    mp_useRadius = new sf::CircleShape(radius);
    mp_useRadius->setOrigin(mp_useRadius->getLocalBounds().width/2, mp_useRadius->getLocalBounds().height/2);
    sf::Vector2f newCirclePosition;
    newCirclePosition.x = circlePosition.x * cosf(m_degree * (M_PI / 180)) - circlePosition.y * sinf(m_degree * (M_PI / 180));
    newCirclePosition.y = circlePosition.x * sinf(m_degree * (M_PI / 180)) + circlePosition.y * cosf(m_degree * (M_PI / 180));
    mp_useRadius->setPosition(this->getPosition() + newCirclePosition);
    mp_useRadius->setFillColor(*mp_key);
}
Door::~Door()
{
    delete mp_key;
    delete mp_useRadius;
}

void Door::Update(float deltatime)
{
    m_transparency -= 20 * deltatime;
    if(m_transparency < 0 || m_transparency > 50)
    {
        m_transparency = 50;
    }
    mp_key->a = static_cast<int>(m_transparency);
    mp_useRadius->setFillColor(*mp_key);
}
void Door::Draw(sf::RenderWindow* win)
{
    win->draw(*mp_useRadius);
    win->draw(*this);
}
bool Door::Open(sf::Color key)
{
    if (m_locked)
    {
        if(Unlock(key))
        {
            this->setRotation(static_cast<int>(this->getRotation()-90)%360);
            m_degree = this->getRotation();
            sf::Vector2f circlePosition = sf::Vector2f(this->getLocalBounds().width / 2, this->getLocalBounds().height / 2);
            sf::Vector2f newCirclePosition;
            newCirclePosition.x = circlePosition.x * cosf(m_degree * (M_PI / 180)) - circlePosition.y * sinf(m_degree * (M_PI / 180));
            newCirclePosition.y = circlePosition.x * sinf(m_degree * (M_PI / 180)) + circlePosition.y * cosf(m_degree * (M_PI / 180));
            mp_useRadius->setPosition(this->getPosition() + newCirclePosition);
            mp_useRadius->setFillColor(*mp_key);
            m_open = !m_open;
            return true;
        }
        return false;
    }
    if(!m_open)
    {
        int rotation = this->getRotation();
        if(this->getRotation() < 90)
        {
            rotation +=360;
        }
            
        this->setRotation((rotation-90)%360);
        m_degree = this->getRotation();
    }
    else
    {
        this->setRotation(static_cast<int>(this->getRotation()+90)%360);
        m_degree = this->getRotation();
    }
    
    sf::Vector2f circlePosition = sf::Vector2f(this->getLocalBounds().width / 2, this->getLocalBounds().height / 2);
    sf::Vector2f newCirclePosition;
    newCirclePosition.x = circlePosition.x * cosf(m_degree * (M_PI / 180)) - circlePosition.y * sinf(m_degree * (M_PI / 180));
    newCirclePosition.y = circlePosition.x * sinf(m_degree * (M_PI / 180)) + circlePosition.y * cosf(m_degree * (M_PI / 180));
    mp_useRadius->setPosition(this->getPosition() + newCirclePosition);
    m_open = !m_open;
    return true;
}
bool Door::IsOpen()
{
    return m_open;
}
bool Door::Unlock(sf::Color key)
{
    if(mp_key->r == key.r && mp_key->g == key.g && mp_key->g == key.g)
    {
        m_locked = false;
        *mp_key = sf::Color(255, 255, 255, 50);
        return true;
    }
    return false;
}
sf::CircleShape* Door::GetUseRadius()
{
    return mp_useRadius;
}