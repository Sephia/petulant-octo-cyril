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

	int width = p_level->GetSpriteSize().x / squareSize;
	int height = p_level->GetSpriteSize().y / squareSize;


	m_grid.resize(height);
	for(int i = 0; i < m_grid.size(); i++) {
		m_grid.at(i).resize(width);
		for(int j = 0; j < m_grid.at(i).size(); j++) {
			m_grid.at(i).at(j) = !cm->Circle_WallCollision(sf::Vector2f(j * squareSize + squareSize / 2.0f, i * squareSize + squareSize / 2.0f), squareSize / 2.0f);
		}
	}	
}

bool Grid2D::Walkable(int x, int y) {
	if(x < 0 || y < 0 || x > 9 || y > 9) {
		return true;
	}
	return m_grid.at(y).at(x);
}

bool Grid2D::SetWalkable(int x, int y, bool walkable) {
	if(y < m_grid.size()) {
		if(x < m_grid.at(y).size()) {
			m_grid.at(y).at(x) = walkable;
			return true;
		}
	}
	return false;
}

void Grid2D::Draw() {
	
	sf::RectangleShape *circ = new sf::RectangleShape(sf::Vector2f(30.0f, 30.0f));
	circ->setOutlineThickness(-2);
	circ->setFillColor(sf::Color(0.0f, 0.0f, 0.0f, 0));

	for(int i = 0; i < m_grid.size(); i++) {
		for(int j = 0; j < m_grid.at(i).size(); j++) {
			if(m_grid.at(i).at(j)) {
				circ->setOutlineColor(sf::Color(255.0f, 0.0f, 0.0f));
			}
			else {
				circ->setOutlineColor(sf::Color(0.0f, 255.0f, 255.0f));
			}
			circ->setPosition(j * squareSize, i * squareSize);
			Settings::ms_window->draw(*circ);
		}
	}
}