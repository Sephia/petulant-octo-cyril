//
//  GUI.cpp

#include "GUI.h"

GUI::GUI()
{
    m_keyTexture.loadFromFile("../data/Key-Item-HUD.png");
    
}
GUI::~GUI()
{
    Clear();
}

sf::Sprite* GUI::GetItem(int index)
{
    if(index < GetItemCount() && !(index < 0))
    {
        return m_items.at(index);
    }
    return nullptr;
}
void GUI::AddItem(sf::Sprite* item)
{
    if(item != nullptr)
    {
        item->setTexture(m_keyTexture);
        item->setTextureRect(sf::IntRect(0,0,m_keyTexture.getSize().x, m_keyTexture.getSize().y));
        item->setOrigin(0,0);
        
        m_items.push_back(item);
    }
}
int GUI::GetItemCount()
{
    return m_items.size();
}

void GUI::Draw(sf::RenderWindow* window)
{
    int x, y;
    x = window->getView().getCenter().x + window->getSize().x / 2;
    y = window->getView().getCenter().y - window->getSize().y / 2 + 20;
    if (m_items.size() == 0)
    {
        return;
    }
    for(int i = 0; i <m_items.size(); i++)
    {
        x -= m_items.at(i)->getLocalBounds().width + 20;
        m_items.at(i)->setPosition(x, y);
        window->draw(*m_items.at(i));

    }
}
void GUI::Clear()
{
    auto it = m_items.begin();
    while (it != m_items.end())
    {
        delete (*it);
        (*it) = nullptr;
        it++;
    }
}