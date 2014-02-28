//
//  CollisionManager.h

#pragma once

#include <SFML/Graphics.hpp>

class WallManager;
class KeyManager;
class DoorManager;
class Door;
class Furniture;

class CollisionManager
{
public:
    CollisionManager(WallManager* wm, KeyManager* km, DoorManager* dm /*, andra managers som behöver kolla kollision senare, kanske*/);
    ~CollisionManager();
    
    //för kollision mellan väggar och ett rörligt object (bullets är små och snabba, kan uppstå wall hax om dem är för snabba)
    bool Circle_WallCollision(const sf::Sprite& sprite);
    bool Circle_WallCollision(const sf::Vector2f coordinates, float radius);
    //tar för mycket CPU kraft att kolla alla linjer i två former, och i vårat projekt kommer vi bara använda circle collision från spelaren och vakter
    //bool Shape_ShapeCollision(const sf::Vector2f coordinates, sf::VertexArray points);
    
    //för kollision mellan karaktären och andra rörliga objekt som guards och bullets
    bool Circle_CircleCollision(const sf::Sprite& sprite1, const sf::Sprite& sprite2);
    //för att kolla kollision mellan spelaren och key pickup radius
    sf::CircleShape* Circle_KeyPickup(const sf::Sprite& player);
    //för kollision mellan spelaren eller en vakt och door use radius
    sf::CircleShape* Circle_DoorUse(const sf::Sprite& sprite);
    //för kollision melllan spelare/vakter och dörrar
    Door* Circle_DoorCollision(const sf::Sprite& sprite);
    //kollision för furniture
    bool Circle_FurnitureCollision(const sf::Sprite& player, const Furniture& furniture);
    
private:
    WallManager* m_wallManager;
    KeyManager* m_keyManager;
    DoorManager* m_doorManager;
};