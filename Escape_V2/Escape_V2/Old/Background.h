#pragma once

class AnimatedSprite;

class Background {
public:
	Background(AnimatedSprite* sprite);
	~Background();

	void Draw();

private:
	AnimatedSprite* mp_sprite;
};