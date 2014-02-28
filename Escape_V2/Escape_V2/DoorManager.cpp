//
//  DoorManager.cpp

#include "DoorManager.h"
#include "Door.h"
#include <LTBL/Light/ConvexHull.h>
#include <LTBL/Light/LightSystem.h>
#include "HullManager.h"
#include "fstream"
#include "sstream"

const float M_PI = 3.14159265359;

DoorManager::DoorManager(HullManager* hullManager, ltbl::LightSystem* lightsystem)
:mp_hullManager(hullManager)
,mp_lightSystem(lightsystem)
{
    
}
DoorManager::~DoorManager()
{
    Clear();
}

sf::CircleShape* DoorManager::GetUseRadius(int index)
{
    if(index >= m_doors.size() || index < 0)
    {
        return nullptr;
    }
    auto it = m_doors.begin();
    for (int i =0; i<index; i++)
    {
        it++;
    }
    return it->first;
}
Door* DoorManager::GetDoor(sf::CircleShape *useRadius)
{
    auto it = m_doors.find(useRadius);
    if (it != m_doors.end())
    {
        return it->second;
    }
    return nullptr;
}
bool DoorManager::OpenDoor(sf::CircleShape* useRadius, sf::Color key)
{
    auto it = m_doors.find(useRadius);
    if (it == m_doors.end())
    {
        return false;
    }
    if(it->second->Open(key))
    {
        Door* door = it->second;
        ltbl::ConvexHull* hull = mp_hullManager->GetHull(door);
        if(door->IsOpen())
        {
            int rotation = -90;
            for (int i = 0; i < door->getPointCount(); i++)
            {
                sf::Vector2f vector = door->getPoint(i);
                vector.x = (hull->m_vertices[i].x*cosf(-rotation * (M_PI/180)) - hull->m_vertices[i].y * sinf(-rotation * (M_PI/180)));
                vector.y = (hull->m_vertices[i].x*sinf(-rotation * (M_PI/180)) + hull->m_vertices[i].y * cosf(-rotation * (M_PI/180)));
                hull->m_vertices[i].x = vector.x;
                hull->m_vertices[i].y = vector.y;
            }
            hull->CalculateNormals();
            hull->CalculateAABB();
            hull->SetWorldCenter(Vec2f(door->getPosition().x, (mp_lightSystem->m_viewAABB.GetDims().y - door->getPosition().y)));
        }
        else
        {
            Door* door = it->second;
            ltbl::ConvexHull* hull = mp_hullManager->GetHull(door);
            int rotation = 90;
            for (int i = 0; i < door->getPointCount(); i++)
            {
                sf::Vector2f vector = door->getPoint(i);
                vector.x = (hull->m_vertices[i].x*cosf(-rotation * (M_PI/180)) - hull->m_vertices[i].y * sinf(-rotation * (M_PI/180)));
                vector.y = (hull->m_vertices[i].x*sinf(-rotation * (M_PI/180)) + hull->m_vertices[i].y * cosf(-rotation * (M_PI/180)));
                hull->m_vertices[i].x = vector.x;
                hull->m_vertices[i].y = vector.y;
            }
            hull->CalculateNormals();
            hull->CalculateAABB();
            hull->SetWorldCenter(Vec2f(door->getPosition().x, (mp_lightSystem->m_viewAABB.GetDims().y - door->getPosition().y)));
        }
        return true;
    }
    return false;
}
/*Plaque* DoorManager::GetPlaque(sf::CircleShape* useRadius)
{
    
}*/
bool DoorManager::IsOpen(sf::CircleShape* useRadius)
{
    auto it = m_doors.find(useRadius);
    if (it == m_doors.end())
    {
        return false;
    }
    return it->second->IsOpen();
}

int DoorManager::GetCount()
{
    return m_doors.size();
}
void DoorManager::Update(float deltatime)
{
    auto it = m_doors.begin();
    while (it != m_doors.end())
    {
        it->second->Update(deltatime);
        it++;
    }
}
void DoorManager::Draw(sf::RenderWindow* win)
{
    auto it = m_doors.begin();
    while (it != m_doors.end())
    {
        it->second->Draw(win);
        it++;
    }
}

bool DoorManager::LoadFromFile(std::string filename)
{
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
        
        while(row != "\xc2\xa7")
        {
            int x, y;
            Door* door;
            std::stringstream ss(row);
            ss >> x >> y;
            std::string filename;
            
            //rotation of the image
            int rotation;
            ss >> rotation;
            
            //plaque or not
            std::string pl;
            bool plaque;
            ss >> pl;
            if(pl == "True")
            {
                plaque = true;
            }
            else
            {
                plaque = false;
            }
            sf::Color* color = new sf::Color;
            int c;
            ss >> c;
            color->r=c;
            ss >> c;
            color->g=c;
            ss >> c;
            color->b=c;
            color->a = 50;
            
            std::string textureName;
            ss >> textureName;
            if(m_textures.find(textureName)== m_textures.end())
            {
                sf::Texture* texture = new sf::Texture();
                std::ifstream image;
                image.open(textureName);
                if(!image.is_open())
                {
                    delete texture;
                    delete color;
                    std::getline(stream, row, '\n');
                    continue;
                }
                image.close();
                texture->loadFromFile(textureName);
                m_textures.insert(std::make_pair(textureName, texture));
                door = new Door(x, y, texture, rotation, color);
            }
            else
            {
                sf::Texture* texture = m_textures.find(textureName)->second;
                door = new Door(x, y, texture, rotation, color);
            }
            
            

            ltbl::ConvexHull* hull = new ltbl::ConvexHull();
            
            
            for (int i = 0; i < door->getPointCount(); i++)
            {
                hull->m_vertices.push_back(Vec2f(door->getPoint(i).x, door->getPoint(i).y - door->getSize().y));
                sf::Vector2f vector = door->getPoint(i);
                vector.x = (hull->m_vertices[i].x*cosf(-rotation * (M_PI/180)) - hull->m_vertices[i].y * sinf(-rotation * (M_PI/180)));
                vector.y = (hull->m_vertices[i].x*sinf(-rotation * (M_PI/180)) + hull->m_vertices[i].y * cosf(-rotation * (M_PI/180)));
                hull->m_vertices[i].x = vector.x;
                hull->m_vertices[i].y = vector.y;
            }
            hull->CalculateNormals();
            hull->CalculateAABB();
            hull->SetWorldCenter(Vec2f(door->getPosition().x, (mp_lightSystem->m_viewAABB.GetDims().y - door->getPosition().y)));
            hull->m_renderLightOverHull = true;
            hull->m_transparency = 0.5f;
            
            m_doors.insert(std::make_pair(door->GetUseRadius(), door));
            mp_hullManager->AddHull(hull, door);
            
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
void DoorManager::Clear()
{
    auto it = m_doors.begin();
    while (it != m_doors.end())
    {
        if (it->second != nullptr)
        {
            mp_hullManager->RemoveHull(it->second);
            delete it->second;
            it->second = nullptr;
            it++;
        }
    }
    m_doors.clear();
    auto it2 = m_textures.begin();
    while (it2 != m_textures.end())
    {
        delete it2->second;
        it2->second = nullptr;
        it2++;
    }
    m_textures.clear();
}