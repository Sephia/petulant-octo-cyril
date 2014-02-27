#pragma once

#include "stdafx.h"

class Level;

class Grid2D
{
public:
	Grid2D(void);
	~Grid2D(void);

	void Init(Level* level);
	bool Walkable(int x, int y);
	bool SetWalkable(int x, int y, bool walkable);

private:
	std::vector<std::vector<bool>> m_grid;

	int squareSize;
};

