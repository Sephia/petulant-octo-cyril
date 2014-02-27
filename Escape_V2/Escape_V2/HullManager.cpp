//
//  HullManager.cpp

#include "HullManager.h"
#include <LTBL/Light/LightSystem.h>
#include <LTBL/Light/ConvexHull.h>
#include <sstream>
#include <fstream>

HullManager::HullManager(ltbl::LightSystem* lightSystem)
:m_lightSystem(lightSystem)
{
    
}
HullManager::~HullManager()
{
    auto it = m_hulls.begin();
    while(it != m_hulls.end())
    {
        if(it->second != nullptr)
        {
            m_lightSystem->RemoveConvexHull(it->second);
        }
        it++;
    }
}

bool HullManager::AddHull(ltbl::ConvexHull* hull, sf::Shape* wall)
{
    if(wall == nullptr)
    {
        m_staticHulls.push_back(hull);
        m_lightSystem->AddConvexHull(hull);
        return true;
    }
    std::map<sf::Shape*, ltbl::ConvexHull*>::iterator it;
    it = m_hulls.find(wall);
    if(it == m_hulls.end())
    {
        m_hulls.insert(std::make_pair(wall, hull));
        m_lightSystem->AddConvexHull(hull);
        return true;
    }
    else if(it->second == nullptr)
    {
        it->second = hull;
        m_lightSystem->AddConvexHull(hull);
        return true;
    }
    return false;
}
ltbl::ConvexHull* HullManager::GetHull(sf::Shape* wall)
{
    std::map<sf::Shape*, ltbl::ConvexHull*>::iterator it;
    it = m_hulls.find(wall);
    ltbl::ConvexHull* hull = nullptr;
    if(it != m_hulls.end() && wall != nullptr)
    {
        hull = it->second;
    }
    return hull;
}
void HullManager::RemoveHull(sf::Shape* wall)
{
    std::map<sf::Shape*, ltbl::ConvexHull*>::iterator it;
    it = m_hulls.find(wall);
    if(it != m_hulls.end() && wall != nullptr)
    {
        m_lightSystem->RemoveConvexHull(it->second);
        it->second = nullptr;
    }
}
std::vector<sf::ConvexShape*> HullManager::LoadFromFile(std::string filename)
{
    std::vector<sf::ConvexShape*> shapes;
    std::ifstream stream;
    stream.open(filename);
    if(!stream.is_open())
    {
        return shapes;
    }
    std::string row;
    std::getline(stream, row, '\n');
    /*if (*(row.end()-1) == '\r')
    {
        row.erase(row.end()-1);
    }*/
    if (*(row.begin()) == '\xef')
    {
        for(int i =0; i<3; i++)
        {
            row.erase(row.begin());
        }
    }
    while (!stream.eof())
    {
        std::vector<int> sx, sy;
        //ladda in första formen (collision box)
        while (row != "")
        {
            int tx, ty;
            std::stringstream ss(row);
            ss >> tx >> ty;
            sx.push_back(tx);
            sy.push_back(ty);
            
            std::getline(stream, row, '\n');
          /*  if (*(row.end()-1) == '\r')
            {
                row.erase(row.end()-1);
            }*/
        }
        /*std::getline(stream, row, '\n');
        if (*(row.end()-1) == '\r')
        {
            row.erase(row.end()-1);
        }*/
        sf::ConvexShape* shape = new sf::ConvexShape();
        shape->setPointCount(sx.size()-1);
        shape->setPosition(sx[0], sy[0]);
        
        for(int i =1; i < sx.size(); i++)
        {
            shape->setPoint(i-1, sf::Vector2f((float)sx[i]-(float)sx[0], (float)sy[i]-(float)sy[0]));
        }
        shapes.push_back(shape);
        
        //ladda in hulls
        while(row != "\xc2\xa7")
        {
            std::vector<int> x, y;
            std::getline(stream, row, '\n');
            /*if (*(row.end()-1) == '\r')
            {
                row.erase(row.end()-1);
            }*/
            while (row != "" && row != "\xc2\xa7")
            {
                int tx, ty;
                std::stringstream ss(row);
                ss >> tx >> ty;
                x.push_back(tx);
                y.push_back(ty);
                
                std::getline(stream, row, '\n');
                /*if (*(row.end()-1) == '\r')
                {
                    row.erase(row.end()-1);
                }*/
            }
            
            
            
            if(x.size()!=0)
            {
                ltbl::ConvexHull* hull = new ltbl::ConvexHull();
                
                for(int i =1; i < x.size(); i++)
                {
                    hull->m_vertices.push_back(Vec2f((float)x[i]-(float)x[0], - ((float)y[i]-(float)y[0])));
                }
                hull->CalculateNormals();
                hull->CalculateAABB();
                hull->SetWorldCenter(Vec2f((float)x[0], (m_lightSystem->m_viewAABB.GetDims().y - (float)y[0])));
                
                //alternativt ändra till false senare för att inte rendera ljus genom väggarna
                hull->m_renderLightOverHull = false;
                AddHull(hull, nullptr);
            }
        }
        std::getline(stream, row, '\n');
        /*if (*(row.end()-1) == '\r')
        {
            row.erase(row.end()-1);
        }*/
        if (row == "")
        {
            break;
        }
    }
    return shapes;
}
void HullManager::Clear()
{
    auto it = m_hulls.begin();
    while(it != m_hulls.end())
    {
        if(it->second != nullptr)
        {
            m_lightSystem->RemoveConvexHull(it->second);
        }
        it++;
    }
    m_hulls.clear();
}
