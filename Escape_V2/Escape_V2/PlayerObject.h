// PlayerObject.h

#pragma once

#include "stdafx.h"

class AnimatedSprite;
class CollisionManager;
class FurnitureManager;
namespace sf{
	class Sprite;
}

class PlayerObject {
public:
	PlayerObject(AnimatedSprite* sprite);
	~PlayerObject();

	void SetPosition(sf::Vector2f position);
	sf::Vector2f GetPosition();

	int Update(CollisionManager* p_collisionManager, FurnitureManager* p_furnitureManager);
	void SetSneak(bool sneak);
	void SetDirection(sf::Vector2f direction);
	void UpdateSprite();
	bool ChangeAnimation(const std::string& name);
	bool IsRunning();
	bool IsRightFoot();

	sf::Sprite* GetSprite();
	void Draw();

private:
	int Movement(CollisionManager* p_collisionManager, FurnitureManager* p_furnitureManager);
	void NormalizeDirection();

private:
	sf::Vector2f m_position;
	bool m_sneak;
	sf::Vector2f m_direction;
	float m_speed;
	bool m_alive;
	bool m_rightFoot;

	AnimatedSprite* mp_sprite;

};
