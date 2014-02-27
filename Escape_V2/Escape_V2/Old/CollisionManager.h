//
//  CollisionManager.h

#pragma once

#include <SFML/Graphics.hpp>

class WallManager;

class CollisionManager
{
public:
    CollisionManager(WallManager* wm /*andra managers som behöver kolla kollision senare, kanske*/);
    ~CollisionManager();
    
    //för kollision mellan väggar och ett rörligt object (bullets är små och snabba, kan uppstå wall hax om dem är för snabba)
    bool Circle_WallCollision(const sf::Vector2f coordinates, float radius);
    
    //tar för mycket CPU kraft att kolla alla linjer i två former, och i vårat projekt kommer vi bara använda circle collision
    //bool Shape_ShapeCollision(const sf::Vector2f coordinates, sf::VertexArray points);
    
    //för kollision mellan karaktären och andra rörliga objekt som guards och bullets
    bool Circle_CircleCollision(const sf::Sprite& sprite1, const sf::Sprite& sprite2);
    
    
private:
    WallManager* m_wallManager;
};