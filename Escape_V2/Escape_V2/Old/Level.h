// Level.h

#pragma once

#include "stdafx.h"

class Level {
public:
	Level();
	~Level();

	void Draw();
	Vec2f GetSpriteSize();

private:
	sf::Texture* mp_levelTexture;
	sf::Sprite* mp_levelSprite;
};

