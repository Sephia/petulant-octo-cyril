#pragma once

#include "stdafx.h"
#include "SearchCell.h"
#include "Grid2D.h"

class PathFinding {
public:
	PathFinding();
	~PathFinding();

	void Init(Grid2D* grid);
	void FindPath(sf::Vector2f currentPos, sf::Vector2f targetPos);
	sf::Vector2f NextPathPos(sf::Vector2f pos, float radius);
	void ClearOpenList() { m_openList.clear(); }
	void ClearVisitedList() { m_visitedList.clear(); }
	void ClearPathToGoal() { m_pathToGoal.clear(); }
	bool m_initializedStartGoal;
	bool m_foundGoal;
	void Draw(sf::RenderWindow* window);

private:
	void SetStartAndGoal(SearchCell start, SearchCell goal);
	void PathOpened(int x, int y, float newCost, SearchCell* p_parent);
	SearchCell* GetNextCell();
	void ContinuePath();
	SearchCell* m_startCell;
	SearchCell* m_goalCell;
	std::vector<SearchCell*> m_openList;
	std::vector<SearchCell*> m_visitedList;
	std::vector<sf::Vector2f*> m_pathToGoal;

	Grid2D* m_grid;
};