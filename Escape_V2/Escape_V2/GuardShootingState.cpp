// GuardShootingState.cpp

#include "GuardShootingState.h"
#include "AnimatedSprite.h"
#include "CollisionManager.h"
#include "FurnitureManager.h"
#include "Furniture.h"
#include "Settings.h"
#include "PathFinding.h"
#include "Grid2D.h"
#include "stdafx.h"

GuardShootingState::GuardShootingState() {
	m_nextState = "GuardSearchState";
	m_timer = 0.0f;
	mp_shooting = nullptr;
}

GuardShootingState::~GuardShootingState() {
	if(mp_pathfinding != nullptr) {
		delete mp_pathfinding;
		mp_pathfinding = nullptr;
	}
}

void GuardShootingState::Enter() {
	m_done = false;
	m_timer = 0.0f;
	mp_sprite->ChangeAnimation("Guard1DrawWeapon.png");
}

void GuardShootingState::Exit() {
	if(mp_shooting != nullptr)
	{
		delete mp_shooting;
		mp_shooting = nullptr;
	}
}

void GuardShootingState::Init(int number, sf::Vector2f* p_position, float* p_rotation, AnimatedSprite* sprite, Grid2D* p_grid) {
	m_waypoints.clear();

	mp_position = p_position;
	mp_rotation = p_rotation;

	m_done = false;
	m_nextState = "";

	mp_sprite = sprite;

	mp_grid = p_grid;
	mp_pathfinding = new PathFinding();
	mp_pathfinding->Init(mp_grid);
}

bool GuardShootingState::Update(sf::Vector2f playerPosition, CollisionManager* p_collisionManager, FurnitureManager* p_furnitureManager) {
	if(!Detected(playerPosition, p_collisionManager, p_furnitureManager)) {
		m_nextState = "GuardChaseState";
		m_done = true;
	}


	if(m_timer > 2.5f) {
		if(!SoundEntity::IsMuted())
        {
            mp_shooting->play();
            mp_shooting->setPosition(mp_position->x, 0, mp_position->y);
        }
		Shoot(p_collisionManager, playerPosition);
	}
	else if(m_timer > 1.0f) {
		mp_sprite->ChangeAnimation("Guard1Shooting.png");
	}

	m_timer += Settings::ms_deltatime;

	return m_done;
}

std::string GuardShootingState::Next() {
	return m_nextState;
}

bool GuardShootingState::IsType(const std::string &c_type) {
	return c_type.compare("GuardShootingState") == 0;
}


void GuardShootingState::Cleanup() {

}

void GuardShootingState::UpdateAnimation(sf::Vector2f playerPosition) {
	mp_sprite->Update();
	mp_sprite->getSprite()->setRotation(*mp_rotation);
}

void GuardShootingState::AddWaypointToFront(sf::Vector2f waypoint) {

}

bool GuardShootingState::Detected(sf::Vector2f playerPosition, CollisionManager* p_collisionManager, FurnitureManager* p_furnitureManager) {
	sf::Vector2f vectorBetween(playerPosition - *mp_position);
	float distance = sqrtf(vectorBetween.x * vectorBetween.x + vectorBetween.y * vectorBetween.y);

	if(distance > 800) {
		return false;
	}

	Rotate(playerPosition);

	sf::Vector2f direction;
	if(distance > 0.001f) {
		direction = vectorBetween / distance;
	}

	while(distance > 8) {
		if(p_collisionManager->Circle_WallCollision(playerPosition - vectorBetween, 10)) {
			return false;
		}
		sf::Sprite tempSprite = *mp_sprite->getSprite();
		tempSprite.setScale(0.1, 0.1);
		tempSprite.setPosition(playerPosition - vectorBetween);

		if(p_collisionManager->Circle_DoorCollision(tempSprite) != nullptr) {
			return false;
		}
		for(int i = 0; i < p_furnitureManager->GetCount(); i++) {	
			if(p_collisionManager->Circle_FurnitureCollision(tempSprite, *p_furnitureManager->GetFurniture(i))) {
				return false;
			}
		}

		vectorBetween.x -= direction.x * 10;
		vectorBetween.y -= direction.y * 10;


		distance = sqrtf(vectorBetween.x * vectorBetween.x + vectorBetween.y * vectorBetween.y);
	}
	return true;
}

bool GuardShootingState::Rotate(sf::Vector2f playerPosition) {
	int rotationToGetTo = ( static_cast<int>(atan2(mp_position->y - playerPosition.y, mp_position->x - playerPosition.x) * 180 / static_cast<float>(M_PI) - 90) + 360 ) % 360;
	*mp_rotation = rotationToGetTo;

	return false;
}

void GuardShootingState::Shoot(CollisionManager* p_collisionManager, sf::Vector2f playerPosition) {
	Settings::Shoot(*mp_position, playerPosition);
	Settings::ms_gameOver = true;
}
