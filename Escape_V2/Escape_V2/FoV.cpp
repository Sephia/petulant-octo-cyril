//
//  FoV.cpp

#include "FoV.h"
#include "WallManager.h"
#include "Settings.h"

FoV::FoV(WallManager* wallManager, unsigned int width, unsigned int height)
:mp_wallManager(wallManager)
{
    m_mask = new sf::RenderTexture();
    m_mask->create(width, height);
}
FoV::~FoV()
{
    if(m_mask != nullptr)
    {
        delete m_mask;
        m_mask = nullptr;
    }
}

void FoV::Update(sf::Vector2f position)
{
    //rensa gamla masken
    m_mask->clear(sf::Color(0,0,0,0));
    //kod för att skapa masken
    sf::RenderWindow* win = Settings::ms_window;
    float maxDistance = sqrtf(win->getSize().x * win->getSize().x + win->getSize().y * win->getSize().y) * 2000;
    //hämta antalet väggar att kolla
    for (int i = 0; i < mp_wallManager->GetCount(); i++)
    {
        //hämta antalet punkter i en convex form
        for (int j = 0; j < mp_wallManager->GetWall(i)->getPointCount(); j++)
        {
            //hämta startpunkten
            sf::Vector2f A = mp_wallManager->GetWall(i)->getPosition() + mp_wallManager->GetWall(i)->getPoint(j);
            //hämta slutpunkten
            sf::Vector2f B;
            if (j==0)
            {
                B = mp_wallManager->GetWall(i)->getPosition() + mp_wallManager->GetWall(i)->getPoint(mp_wallManager->GetWall(i)->getPointCount()-1);
            }
            else
            {
                B = mp_wallManager->GetWall(i)->getPosition() + mp_wallManager->GetWall(i)->getPoint(j-1);
            }
            sf::Vector2f C = position;
            
            //räkna ut avstånden mellan punkterna
            sf::Vector2f f = A - C;
            sf::Vector2f t = B - C;
            
			sf::Vector2f size = static_cast<sf::Vector2f>(win->getSize());
            //kolla om någon av punkterna är i skärmen, och sen om någon av punkterna genomskär skärmen i X eller Y led
            if(
               //i skärmen
               (t.x < -(size.x/2) || t.x > size.x/2 || t.y < -(size.y/2) || t.y > size.y/2)
               &&
               (f.x < -(size.x/2) || f.x > size.x/2 || f.y < -(size.y/2) || f.y > size.y/2)
               &&
               //kolla att den inte går igenom skärmen
               !(
                 (
                  //går igenom skärmen i X-led
                  (t.x < -(size.x/2) && f.x > size.x/2 && t.y > -(size.y/2) && t.y < size.y/2)
                  ||
                  (t.x < -(size.x/2) && f.x > size.x/2 && f.y > -(size.y/2) && f.y < size.y/2)
                  ||
                  (f.x < -(size.x/2) && t.x > size.x/2 && f.y > -(size.y/2) && f.y < size.y/2)
                  ||
                  (f.x < -(size.x/2) && t.x > size.x/2 && t.y > -(size.y/2) && t.y < size.y/2)
                  )
                 
                 ||
                 //går igenom skärmen i Y-led
                 (
                  (t.y < -(size.y/2) && f.y > size.y/2 && t.x > -(size.x/2) && t.x < size.x/2)
                  ||
                  (t.y < -(size.y/2) && f.y > size.y/2 && f.x > -(size.x/2) && f.x < size.x/2)
                  ||
                  (f.y < -(size.y/2) && t.y > size.y/2 && f.x > -(size.x/2) && f.x < size.x/2)
                  ||
                  (f.y < -(size.y/2) && t.y > size.y/2 && t.x > -(size.x/2) && t.x < size.x/2)
                  )
                 )
               )
            {
                continue;
            }
            //beräkna riknting på punkt A
            sf::Vector2f dirA;
            if (f.x != 0 && f.y != 0)
            {
                dirA = f / sqrtf(f.x * f.x + f.y * f.y);
            }
            else
            {
                if (f.x == 0)
                {
                    dirA.x = 0;
                    dirA.y = f.y / fabs(f.y);
                }
                else
                {
                    dirA.x = f.x / fabs(f.x);;
                    dirA.y = 0;
                }
            }
            //beräkna riktning på punkt B
            sf::Vector2f dirB;
            if (t.x != 0 && t.y != 0)
            {
                dirB = t / sqrtf(t.x * t.x + f.y * f.y);
            }
            else
            {
                if (t.x == 0)
                {
                    dirB.x = 0;
                    dirB.y = t.y / fabs(t.y);
                }
                else
                {
                    dirB.x = t.x / fabs(t.x);
                    dirB.y = 0;
                }
            }
            
            sf::Vector2f pointC, pointD;

			pointC = C + (dirB * maxDistance);
			pointD = C + (dirA * maxDistance);

            sf::ConvexShape shadow;
            shadow.setPointCount(4);
            shadow.setPoint(0, A);
            shadow.setPoint(1, B);
			shadow.setPoint(2, pointC);
			shadow.setPoint(3, pointD);

            shadow.setFillColor(sf::Color(0,0,0,255));
            m_mask->draw(shadow);
        }
    }
	m_mask->display();
}
void FoV::Draw()
{
    Settings::ms_window->draw(sf::Sprite(m_mask->getTexture()));
}
/*void FoV::SetMask(unsigned int width, unsigned int height)
{
    m_mask->create(width, height);
}*/