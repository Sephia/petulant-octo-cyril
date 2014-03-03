#include "stdafx.h"
#include "Grid2D.h"
#include "Level.h"
#include "CollisionManager.h"
#include "Settings.h"

Grid2D::Grid2D() {
	squareSize = 40;
}


Grid2D::~Grid2D() {
}

void Grid2D::Init(Level* p_level, CollisionManager* cm) {

	int width = static_cast<int>(p_level->GetSpriteSize().x) / squareSize;
	int height = static_cast<int>(p_level->GetSpriteSize().y) / squareSize;


	m_grid.resize(height);
	for(unsigned int i = 0; i < m_grid.size(); i++) {
		m_grid.at(i).resize(width);
		for(unsigned int j = 0; j < m_grid.at(i).size(); j++) {
			m_grid.at(i).at(j) = !cm->Circle_WallCollision(sf::Vector2f(j * squareSize + squareSize / 2.0f, i * squareSize + squareSize / 2.0f), squareSize / 2.0f);
		}
	}	
}

bool Grid2D::Walkable(int x, int y) {
	if(x < 0 || y < 0 || x > 92 || y > 142) {
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