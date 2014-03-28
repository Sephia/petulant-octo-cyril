//
//  FurnitureManager.cpp

#include "FurnitureManager.h"
#include "Furniture.h"
#include <LTBL/Light/ConvexHull.h>
#include <LTBL/Light/LightSystem.h>
#include "HullManager.h"
#include <fstream>
#include <sstream>
#include "stdafx.h"

FurnitureManager::FurnitureManager(HullManager* hullManager)
:mp_hullManager(hullManager)
{
    
}

FurnitureManager::~FurnitureManager()
{
    Clear();
}

Furniture* FurnitureManager::GetFurniture(int index)
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
	std::string path = "../data/sprites/";
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
        
        while(row.find("§") == std::string::npos)
        {
            int x, y;
            std::stringstream ss(row);
            ss >> x >> y;
            Furniture* shape;
            std::string textureName;
            ss >> textureName;

			
			if(textureName == "OfficeChair.png") {			//Randomize which office chair to pick 1-6
				int random = (rand() % 6) + 1;
				std::stringstream ss2;
				ss2 << random;
				textureName = "OfficeChair_" + ss2.str() + ".png";
			}
			else if(textureName == "SecretaryDesk.png") {	//Randomize which secretary desk to pick 1-9
				int random = (rand() % 50) + 1;
				if(random > 11) {
					random = random % 5 + 1;
				}
				std::stringstream ss2;
				ss2 << random;
				textureName = "SecretaryDesk_" + ss2.str() + ".png";
			}
			else if(textureName == "Sofa.png") {			//Randomize which sofa to pick 1-2
				int random = (rand() % 2) + 1;
				std::stringstream ss2;
				ss2 << random;
				textureName = "Sofa_" + ss2.str() + ".png";
			}
			else if(textureName == "CafeTable.png") {		//Randomize which cafeteria table to pick 1-4
				int random = (rand() % 4) + 1;
				std::stringstream ss2;
				ss2 << random;
				textureName = "CafeTable_" + ss2.str() + ".png";
			}
			else if(textureName == "FileCabinet.png") {		//Randomize which filing cabinet to pick 1-3
				int random = (rand() % 3) + 1;
				std::stringstream ss2;
				ss2 << random;
				textureName = "FileCabinet_" + ss2.str() + ".png";
			}
			else if(textureName == "Plant.png") {			//Randomize which plant to pick 2-4
				int random = (rand() % 3) + 2;
				std::stringstream ss2;
				ss2 << random;
				textureName = "Plant_" + ss2.str() + ".png";
			}


            if(m_textures.find(textureName)== m_textures.end())
            {
                sf::Texture* texture = new sf::Texture();
                std::ifstream image;
                image.open(path + textureName);
                if(!image.is_open())
                {
					std::cout << "Could not find picture " << path + textureName << std::endl;
                    delete texture;
                    std::getline(stream, row, '\n');
                    if (*(row.end()-1) == '\r')
                    {
                        row.erase(row.end()-1);
                    }
                    continue;
                }
                image.close();
                texture->loadFromFile(path + textureName);
                m_textures.insert(std::make_pair(textureName, texture));
                sf::Vector2f size = static_cast<sf::Vector2f>(texture->getSize());
                shape = new Furniture(size);
                shape->setTexture(texture);
                shape->setTextureRect(sf::Rect<int>(0,0,texture->getSize().x, texture->getSize().y));
            }
            else
            {
                sf::Texture* texture = m_textures.find(textureName)->second;
                sf::Vector2f size = static_cast<sf::Vector2f>(texture->getSize());
                shape = new Furniture(size);
                shape->setTexture(texture);
                shape->setTextureRect(sf::Rect<int>(0,0,texture->getSize().x, texture->getSize().y));
            }
            shape->setPosition(static_cast<float>(x), static_cast<float>(y));
            shape->setOrigin(static_cast<float>(shape->getTextureRect().width / 2), static_cast<float>(shape->getTextureRect().height / 2));
            
            //rotation of the image
            int rotation;
            ss >> rotation;
            shape->setRotation(static_cast<float>(rotation));
            
            //0 = see-through, 1 = semi-transparent, 2+ = solid
            int lightProperty;
            ss >> lightProperty;
            
            float noise;
            ss >> noise;
            shape->SetNoise(noise);
            
            if (lightProperty > 0)
            {
                ltbl::ConvexHull* hull = new ltbl::ConvexHull();
                
                
                for (unsigned int i = 0; i < shape->getPointCount(); i++)
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
                    hull->m_transparency = 0.0f;
                }
                mp_hullManager->AddHull(hull, shape);
            }
            
			m_furniture.push_back(shape);

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
        delete it2->second;
        it2->second = nullptr;
        it2++;
    }
    m_textures.clear();
}