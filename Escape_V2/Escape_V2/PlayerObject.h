// PlayerObject.h

#pragma once

#include "stdafx.h"

class AnimatedSprite;
namespace sf{
	class Sprite;
}

class PlayerObject {
public:
	PlayerObject(AnimatedSprite* sprite);
	~PlayerObject();

	void SetPosition(sf::Vector2f position);
	sf::Vector2f GetPosition();

	void Update();
	void SetSneak(bool sneak);
	void SetDirection(sf::Vector2f direction);
	void UpdateSprite();
	bool CollisionDetected(int tries);
	bool ChangeAnimation(const std::string& name);

	sf::Sprite* GetSprite();

	void Draw();

private:
	void Movement();
	void NormalizeDirection();

private:
	sf::Vector2f m_position;
	bool m_sneak;
	sf::Vector2f m_direction;
	float m_speed;
	bool m_alive;

	AnimatedSprite* mp_sprite;

};
