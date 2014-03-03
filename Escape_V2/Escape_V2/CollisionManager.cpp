//
//  CollisionManager.cpp

#include "CollisionManager.h"
#include "WallManager.h"
#include "KeyManager.h"
#include "DoorManager.h"
#include "Door.h"
#include "Key.h"
#include "Furniture.h"
#include <cmath>

CollisionManager::CollisionManager(WallManager* wm, KeyManager* km, DoorManager* dm)
:m_wallManager(wm)
,m_keyManager(km)
,m_doorManager(dm)
{
    
}
CollisionManager::~CollisionManager()
{
    
}
bool CollisionManager::Circle_WallCollision(const sf::Vector2f coordinates, float radius)
{
    //hämta antalet väggar att kolla
    for (int i = 0; i < m_wallManager->GetCount(); i++)
    {
        //hämta antalet punkter i en convex form
        for (unsigned int j = 0; j < m_wallManager->GetWall(i)->getPointCount(); j++)
        {
            //hämta startpunkten
            sf::Vector2f A = m_wallManager->GetWall(i)->getPosition() + m_wallManager->GetWall(i)->getPoint(j);
            //hämta slutpunkten
            sf::Vector2f B;
            if (j==0)
            {
                B = m_wallManager->GetWall(i)->getPosition() + m_wallManager->GetWall(i)->getPoint(m_wallManager->GetWall(i)->getPointCount()-1);
            }
            else
            {
                B = m_wallManager->GetWall(i)->getPosition() + m_wallManager->GetWall(i)->getPoint(j-1);
            }
            sf::Vector2f C = coordinates;
            
            //räkna ut avstånden mellan punkterna
            sf::Vector2f f = A - C;
            sf::Vector2f t = B - C;
            sf::Vector2f l = t - f;
            
            
            float a = l.x * l.x + l.y * l.y ;
            float b = 2*(l.x * f.x + l.y * f.y) ;
            float c = f.x * f.x + f.y * f.y - radius*radius ;
            
            float discriminant = b*b-4*a*c;
            if( discriminant < 0 )
            {
                continue;
            }
            else
            {
                discriminant = sqrt( discriminant );
                
                // cirkeln kan penetreras på två olika punkter, ingångspunkten och utgångspunkten
                float t1 = (-b - discriminant)/(2*a);
                float t2 = (-b + discriminant)/(2*a);
                
                // om t1 och t2 är mellan 0 och 1 har linjen gått igenom cirkeln
                // om t1 eller t2 är mellan 0 och 1 har linjen gått in i men inte igenom cirkeln
                if( t1 >= 0 && t1 <= 1 )
                {
                    return true ;
                }
                if( t2 >= 0 && t2 <= 1 )
                {
                    return true ;
                }
                //om t1 och t2 är större än 1 eller mindre än 0 har linjen missat cirkeln helt
            }
            
        }
    }
    //har den gått igenom alla former och kommit till slutet har ingen kollision sket mellan några punkter
    return false;
}
bool CollisionManager::Circle_WallCollision(const sf::Sprite& sprite)
{
    float radius = (sprite.getTextureRect().width * sprite.getScale().x +
                     sprite.getTextureRect().height * sprite.getScale().y)/4;
    sf::Vector2f coordinates = sf::Vector2f(sprite.getGlobalBounds().left + sprite.getGlobalBounds().width / 2.0f,
                                            sprite.getGlobalBounds().top + sprite.getGlobalBounds().height / 2.0f);
    //hämta antalet väggar att kolla
    for (int i = 0; i < m_wallManager->GetCount(); i++)
    {
        //hämta antalet punkter i en convex form
        for (unsigned int j = 0; j < m_wallManager->GetWall(i)->getPointCount(); j++)
        {
            //hämta startpunkten
            sf::Vector2f A = m_wallManager->GetWall(i)->getPosition() + m_wallManager->GetWall(i)->getPoint(j);
            //hämta slutpunkten
            sf::Vector2f B;
            if (j==0)
            {
                B = m_wallManager->GetWall(i)->getPosition() + m_wallManager->GetWall(i)->getPoint(m_wallManager->GetWall(i)->getPointCount()-1);
            }
            else
            {
                B = m_wallManager->GetWall(i)->getPosition() + m_wallManager->GetWall(i)->getPoint(j-1);
            }
            sf::Vector2f C = coordinates;
            
            //räkna ut avstånden mellan punkterna
            sf::Vector2f f = A - C;
            sf::Vector2f t = B - C;
            sf::Vector2f l = t - f;
            
            
            float a = l.x * l.x + l.y * l.y ;
            float b = 2*(l.x * f.x + l.y * f.y) ;
            float c = f.x * f.x + f.y * f.y - radius*radius ;
            
            float discriminant = b*b-4*a*c;
            if( discriminant < 0 )
            {
                continue;
            }
            else
            {
                discriminant = sqrt( discriminant );
                
                // cirkeln kan penetreras på två olika punkter, ingångspunkten och utgångspunkten
                float t1 = (-b - discriminant)/(2*a);
                float t2 = (-b + discriminant)/(2*a);
                
                // om t1 och t2 är mellan 0 och 1 har linjen gått igenom cirkeln
                // om t1 eller t2 är mellan 0 och 1 har linjen gått in i men inte igenom cirkeln
                if( t1 >= 0 && t1 <= 1 )
                {
					return true ;
                }
                if( t2 >= 0 && t2 <= 1 )
                {
                    return true ;
                }
                //om t1 och t2 är större än 1 eller mindre än 0 har linjen missat cirkeln helt
            }
            
        }
    }
    //har den gått igenom alla former och kommit till slutet har ingen kollision sket mellan några punkter
    return false;
}

//otestad (har inga vettiga cirklar) att testa med)
bool CollisionManager::Circle_CircleCollision(const sf::Sprite& sprite1, const sf::Sprite& sprite2)
{
    // avståndet mellan cirklarnas mittpunkter
    sf::Vector2f distance =
    sf::Vector2f(sprite1.getGlobalBounds().left + sprite1.getGlobalBounds().width / 2.0f,
                 sprite1.getGlobalBounds().top + sprite1.getGlobalBounds().height / 2.0f)
    -
    sf::Vector2f(sprite2.getGlobalBounds().left + sprite2.getGlobalBounds().width / 2.0f,
                 sprite2.getGlobalBounds().top + sprite2.getGlobalBounds().height / 2.0f);
    
    float magnitudeOfDistanceSquared = distance.x * distance.x + distance.y * distance.y;
    // beräkna radien på båda objekten
    float radius1 = (sprite1.getTextureRect().width * sprite1.getScale().x +
                     sprite1.getTextureRect().height * sprite1.getScale().y)/4;
    float radius2 = (sprite2.getTextureRect().width * sprite2.getScale().x +
                     sprite2.getTextureRect().height * sprite2.getScale().y)/4;
    
    float maximumCollidingDistanceBetweenBoundings = (radius1 + radius1) * (radius2 + radius2);
    
    return (magnitudeOfDistanceSquared <= maximumCollidingDistanceBetweenBoundings);
}
sf::CircleShape* CollisionManager::Circle_KeyPickup(const sf::Sprite& player)
{
    for(int i = 0; i < m_keyManager->GetCount(); i++)
    {
        // avståndet mellan cirklarnas mittpunkter
        sf::Vector2f distance =
        sf::Vector2f(player.getGlobalBounds().left + player.getGlobalBounds().width / 2.0f,
                     player.getGlobalBounds().top + player.getGlobalBounds().height / 2.0f)
        -
        m_keyManager->GetPickUpRadius(i)->getPosition();
        
        float magnitudeOfDistanceSquared = distance.x * distance.x + distance.y * distance.y;
        // beräkna radien på båda objekten
        float radius1 = (player.getTextureRect().width * player.getScale().x +
                         player.getTextureRect().height * player.getScale().y)/4;
        float radius2 = m_keyManager->GetPickUpRadius(i)->getRadius();
        
        float maximumCollidingDistanceBetweenBoundings = (radius1 + radius1) * (radius2 + radius2);
        
        if(magnitudeOfDistanceSquared <= maximumCollidingDistanceBetweenBoundings)
        {
            return m_keyManager->GetPickUpRadius(i);
        }
    }
    
    return nullptr;
}
sf::CircleShape* CollisionManager::Circle_DoorUse(const sf::Sprite& sprite)
{
    for(int i = 0; i < m_doorManager->GetCount(); i++)
    {
        // avståndet mellan cirklarnas mittpunkter
        sf::Vector2f distance =
        sf::Vector2f(sprite.getGlobalBounds().left + sprite.getGlobalBounds().width / 2.0f,
                     sprite.getGlobalBounds().top + sprite.getGlobalBounds().height / 2.0f)
        -
        m_doorManager->GetUseRadius(i)->getPosition();
        
        float magnitudeOfDistanceSquared = distance.x * distance.x + distance.y * distance.y;
        // beräkna radien på båda objekten
        float radius1 = (sprite.getTextureRect().width * sprite.getScale().x +
                         sprite.getTextureRect().height * sprite.getScale().y)/4;
        float radius2 = m_doorManager->GetUseRadius(i)->getRadius();
        
        float maximumCollidingDistanceBetweenBoundings = (radius1 + radius1) * (radius2 + radius2);
        
        if(magnitudeOfDistanceSquared <= maximumCollidingDistanceBetweenBoundings)
        {
            return m_keyManager->GetPickUpRadius(i);
        }
    }
    
    return nullptr;
}
Door* CollisionManager::Circle_DoorCollision(const sf::Sprite& sprite)
{
    float radius = (sprite.getTextureRect().width * sprite.getScale().x +
                    sprite.getTextureRect().height * sprite.getScale().y)/4;
    sf::Vector2f coordinates = sf::Vector2f(sprite.getGlobalBounds().left + sprite.getGlobalBounds().width / 2.0f,
                                            sprite.getGlobalBounds().top + sprite.getGlobalBounds().height / 2.0f);
    //hämta antalet dörrar att kolla
    for (int i = 0; i < m_doorManager->GetCount(); i++)
    {
        sf::CircleShape* door = m_doorManager->GetUseRadius(i);
        //hämta antalet punkter i en convex form
        for (unsigned int j = 0; j < m_doorManager->GetDoor(door)->getPointCount(); j++)
        {
            //hämta startpunkten
            sf::Vector2f A = m_doorManager->GetDoor(door)->getPosition() + m_doorManager->GetDoor(door)->getPoint(j);
            //hämta slutpunkten
            sf::Vector2f B;
            if (j==0)
            {
                B = m_doorManager->GetDoor(door)->getPosition() + m_doorManager->GetDoor(door)->getPoint(m_doorManager->GetDoor(door)->getPointCount()-1);
            }
            else
            {
                B = m_doorManager->GetDoor(door)->getPosition() + m_doorManager->GetDoor(door)->getPoint(j-1);
            }
            sf::Vector2f C = coordinates;
            
            //räkna ut avstånden mellan punkterna
            sf::Vector2f f = A - C;
            sf::Vector2f t = B - C;
            sf::Vector2f l = t - f;
            
            
            float a = l.x * l.x + l.y * l.y ;
            float b = 2*(l.x * f.x + l.y * f.y) ;
            float c = f.x * f.x + f.y * f.y - radius*radius ;
            
            float discriminant = b*b-4*a*c;
            if( discriminant < 0 )
            {
                continue;
            }
            else
            {
                discriminant = sqrt( discriminant );
                
                // cirkeln kan penetreras på två olika punkter, ingångspunkten och utgångspunkten
                float t1 = (-b - discriminant)/(2*a);
                float t2 = (-b + discriminant)/(2*a);
                
                // om t1 och t2 är mellan 0 och 1 har linjen gått igenom cirkeln
                // om t1 eller t2 är mellan 0 och 1 har linjen gått in i men inte igenom cirkeln
                if( t1 >= 0 && t1 <= 1 )
                {
					return m_doorManager->GetDoor(door);
                }
                if( t2 >= 0 && t2 <= 1 )
                {
                    return m_doorManager->GetDoor(door);
                }
                //om t1 och t2 är större än 1 eller mindre än 0 har linjen missat cirkeln helt
            }
            
        }
    }
    //har den gått igenom alla former och kommit till slutet har ingen kollision sket mellan några punkter
    return nullptr;

}
bool CollisionManager::Circle_FurnitureCollision(const sf::Sprite& player, const Furniture& furniture)
{
    float radius = (player.getTextureRect().width * player.getScale().x +
                    player.getTextureRect().height * player.getScale().y)/4;
    sf::Vector2f coordinates = sf::Vector2f(player.getGlobalBounds().left + player.getGlobalBounds().width / 2.0f,
                                            player.getGlobalBounds().top + player.getGlobalBounds().height / 2.0f);
    
        //hämta antalet punkter i en convex form
        for (unsigned int j = 0; j < furniture.getPointCount(); j++)
        {
            //hämta startpunkten
            sf::Vector2f A = furniture.getPosition() + furniture.getPoint(j);
            //hämta slutpunkten
            sf::Vector2f B;
            if (j==0)
            {
                B = furniture.getPosition() + furniture.getPoint(furniture.getPointCount()-1);
            }
            else
            {
                B = furniture.getPosition() + furniture.getPoint(j-1);
            }
            sf::Vector2f C = coordinates;
            
            //räkna ut avstånden mellan punkterna
            sf::Vector2f f = A - C;
            sf::Vector2f t = B - C;
            sf::Vector2f l = t - f;
            
            
            float a = l.x * l.x + l.y * l.y ;
            float b = 2*(l.x * f.x + l.y * f.y) ;
            float c = f.x * f.x + f.y * f.y - radius*radius ;
            
            float discriminant = b*b-4*a*c;
            if( discriminant < 0 )
            {
                continue;
            }
            else
            {
                discriminant = sqrt( discriminant );
                
                // cirkeln kan penetreras på två olika punkter, ingångspunkten och utgångspunkten
                float t1 = (-b - discriminant)/(2*a);
                float t2 = (-b + discriminant)/(2*a);
                
                // om t1 och t2 är mellan 0 och 1 har linjen gått igenom cirkeln
                // om t1 eller t2 är mellan 0 och 1 har linjen gått in i men inte igenom cirkeln
                if( t1 >= 0 && t1 <= 1 )
                {
					return true ;
                }
                if( t2 >= 0 && t2 <= 1 )
                {
                    return true ;
                }
                //om t1 och t2 är större än 1 eller mindre än 0 har linjen missat cirkeln helt
            }
            
        }
    //har den gått igenom alla former och kommit till slutet har ingen kollision sket mellan några punkter
    return false;
}