// GuardInvestigateState.cpp

#include "GuardInvestigateState.h"
#include "AnimatedSprite.h"
#include "Settings.h"
#include "CollisionManager.h"
#include "FurnitureManager.h"
#include "PathFinding.h"
#include "Grid2D.h"
#include "stdafx.h"

GuardInvestigateState::GuardInvestigateState() {
	m_speed = 200.0f;
}

GuardInvestigateState::~GuardInvestigateState() {

}

void GuardInvestigateState::Enter() {
	m_done = false;
	Settings::ms_soundManager.newSound("../data/sound/ALERTED_GUARD.ogg", false, 800, 1.0f);

	mp_alerted = Settings::ms_soundManager.GetSound("../data/sound/ALERTED_GUARD.ogg")->CreateSound(*mp_position);
	if(!SoundEntity::IsMuted())
	{
		mp_alerted->play();
	}
}

void GuardInvestigateState::Exit() {
	if(mp_alerted != nullptr)
	{
		delete mp_alerted;
		mp_alerted = nullptr;
	}
}

void GuardInvestigateState::Init(int number, sf::Vector2f* p_position, float* p_rotation, AnimatedSprite* sprite, Grid2D* p_grid) {
	m_waypoints.clear();

	m_done = false;
	m_nextState = "";

	mp_sprite = sprite;

	m_number = number;

	mp_position = &Settings::ms_guards.at(m_number);
	mp_rotation = p_rotation;

	for(unsigned int i = 0; i < Settings::m_allGuardWaypoints[m_number].size(); i++) {

		m_waypoints.push_back(Settings::m_allGuardWaypoints[m_number].at(i));

	}

	mp_position = p_position;

	//mp_sprite = sprite;

	mp_grid = p_grid;
	mp_pathfinding = new PathFinding();
	mp_pathfinding->Init(mp_grid);
}

bool GuardInvestigateState::Update(sf::Vector2f playerPosition, CollisionManager* p_collisionManager, FurnitureManager* p_furnitureManager) {
	if(!Detected(playerPosition, p_collisionManager, p_furnitureManager)) {
		Movement();
	}
	else {
		m_nextState = "GuardShootingState";
		m_done = true;
	}

	return m_done;
}

std::string GuardInvestigateState::Next() {
	return m_nextState;
}

bool GuardInvestigateState::IsType(const std::string &c_type) {
	return c_type.compare("GuardInvestigateState") == 0;
}


void GuardInvestigateState::Cleanup() {

}

void GuardInvestigateState::UpdateAnimation(sf::Vector2f playerPosition) {
	mp_sprite->Update();
	mp_sprite->getSprite()->setPosition(*mp_position);
	mp_sprite->getSprite()->setRotation(*mp_rotation);
}

void GuardInvestigateState::AddWaypointToFront(sf::Vector2f waypoint) {
	m_waypoints.clear();
	m_waypoints.push_back(waypoint);
}

void GuardInvestigateState::Movement() {
	if(!mp_pathfinding->m_foundGoal) {
		while(!mp_pathfinding->m_foundGoal) {
			if(!mp_pathfinding->FindPath(*mp_position, m_waypoints.at(0))) {
				break;
			}
		}
	}
	if(mp_pathfinding->m_foundGoal) {
		m_nextPosition = mp_pathfinding->NextPathPos(*mp_position, 12.0f);
	}

	if(m_waypoints.size() > 0) {
		if(m_timer > 0.0f) {
			if(m_timer < 1.0f) {
				Rotate(-1);
			}
			else if(m_timer < 3.0f) {
				Rotate(1);
			}
			else {
				m_nextState = "GuardPatrolState";
				m_done = true;
			}

			m_timer += Settings::ms_deltatime;
			mp_sprite->ChangeAnimation("Guard1Turning.png");
		}
		else if(Rotate(0)) {
			mp_sprite->ChangeAnimation("Guard1Walking.png");
			sf::Vector2f distance = *mp_position - m_nextPosition;
			float dist = sqrtf(distance.x * distance.x + distance.y * distance.y);
			if(dist > 5.0f) {
				distance.x /= dist;
				distance.y /= dist;

				*mp_position = *mp_position - distance * m_speed * Settings::ms_deltatime;
				m_timer = 0.0f;
			}
			else {
				m_timer += Settings::ms_deltatime;
			}
		}
	}
}

bool GuardInvestigateState::Detected(sf::Vector2f playerPosition, CollisionManager* p_collisionManager, FurnitureManager* p_furnitureManager) {
	sf::Vector2f vectorBetween(playerPosition - *mp_position);
	float distance = sqrtf(vectorBetween.x * vectorBetween.x + vectorBetween.y * vectorBetween.y);

	if(distance > 600) {
		return false;
	}

	float angleToPlayer = static_cast<int>(atan2f(vectorBetween.y, -vectorBetween.x) * 180 / static_cast<float>(M_PI) + 180.0f) % 360;
	float directionLooking = -(*mp_rotation) + 90;
	int diffAngle = static_cast<int>(angleToPlayer - directionLooking + 360) % 360;

	if(diffAngle < 330 && diffAngle > 30) {
		return false;
	}
	sf::Vector2f direction;
	if(distance > 0.001) {
		direction = vectorBetween / distance;
	}

	while(distance > 8) {
		if(p_collisionManager->Circle_WallCollision(playerPosition - vectorBetween, 15)) {
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

bool GuardInvestigateState::Rotate(int rotationWay) {
	float rotationToGetTo;
	int diffDegrees;
	float rotationSpeed = 4.0f;

	if(rotationWay < 0) {
		diffDegrees = 10;
	}
	else if(rotationWay > 0) {
		diffDegrees = 350;
	}
	else {
		rotationToGetTo = ( static_cast<int>(atan2(mp_position->y - m_nextPosition.y, mp_position->x - m_nextPosition.x) * 180 / static_cast<float>(M_PI) - 90) + 360 ) % 360;
		diffDegrees = ( static_cast<int>(*mp_rotation - rotationToGetTo) + 720 ) % 360;
	}

	if(diffDegrees < 5 || diffDegrees > 355) {
		*mp_rotation = rotationToGetTo;
		return true;
	}
	else if(diffDegrees >= 185) {
		*mp_rotation += rotationSpeed;
	}
	else if(diffDegrees < 185) {
		*mp_rotation -= rotationSpeed;
	}

	*mp_rotation = static_cast<int>(*mp_rotation) % 360;

	mp_sprite->ChangeAnimation("Guard1Turning.png");

	return false;
}