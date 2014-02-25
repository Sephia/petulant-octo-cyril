//
//  LightManager.cpp
#include "LightManager.h"
#include <LTBL/Light/Light_Point.h>
#include <LTBL/Light/LightSystem.h>
#include <map>


LightManager::LightManager(ltbl::LightSystem* lightSystem)
:m_lightSystem(lightSystem)
{
    
}
LightManager::~LightManager()
{
    auto it = m_lights.begin();
    while(it != m_lights.end())
    {
        if(it->second != nullptr)
        {
            m_lightSystem->RemoveLight(it->second);
        }
        it++;
    }
}

bool LightManager::AddLight(ltbl::Light_Point* light, void* source)
{
    std::map<void*, ltbl::Light_Point*>::iterator it;
    it = m_lights.find(source);
    if(it == m_lights.end())
    {
        m_lights.insert(std::make_pair(source, light));
        m_lightSystem->AddLight(light);
        return true;
    }
    else if(it->second == nullptr)
    {
        it->second = light;
        m_lightSystem->AddLight(light);
        return true;
    }
    
    return false;
}
ltbl::Light_Point* LightManager::GetLight(void* source)
{
    std::map<void*, ltbl::Light_Point*>::iterator it;
    ltbl::Light_Point* light = nullptr;
    it = m_lights.find(source);
    if(it != m_lights.end() && source != nullptr)
    {
        light = it->second;
    }
    return light;
}
void LightManager::RemoveLight(void* source)
{
    std::map<void*, ltbl::Light_Point*>::iterator it;
    it = m_lights.find(source);
    if(it != m_lights.end() && source != nullptr)
    {
        m_lightSystem->RemoveLight(it->second);
        it->second = nullptr;
    }
}

void LightManager::Clear()
{
    auto it = m_lights.begin();
    while(it != m_lights.end())
    {
        if(it->second != nullptr)
        {
            m_lightSystem->RemoveLight(it->second);
        }
        it++;
    }
    m_lights.clear();
}