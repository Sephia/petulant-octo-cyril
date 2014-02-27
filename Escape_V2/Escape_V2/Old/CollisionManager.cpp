//
//  CollisionManager.cpp

#include "CollisionManager.h"
#include "WallManager.h"
#include <cmath>

CollisionManager::CollisionManager(WallManager* wm)
:m_wallManager(wm)
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
        for (int j = 0; j < m_wallManager->GetWall(i)->getPointCount(); j++)
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
bool Circle_CircleCollision(sf::Sprite sprite1, sf::Sprite sprite2)
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