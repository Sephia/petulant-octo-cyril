//
//  KeyManager.cpp

#include "KeyManager.h"
#include <LTBL/Light/LightSystem.h>
#include <LTBL/Light/Light_Point.h>
#include <LTBL/Utils.h>
#include "LightManager.h"
#include "Key.h"
#include "stdafx.h"

KeyManager::KeyManager(LightManager* lightManager, ltbl::LightSystem* lightSystem)
:mp_lightManager(lightManager)
,mp_lightSystem(lightSystem)
{
    m_texture = new sf::Texture();
    m_texture->loadFromFile("../data/Key.png");
    m_glowRising = true;
}
KeyManager::~KeyManager()
{
    Clear();
}

sf::CircleShape* KeyManager::GetPickUpRadius(int index)
{
    if(static_cast<unsigned int>(index) >= m_keys.size() || index < 0)
    {
        return nullptr;
    }
    auto it = m_keys.begin();
    for (int i =0; i<index; i++)
    {
        it++;
    }
    return it->first;
}
Key* KeyManager::PickUpKey(sf::CircleShape* pickUpRadius)
{
    auto it = m_keys.find(pickUpRadius);
    if (it == m_keys.end())
    {
        return nullptr;
    }
    mp_lightManager->RemoveLight(it->second);
    Key* pickup = it->second;
    m_keys.erase(it);
    return pickup;
}

int KeyManager::GetCount()
{
    return m_keys.size();
}
/*void KeyManager::Update(float deltatime)
{
    ltbl::Light_Point* glow;
    auto it = m_keys.begin();
    while (it != m_keys.end())
    {
        glow = mp_lightManager->GetLight(it->second);
        if(m_glowRising)
        {
            glow->SetRadius(glow->GetRadius()+deltatime*10);
        }
        else
        {
            glow->SetRadius(glow->GetRadius()-deltatime*10);
        }
        it++;
    }
    it--;
    if(glow->GetRadius() < it->second->GetPickUpRadius()->getRadius() / 3)
    {
        m_glowRising = true;
    }
    if(glow->GetRadius() > it->second->GetPickUpRadius()->getRadius() / 3 + 1)
    {
        m_glowRising = false;
    }
}*/
void KeyManager::Draw(sf::RenderWindow* win)
{
    auto it = m_keys.begin();
    while (it != m_keys.end())
    {
        it->second->Draw(win);
        it++;
    }
}

bool KeyManager::LoadFromFile(std::string filename){
    std::ifstream stream;
    stream.open(filename);
    if(!stream.is_open())
    {
        return false;
    }
    std::string row;
    std::getline(stream, row, '\n');
    if (*(row.end()-1) == '\r')
    {
        row.erase(row.end()-1);
    }
    if (*(row.begin()) == '\xef')
    {
        for(int i =0; i<3; i++)
        {
            row.erase(row.begin());
        }
    }
    while (!stream.eof())
    {
        
        while(row != "\xa7")
        {
            int x, y;
            std::stringstream ss(row);
            ss >> x >> y;
            
            sf::Color* color = new sf::Color;
            int c;
            ss >> c;
            color->r=c;
            ss >> c;
            color->g=c;
            ss >> c;
            color->b=c;
            
            Key* key = new Key(x, y, m_texture, color);
            m_keys.insert(std::make_pair(key->GetPickUpRadius(), key));
            
            ltbl::Light_Point* glow = new ltbl::Light_Point();
            glow->m_center = Vec2f(key->GetPickUpRadius()->getPosition().x, mp_lightSystem->m_viewAABB.GetDims().y - key->GetPickUpRadius()->getPosition().y);
            glow->m_radius = key->GetPickUpRadius()->getRadius();
            glow->m_size = 1.0f;
            glow->m_color.r = static_cast<float>(color->r) / 255.0f;
            glow->m_color.g = static_cast<float>(color->g) / 255.0f;
            glow->m_color.b = static_cast<float>(color->b) / 255.0f;
            glow->m_intensity = 2.0f;
            glow->m_spreadAngle = ltbl::pifTimes2;
            glow->m_softSpreadAngle = 0.0f;
            glow->CalculateAABB();
            
            mp_lightManager->AddLight(glow, key);
            
            //glow->SetAlwaysUpdate(false);
            
            std::getline(stream, row, '\n');
            if (*(row.end()-1) == '\r')
            {
                row.erase(row.end()-1);
            }
        }
        std::getline(stream, row, '\n');
        if (row == "")
        {
            break;
        }
    }
    return true;
}
void KeyManager::Clear()
{
    delete m_texture;
    m_texture = nullptr;
    auto it = m_keys.begin();
    while (it != m_keys.end())
    {
        mp_lightManager->RemoveLight(it->second);
        delete it->second;
        it->second = nullptr;
        it++;
    }
    m_keys.clear();
}