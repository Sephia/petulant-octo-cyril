#pragma once

#include "stdafx.h"

class Level;
class CollisionManager;

class Grid2D
{
public:
	Grid2D(void);
	~Grid2D(void);

	void Init(Level* p_level, CollisionManager* p_cm);
	bool Walkable(int x, int y);
	bool SetWalkable(int x, int y, bool walkable);
	void Draw();

private:
	std::vector<std::vector<bool>> m_grid;

	unsigned int width;
	unsigned int height;
	unsigned int squareSize;
};

