#include "stdafx.h"
#include "Grid2D.h"
#include "Level.h"

Grid2D::Grid2D() {
	squareSize = 10;
}


Grid2D::~Grid2D() {
}

void Grid2D::Init(Level* level) {

	int width = level->GetSpriteSize().x / 10;
	int height = level->GetSpriteSize().y / 10;


	m_grid.resize(height);
	for(int i = 0; i < m_grid.size(); i++) {
		m_grid.at(i).resize(width);
		for(int j = 0; j < m_grid.at(i).size(); j++) {
			m_grid.at(i).at(j) = false;
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