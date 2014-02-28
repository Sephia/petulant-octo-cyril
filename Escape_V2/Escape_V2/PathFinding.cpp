
#include "PathFinding.h"
#include "stdafx.h"


PathFinding::PathFinding(void)
{
	m_initializedStartGoal = false;
	m_foundGoal = false;
}


PathFinding::~PathFinding(void)
{
}

void PathFinding::Init(Grid2D* grid) {
	m_grid = grid;
}

void PathFinding::FindPath(sf::Vector2f currentPos, sf::Vector2f targetPos) {
	if(!m_initializedStartGoal) {
		for(int i = 0; i < m_openList.size(); i++) {
			delete m_openList.at(i);
		}
		m_openList.clear();
		
		for(int i = 0; i < m_visitedList.size(); i++) {
			delete m_visitedList.at(i);
		}
		m_visitedList.clear();

		for(int i = 0; i < m_pathToGoal.size(); i++) {
			delete m_pathToGoal.at(i);
		}
		m_pathToGoal.clear();

		SearchCell start;
		start.m_xcoord = currentPos.x;
		start.m_ycoord = currentPos.y;

		SearchCell goal;
		goal.m_xcoord = targetPos.x;
		goal.m_ycoord = targetPos.y;

		SetStartAndGoal(start, goal);
		m_initializedStartGoal = true;
	}

	if(m_initializedStartGoal) {
		ContinuePath();
	}
}

void PathFinding::SetStartAndGoal(SearchCell start, SearchCell goal) {
	m_startCell = new SearchCell(start.m_xcoord, start.m_ycoord, NULL);
	m_goalCell = new SearchCell(goal.m_xcoord, goal.m_ycoord, &goal);

	m_startCell->m_g = 0;
	m_startCell->m_h = m_startCell->ManHattanDistance(m_goalCell);
	m_startCell->mp_parent = 0;

	m_openList.push_back(m_startCell);
}

SearchCell* PathFinding::GetNextCell() {
	float bestF = 9999999.0f;
	int cellIndex = -1;
	SearchCell* nextCell = NULL;

	for(int i = 0; i < m_openList.size(); i++) {
		if(m_openList.at(i)->GetF() < bestF) {
			bestF = m_openList.at(i)->GetF();
			cellIndex = i;
		}
	}

	if(cellIndex >= 0) {
		nextCell = m_openList.at(cellIndex);
		m_visitedList.push_back(nextCell);
		m_openList.erase(m_openList.begin() + cellIndex);
	}

	return nextCell;
}

void PathFinding::PathOpened(int x, int y, float newCost, SearchCell* p_parent) {
	if(!m_grid->Walkable(x, y)) {
		return;
	}

	int id = y * WORLD_SIZE + x;
	for(int i = 0; i < m_visitedList.size(); i++) {
		if(id == m_visitedList.at(i)->m_id) {
			return;
		}
	}

	SearchCell* newChild = new SearchCell(x, y, p_parent);
	newChild->m_g = newCost;
	newChild->m_h = p_parent->ManHattanDistance(m_goalCell);

	for(int i = 0; i < m_openList.size(); i++) {
		if(id == m_openList.at(i)->m_id) {
			float newF = newChild->m_g + newCost + m_openList.at(i)->m_h;

			if(m_openList.at(i)->GetF() > newF) {
				m_openList.at(i)->m_g = newChild->m_g = newChild->m_g + newCost;
				m_openList.at(i)->mp_parent = newChild;
			}
			else {
				delete newChild;
				return;
			}
		}
	}

	m_openList.push_back(newChild);
}

void PathFinding::ContinuePath() {
	if(m_openList.empty()) {
		return;
	}

	SearchCell* currentCell = GetNextCell();

	if(currentCell->m_id == m_goalCell->m_id) {
		m_goalCell->mp_parent = currentCell->mp_parent;

		SearchCell* getPath;

		for(getPath = m_goalCell; getPath != NULL; getPath = getPath->mp_parent) {
			m_pathToGoal.push_back(new sf::Vector2f(getPath->m_xcoord, getPath->m_ycoord));
		}

		m_foundGoal = true;
		return;
	}
	else {
		//right
		PathOpened(currentCell->m_xcoord + 1, currentCell->m_ycoord, currentCell->m_g + 10, currentCell);
		//left
		PathOpened(currentCell->m_xcoord - 1, currentCell->m_ycoord, currentCell->m_g + 10, currentCell);
		//down
		PathOpened(currentCell->m_xcoord, currentCell->m_ycoord + 1, currentCell->m_g + 10, currentCell);
		//up
		PathOpened(currentCell->m_xcoord, currentCell->m_ycoord - 1, currentCell->m_g + 10, currentCell);
		/*//left_down diagonal
		PathOpened(currentCell->m_xcoord - 1, currentCell->m_ycoord + 1, currentCell->m_g + 14, currentCell);
		//right_down diagonal
		PathOpened(currentCell->m_xcoord + 1, currentCell->m_ycoord + 1, currentCell->m_g + 14, currentCell);
		//left_up diagonal
		PathOpened(currentCell->m_xcoord - 1, currentCell->m_ycoord - 1, currentCell->m_g + 14, currentCell);
		//right_up diagonal
		PathOpened(currentCell->m_xcoord + 1, currentCell->m_ycoord - 1, currentCell->m_g + 14, currentCell);
		*/
		for(int i = 0; i < m_openList.size(); i++) {
			if(currentCell->m_id == m_openList.at(i)->m_id) {
				m_openList.erase(m_openList.begin() + i);
			}
		}
	}
}

sf::Vector2f PathFinding::NextPathPos(sf::Vector2f pos, float radius) {
	int index = 1;

	sf::Vector2f nextPos;
	nextPos.x = m_pathToGoal.at(m_pathToGoal.size() - index)->x;
	nextPos.y = m_pathToGoal.at(m_pathToGoal.size() - index)->y;

	sf::Vector2f distance = nextPos - pos;

	if(index < m_pathToGoal.size()) {
		if(sqrtf(distance.x * distance.x + distance.y * distance.y) < radius) {
			m_pathToGoal.erase(m_pathToGoal.end() - index);
		}
	}

	return nextPos;
}


void PathFinding::Draw(sf::RenderWindow* window) {
	sf::RectangleShape rec(sf::Vector2f(50, 50));
	rec.setFillColor(sf::Color(150, 150, 150));
	for(int i = 0; i < this->m_pathToGoal.size(); i++) {
		rec.setPosition((*this->m_pathToGoal.at(i)).x * 50, (*this->m_pathToGoal.at(i)).y * 50);
		window->draw(rec);
	}
}