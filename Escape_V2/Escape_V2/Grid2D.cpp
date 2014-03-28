#include "stdafx.h"
#include "Grid2D.h"
#include "Level.h"
#include "CollisionManager.h"
#include "Settings.h"
#include "FurnitureManager.h"

Grid2D::Grid2D() {
	squareSize = 30;
}


Grid2D::~Grid2D() {
}

void Grid2D::Init(sf::Sprite* p_sprite, Level* p_level, CollisionManager* cm, FurnitureManager* p_fm) {

	width = static_cast<int>(p_level->GetSpriteSize().x) / squareSize;
	height = static_cast<int>(p_level->GetSpriteSize().y) / squareSize;


	m_grid.resize(height);
	for(unsigned int i = 0; i < m_grid.size(); i++) {
		m_grid.at(i).resize(width);
		for(unsigned int j = 0; j < m_grid.at(i).size(); j++) {
			bool walkable = false;
			if(!cm->Circle_WallCollision(sf::Vector2f(j * squareSize, i * squareSize), squareSize / 2.0f)) {
				sf::Sprite tempSprite = *p_sprite;
				tempSprite.setScale(0.15f, 0.15f);
				tempSprite.setPosition(sf::Vector2f(j * squareSize, i * squareSize));
				for(int f = 0; f < p_fm->GetCount(); f++) {
					if(cm->Circle_FurnitureCollision(tempSprite, *p_fm->GetFurniture(f))) {
						continue;
					}
					walkable = true;
				}
			}
			m_grid.at(i).at(j) = walkable;
		}
	}	
}

bool Grid2D::Walkable(int x, int y) {
	if(x < 0 || y < 0 || x >= width || y >= height) {
		return false;
	}

	return m_grid.at(y).at(x);
}

bool Grid2D::SetWalkable(int x, int y, bool walkable) {
	if(static_cast<unsigned int>(y) < m_grid.size()) {
		if(static_cast<unsigned int>(x) < m_grid.at(y).size()) {
			m_grid.at(y).at(x) = walkable;
			return true;
		}
	}
	return false;
}

void Grid2D::Draw() {
	
	sf::RectangleShape *circ = new sf::RectangleShape(sf::Vector2f(30.0f, 30.0f));
	circ->setOutlineThickness(-2);
	circ->setFillColor(sf::Color(0, 0, 0, 0));

	for(unsigned int i = 0; i < m_grid.size(); i++) {
		for(unsigned int j = 0; j < m_grid.at(i).size(); j++) {
			if(m_grid.at(i).at(j)) {
				circ->setOutlineColor(sf::Color(255, 0, 0));
			}
			else {
				circ->setOutlineColor(sf::Color(0, 255, 255));
			}
			circ->setPosition(static_cast<float>(j * squareSize), static_cast<float>(i * squareSize));
			Settings::ms_window->draw(*circ);
		}
	}
	
}

int Grid2D::GetSquareSize() {
	return squareSize;
}