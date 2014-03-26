//
//  FoV.h
#pragma once

#include <SFML/Graphics.hpp>

class WallManager;

class FoV
{
public:
    FoV(WallManager* wallManager, unsigned int width, unsigned int height);
    ~FoV();
    
    void Update(sf::Vector2f position);
    void Draw();
    void SetMask(unsigned int width, unsigned int height);
    
private:
    WallManager* mp_wallManager;
    sf::RenderTexture* m_mask;
};
