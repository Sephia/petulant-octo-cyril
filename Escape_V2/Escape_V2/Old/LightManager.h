//
//  LightManager.h

#pragma once

#include <map>

namespace ltbl
{
    class LightSystem;
    class Light_Point;
}



class LightManager
{
public:
    LightManager(ltbl::LightSystem* lightSystem);
    ~LightManager();
    
    bool AddLight(ltbl::Light_Point* light, void* source);
    ltbl::Light_Point* GetLight(void* source);
    void RemoveLight(void* source);
    
    void Clear();
    
private:
    ltbl::LightSystem* m_lightSystem;
    //länka en ljuspunkt till ett objekt
    std::map<void*, ltbl::Light_Point*> m_lights;
    
};