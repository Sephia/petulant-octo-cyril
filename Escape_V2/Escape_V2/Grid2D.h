#pragma once

#include "stdafx.h"

class Level;
class CollisionManager;
class FurnitureManager;

class Grid2D
{
public:
	Grid2D(void);
	~Grid2D(void);

	void Init(sf::Sprite* p_sprite, Level* p_level, CollisionManager* p_cm, FurnitureManager* p_fm);
	bool Walkable(int x, int y);
	bool SetWalkable(int x, int y, bool walkable);
	void Draw();

	int GetSquareSize();

private:
	std::vector<std::vector<bool>> m_grid;

	unsigned int width;
	unsigned int height;
	unsigned int squareSize;
};

