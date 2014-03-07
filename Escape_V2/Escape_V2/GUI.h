//
//  GUI.h

#pragma once

#include <SFML/Graphics.hpp>

class GUI
{
public:
    GUI();
    ~GUI();
    
    sf::Sprite* GetItem(int);
    void AddItem(sf::Sprite* item);
    int GetItemCount();
    
    void Draw(sf::RenderWindow* window);
    void Clear();
    
private:
    std::vector<sf::Sprite*> m_items;
    sf::Texture m_keyTexture;
    
};