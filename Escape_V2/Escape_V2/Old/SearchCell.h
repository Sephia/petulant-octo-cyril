#pragma once

#include "stdafx.h"

#define WORLD_SIZE 500

class SearchCell {
public:
	int m_xcoord, m_ycoord;
	int m_id;
	SearchCell* mp_parent;
	float m_g;
	float m_h;

	SearchCell() : mp_parent(0) {};
	SearchCell(int x, int y, SearchCell* p_parent = 0) : m_xcoord(x), m_ycoord(y),
		mp_parent(p_parent), m_id(y * WORLD_SIZE + x), m_g(0), m_h(0) {};

	float GetF() { return m_g + m_h; }
	float ManHattanDistance(SearchCell* nodeEnd) {
		float x  = static_cast<float>(fabs(this->m_xcoord - static_cast<float>(nodeEnd->m_xcoord)));
		float y  = static_cast<float>(fabs(this->m_ycoord - static_cast<float>(nodeEnd->m_ycoord)));
		return x + y;
	}
};
