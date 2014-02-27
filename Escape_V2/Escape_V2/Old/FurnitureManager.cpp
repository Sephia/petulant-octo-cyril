//
//  FurnitureManager.cpp

#include "FurnitureManager.h"
#include <LTBL/Light/ConvexHull.h>
#include <LTBL/Light/LightSystem.h>
#include <fstream>
#include <sstream>

FurnitureManager::FurnitureManager(HullManager* hullManager)
:mp_hullManager(hullManager)
{
    
}
FurnitureManager::~FurnitureManager()
{
    Clear();
}

sf::RectangleShape* FurnitureManager::GetFurniture(int index)
{
    if (!(index < 0) && index < GetCount())
    {
        return m_furniture[index];
    }
    return nullptr;
}

int FurnitureManager::GetCount()
{
    return m_furniture.size();
}

void FurnitureManager::Draw(sf::RenderWindow* win)
{
    if(win != nullptr && m_furniture.size() !=0)
    {
        auto it = m_furniture.begin();
        while (it < m_furniture.end())
        {
            win->draw(*(*it));
            it++;
        }
    }
}

bool FurnitureManager::LoadFromFile(std::string filename, ltbl::LightSystem* lm)
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
            std::stringstream ss(row);
            ss >> x >> y;
            sf::RectangleShape* shape;
            std::string filename;
            ss >> filename;
            if(m_textures.find(filename) == m_textures.end())
            {
                sf::Texture* texture = new sf::Texture();
                std::ifstream image;
                image.open(filename);
                if(!image.is_open())
                {
                    delete texture;
                    continue;
                }
                image.close();
                texture->loadFromFile(filename);
                m_textures.insert(std::make_pair(filename, texture));
                sf::Vector2f size = static_cast<sf::Vector2f>(texture->getSize());
                shape = new sf::RectangleShape(size);
                shape->setTexture(texture);
                //shape->setTextureRect(sf::Rect<int>(0,0,texture->getSize().x, texture->getSize().y));
            }
            else
            {
                sf::Texture* texture = m_textures.find(filename)->second;
                sf::Vector2f size = static_cast<sf::Vector2f>(texture->getSize());
                shape = new sf::RectangleShape(size);
                shape->setTexture(texture);
                //shape->setTextureRect(sf::Rect<int>(0,0,texture->getSize().x, texture->getSize().y));
            }
            shape->setPosition(x, y);
            shape->setOrigin(shape->getTextureRect().width / 2, shape->getTextureRect().height / 2);
            
            //rotation of the image
            int rotation;
            ss >> rotation;
            shape->setRotation(rotation);
            
            //0 = see-through, 1 = semi-transparent, 2+ = solid
            int lightProperty;
            ss >> lightProperty;
            if (lightProperty > 0)
            {
                ltbl::ConvexHull* hull = new ltbl::ConvexHull();
                
                
                for (int i = 0; i < shape->getPointCount(); i++)
                {
                    hull->m_vertices.push_back(Vec2f(shape->getPoint(i).x - shape->getSize().x/2, shape->getPoint(i).y - shape->getSize().y/2));
                    sf::Vector2f vector = shape->getPoint(i);
                    vector.x = (hull->m_vertices[i].x*cosf(-rotation * (M_PI/180)) - hull->m_vertices[i].y * sinf(-rotation * (M_PI/180)));
                    vector.y = (hull->m_vertices[i].x*sinf(-rotation * (M_PI/180)) + hull->m_vertices[i].y * cosf(-rotation * (M_PI/180)));
                    hull->m_vertices[i].x = vector.x;
                    hull->m_vertices[i].y = vector.y;
                }
                hull->CalculateNormals();
                hull->CalculateAABB();
                hull->SetWorldCenter(Vec2f(shape->getPosition().x, (lm->m_viewAABB.GetDims().y - shape->getPosition().y)));
                hull->m_renderLightOverHull = true;
                if(lightProperty == 1)
                {
                    hull->m_transparency = 0.2f;
                }
                m_furniture.push_back(shape);
                mp_hullManager->AddHull(hull, shape);
                lm->AddConvexHull(hull);
            }
            std::getline(stream, row, '\n');
            if (*(row.end()-1) == '\r')
            {
                row.erase(row.end()-1);
            }
        }
        std::getline(stream, row, '\n');
    }
    return true;
}

void FurnitureManager::Clear()
{
    auto it = m_furniture.begin();
    while (it < m_furniture.end())
    {
        if ((*it) != nullptr)
        {
            mp_hullManager->RemoveHull((*it));
            delete (*it);
            (*it) = nullptr;
            it++;
        }
    }
    m_furniture.clear();
    auto it2 = m_textures.begin();
    while (it2 != m_textures.end())
    {
        delete (*it2).second;
        (*it2).second = nullptr;
        it2++;
    }
    m_textures.clear();
}